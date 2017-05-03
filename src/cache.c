//
//  cache.c
//  MIPs_Processor
//
#include <stdio.h>
#include <cache.h>
#include <limits.h>

// Main entry point for all data reads.
// Must specify size of the data (word, half, or byte)
bool readCache(cache_type type, unsigned int *data, unsigned int address, data_length size) {
	if (!CACHE_ENABLE) {
		readFromWord(&memory[address>>2], data, address, size);
		return true;
	}
	if (UNIFIED_CACHE) {
		return readDataCache(data, address, size);
	}	
	if (type == DATA_CACHE) {
		return readDataCache(data, address, size);
	} else {
		return readInstCache(data, address, size);
	} 
}

// Main entry point for all data writes.
// Must specify size of the data (word, half, or byte)
bool writeCache(cache_type type, unsigned int *data, unsigned int address, data_length size){
	if (!CACHE_ENABLE) {
		writeToWord(&memory[address>>2], data, address, size);
		return true;
	}
	if (UNIFIED_CACHE) {
		writeDataCache(data, address, size);
		return true;
	}
	
	if (type == DATA_CACHE) {
		writeDataCache(data, address, size);
		return true;
	} else {
		writeInstCache(data, address, size);
		return true;
	}
}

bool readDataCache(unsigned int *data, unsigned int address, data_length size) {
	unsigned int block_index, tag, line, offset;
	unsigned int _data;
	
	decodeAddress(DATA_CACHE, address, &tag, &block_index, &line, &offset);
	
	if ((DCache[block_index].tag == tag) && (DCache[block_index].valid)) {
		// HIT!
		DCache_config->hits++;
		if (WRITE_THROUGH) {
			// Single cache read penalty
			readFromWord(&(DCache[block_index].data[line]), data, address, size);
			return true;
		} else {
			// TODO: Implement write-back
			return false;
		}
	} else {
		// MISS!
		DCache_config->misses++;
		if (WRITE_THROUGH) {
			// single cache read penalty
			// cache block fill pen
			fillCacheBlock(DATA_CACHE, address);
			readFromWord(&(DCache[block_index].data[line]), data, address, size);
			DCache[block_index].tag = tag;
			DCache[block_index].valid = true;
			return false;
		} else {
			// TODO: Implement write-back
			return false;
		}
	}
	return false;
}

bool readInstCache( unsigned int *inst, unsigned int address, data_length size) {
	unsigned int block_index, tag, line, offset;
	
	decodeAddress(INST_CACHE, address, &tag, &block_index, &line, &offset);
	
	if (size != word) {
		printf("ERROR: Instructions are 1 word in size!\n");
		return false;
	}
	
	if ((ICache[block_index].tag == tag) && (ICache[block_index].valid)) {
		// HIT!
		ICache_config->hits++;
		if (WRITE_THROUGH) {
			readFromWord(&(ICache[block_index].data[line]), inst, address, size);
			return true;
		} else {
			// TODO: Implement write-back
			return false;
		}
	} else {
		// MISS!
		ICache_config->misses++;
		if (WRITE_THROUGH) {
			fillCacheBlock(INST_CACHE, address);
			readFromWord(&(ICache[block_index].data[line]), inst, address, size);
			ICache[block_index].tag = tag;
			ICache[block_index].valid = true;
			return false;
		} else {
			 // TODO: Implement write-back
		}
	}	
	return false;
}

void writeDataCache(unsigned int *data, unsigned int address, data_length size) {
	unsigned int block_index, tag, line, offset;

	decodeAddress(DATA_CACHE, address, &tag, &block_index, &line, &offset);	
	
	if ((DCache[block_index].tag == tag) && (DCache[block_index].valid)) {
		// Item was already in cache
		if (WRITE_THROUGH) {
			// single cache write penalty
			writeToWord(&(memory[address>>2]), data, address, size);
			writeToWord(&(DCache[block_index].data[line]), data, address, size);
		} else {
			// TODO: Implement write-back
		}
	} else {
		// Item wasn't in cache
		if (WRITE_THROUGH) {
			// single memory write penalty
			// cache fill penalty
			writeToWord(&(memory[address>>2]), data, address, size);
			fillCacheBlock(DATA_CACHE, address);
			DCache[block_index].tag = tag;
			DCache[block_index].valid = true;
		} else {
			// TODO: Implement write-back
		}
	}
	
	writeToWord(&(DCache[block_index].data[line]), data, address, size);
}

void writeInstCache(unsigned int *inst, unsigned int address, data_length size) {
	unsigned int block_index, tag, line, offset;
	
	decodeAddress(INST_CACHE, address, &tag, &block_index, &line, &offset);
	
	if (size != word) {
		printf("ERROR: Instructions are 1 word in size!\n");
		return false;
	}
	
	if ((ICache[block_index].tag == tag) && (DCache[block_index].valid)) {
		// Item was already in cache
		if (WRITE_THROUGH) {
			// single cache write penalty
			writeToWord(&(memory[address>>2]), inst, address, size);
			writeToWord(&(ICache[block_index].data[line]), inst, address, size);
		} else {
			// TODO: Implement write-back
		}
	} else {
		// Item wasn't in cache
		if (WRITE_THROUGH) {
			// single memory write penalty
			// cache fill penalty
			writeToWord(&(memory[address>>2]), inst, address, size);
			fillCacheBlock(INST_CACHE, address);
			ICache[block_index].tag = tag;
			ICache[block_index].valid = true;
		} else {
			// TODO: Implment write-back
		}
	}
}

void writeToWord(unsigned int *destination, unsigned int *data, unsigned int address, data_length size) {
	unsigned int _data = 0;
	unsigned int offset = address&3;

	switch(size) {
		case word:
			if (!((offset+1)%4)) {
				printf("ERROR: Tried to write word to non-word-aligned address!\n");
				return;
			}
			*destination = *data;
			return;
		case half:
			if (!((offset+1)%2)) {
				printf("ERROR: Tried to write half-word to non-half-word-aligned address!\n");
				return;
			}
			if (offset == 0) {
				_data = (*destination)&(~HalfMask);
				*destination = _data + *data;
			} else {
				_data = (*destination)&HalfMask;
				*destination = _data + (*data << 16);
			}
			return;
		case byte:
			if (offset == 0) {
				_data = (*destination)&(~ByteMask);
				*destination = _data + *data;
				return;
			}
			if (offset == 1) {
				_data = (*destination)&(~ByteMask2);
				*destination = _data + (*data << 8);
				return;
			}
			if (offset == 2) {
				_data = (*destination)&(~ByteMask3);
				*destination = _data + (*data << 16);
				return;
			}
			if (offset == 3) {
				_data = (*destination)&(~ByteMask4);
				*destination = _data + (*data << 24);
				return;
			}
		default:
			printf("ERROR: Tried to store something that isn't a 'word', 'half', or 'byte'\n");
			break;
	}
	
}

void readFromWord(unsigned int *destination, unsigned int *data, unsigned int address, data_length size){
	unsigned int _data = 0;
	unsigned int offset = address&3;
	
	
	switch(size) {
		case word:
			if (!((offset+1)%4)) {
				printf("ERROR: Tried to read word from non-word-aligned address!\n");
				return;
			}
			*data = *destination;
			return;
		case half:
			if (!((offset+1)%2)) {
				printf("ERROR: Tried to read half-word from non-half-word-aligned address!\n");
				return;
			}
			_data = *destination;
			if (offset == 0) {
				*data = _data&HalfMask;
			} else {
				*data = (_data&(HalfMask<<16))>>16;
			}
			return;
		case byte:
			_data = *destination;
			if (offset == 0) *data = _data&ByteMask;
			if (offset == 1) *data = (_data&(ByteMask << 8))>>8;
			if (offset == 2) *data = (_data&(ByteMask << 16))>>16;
			if (offset == 3) *data = (_data&(ByteMask << 24))>>24;
			return;
		default:
			printf("ERROR: Tried to read something that isn't a 'word', 'half', or 'byte'\n");
			break;
	}
}

void fillCacheBlock(cache_type type, unsigned int address) {
	unsigned int block_index, tag, line, offset;
	unsigned int _address = (address&(~CacheLineMask))>>2;
	cache *_cache;
	
	decodeAddress(type, address, &tag, &block_index, &line, &offset);
	
	if (type == DATA_CACHE) {
		_cache = DCache;
	} else {
		_cache = ICache;
	}
	
	_cache[block_index].tag = tag;
	_cache[block_index].valid = true;
	
	for (int i=0; i<BLOCK_SIZE; i++) {
		_cache[block_index].data[i] = memory[_address+i];
	}
	
} 

void decodeAddress(cache_type type, unsigned int addr, 
						unsigned int *tag, unsigned int *block_index, 
						unsigned int *line, unsigned int *offset) {
							
	int blockLineShift = 0;
	switch(BLOCK_SIZE) {
		case 1:
			break;
		case 4:
			blockLineShift = 2;
			break;
		case 16:
			blockLineShift = 4;
			break;
		default:
			printf("ERROR: Block size is not 1, 4, or 16!");
			break;
	}
	
	unsigned int loc_addr = addr;
	
	*offset = loc_addr&CacheOffsetMask;
	*line = (loc_addr&CacheLineMask) >> 2;
	
	if (type == DATA_CACHE) {
		*block_index = (loc_addr&dCacheBlockMask) >> (2 + blockLineShift);
		*tag = loc_addr&dCacheTagMask;
	} else {
		*block_index = (loc_addr&iCacheBlockMask) >> (2 + blockLineShift);
		*tag = loc_addr&iCacheTagMask;
	}
}

void initialize_cache_masks() {
	int blockIndexShift = 0;
	
	switch(BLOCK_SIZE) {
		case 0:
			break;
		case 4:
			blockIndexShift = 2;
			break;
		case 16:
			blockIndexShift = 4;
		default:
			printf("ERROR: Block size is not 1, 4, or 16!\n");
			break;
	}
	
	ByteMask = 255;
	ByteMask2 = 65280;
	ByteMask3 = 16711680;
	ByteMask4 = 4278190080;
	HalfMask = 65535;
	
	CacheOffsetMask = 3;
	CacheLineMask = (BLOCK_SIZE - 1) << 2;
	if (CACHE_ENABLE) {
		if (UNIFIED_CACHE) {
			dCacheBlockMask = ((DCACHE_SIZE+ICACHE_SIZE)/(BLOCK_SIZE) - 1) << (2 + blockIndexShift);
			dCacheTagMask = (UINT_MAX - dCacheBlockMask - CacheLineMask - CacheOffsetMask);
		} else {
			iCacheBlockMask = (ICACHE_SIZE/BLOCK_SIZE - 1) << (2 + blockIndexShift);
			iCacheTagMask = (UINT_MAX - iCacheBlockMask - CacheLineMask - CacheOffsetMask);
			
			dCacheBlockMask = (DCACHE_SIZE/BLOCK_SIZE - 1) << (2 + blockIndexShift);
			dCacheTagMask = (UINT_MAX - dCacheBlockMask - CacheLineMask - CacheOffsetMask);
		}
	}
}

void TestCache(){
    unsigned int address = 0;
    unsigned int data_in = 66;
	int data1 = 1;
	int data2 = 2;
	int data3 = 3;
	int data4 = 4;
	int odata1 = 0;
	int odata2 = 0;
	int odata3 = 0;
	int odata4 = 0;
	
/*
    writeCache(DATA_CACHE, &data1, address, half);
    writeCache(DATA_CACHE, &data2, address+2, half);    
    
    readCache(DATA_CACHE, &odata1, address, half);
    readCache(DATA_CACHE, &odata2, address+2, half);
    
    printf("*** Test #1 (Halfwords): 		1 = %d, 2 = %d\n", odata1, odata2);
    
    writeCache(DATA_CACHE, &data1, address, byte);
    writeCache(DATA_CACHE, &data2, address+1, byte);
    writeCache(DATA_CACHE, &data3, address+2, byte);
    writeCache(DATA_CACHE, &data4, address+3, byte);
    
    readCache(DATA_CACHE, &odata1, address, byte);
    readCache(DATA_CACHE, &odata2, address+1, byte);
    readCache(DATA_CACHE, &odata3, address+2, byte);
    readCache(DATA_CACHE, &odata4, address+3, byte);
    
    printf("*** Test #2 (Bytes): 			1 = %d, 2 = %d, 3 = %d, 4 = %d\n", odata1, odata2, odata3, odata4);
*/

/*
	memory[0] = 10;
	memory[1] = 11;
	memory[2] = 12;
	memory[3] = 13;
	unsigned int in1 = 11;
	
	unsigned int out1 = 0;
	unsigned int out2 = 0;
	
	readCache(DATA_CACHE, &out1, 4, word);
	readCache(DATA_CACHE, &out2, 8, word);
	printf("*** Test #3 (Cache Fill):  		11 = %d, 12 = %d\n", out1, out2);
*/

	unsigned int address1 = 0;
	unsigned int address2 = 512;
	unsigned int in01 = 10;
	unsigned int in02 = 15;

	unsigned int out01 = 0;
	
	writeCache(DATA_CACHE, &in01, address1, word);
	writeCache(DATA_CACHE, &in02, address2, word);
	
	readCache(DATA_CACHE, &out01, address2, word);
	
	printf("*** Test #4 (Write Through):  	15 = %d, mem[0] = %d, mem[128] = %d\n", out01, memory[0], memory[128]);
	

	if (CACHE_ENABLE) {
		if (!UNIFIED_CACHE) {
			printf("*** DCache  Hits = %d; Misses = %d;\n", DCache_config->hits, DCache_config->misses);
			printf("*** ICache  Hits = %d; Misses = %d;\n", ICache_config->hits, ICache_config->misses);
		} else {
			printf("*** DCache  Hits = %d; Misses = %d;\n", DCache_config->hits, DCache_config->misses);
		}
	}
}


