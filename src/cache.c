//
//  cache.c
//  MIPs_Processor
//
#include <stdio.h>
#include <cache.h>
#include <limits.h>

// Main entry point for all data reads.
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
		readFromWord(&(DCache[block_index].data[line]), data, address, size);
		return true;
	} else {
		// MISS!
		// go to main memory
		fillCacheBlock(DATA_CACHE, address);
		readFromWord(&(DCache[block_index].data[line]), data, address, size);
		DCache_config->misses++;
		return false;
	}
	return false;
}

bool readInstCache( unsigned int *inst, unsigned int address, data_length size) {
	unsigned int block_index, tag, line, offset;
	unsigned int _inst;
	decodeAddress(INST_CACHE, address, &tag, &block_index, &line, &offset);
	
	if (size != word) {
		printf("ERROR: Instructions are 1 word in size!\n");
		return false;
	}
	
	if ((ICache[block_index].tag == tag) && (ICache[block_index].valid)) {
		// HIT!
		*inst = ICache[block_index].data[line];
		ICache_config->hits++;
		return true;
	} else {
		// MISS!
		_inst = memory[address>>2];
		ICache_config->misses++;
		return false;
	}
	return false;
}

void writeDataCache(unsigned int *data, unsigned int address, data_length size) {
	unsigned int block_index, tag, line, offset;
	
	decodeAddress(DATA_CACHE, address, &tag, &block_index, &line, &offset);
	
	if ((DCache[block_index].tag == tag) && (DCache[block_index].valid)) {
		// Item already exists in cache,
	} else {
		DCache[block_index].tag = tag;
		DCache[block_index].valid = true;
	}
	
	writeToWord(&(DCache[block_index].data[line]), data, address, size);
}

void writeInstCache(unsigned int *inst, unsigned int address, data_length size) {
	unsigned int block_index, tag, line, offset;
	decodeAddress(INST_CACHE, address, &tag, &block_index, &line, &offset);
	
	// TO DO: configure early start cache fill
	// TODO: implement write policy
	
	ICache[block_index].tag = tag;
	ICache[block_index].data[line] = *inst;
	ICache[block_index].valid = true;
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


