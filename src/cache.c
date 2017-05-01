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
		// go straight to memory
		// not yet implemented so return false
		return false;
	}
	if (UNIFIED_CACHE) {
		// read from unified cache
		// not yet implemented so return false
		return false;
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
		// go straight to memory
		// not yet implemented so return false
		return false;
	}
	if (UNIFIED_CACHE) {
		// write to unified cache
		// not yet implemented so return false
		return false;
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
		
		switch(size) {
			case word:
				*data = DCache[block_index].data[line];
				return true;
			case half:
				_data = DCache[block_index].data[line];
				if (offset == 0) {
					*data = _data&HalfMask;
				} else {
					*data = (_data&(HalfMask<<16))>>16;
				}
				return true;
			case byte:
				_data = DCache[block_index].data[line];
				if (offset == 0) *data = _data&ByteMask;
				if (offset == 1) *data = (_data&(ByteMask << 8))>>8;
				if (offset == 2) *data = (_data&(ByteMask << 16))>>16;
				if (offset == 3) *data = (_data&(ByteMask << 24))>>24;
				return true;
			default:
				printf("ERROR: Tried to store something that isn't a 'word', 'half', or 'byte'\n");
				break;
		}
		
	} else {
		// MISS!
		// go to main memory
		
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
	unsigned int _data = 0;
	
	if ((DCache[block_index].tag == tag) && (DCache[block_index].valid)) {
		// Item already exists in cache
	} else {
		DCache[block_index].tag = tag;
		DCache[block_index].valid = true;
	}
		// If the block is already in the Cache, just update it.
		switch(size) {
			case word:
				DCache[block_index].data[line] = *data;
				return;
			case half:
				if (offset == 0) {
					_data = DCache[block_index].data[line]&HalfMaskTop;
					DCache[block_index].data[line] = _data + *data;
				} else {
					_data = DCache[block_index].data[line]&HalfMask;
					DCache[block_index].data[line] = _data + (*data << 16);
				}
				return;
			case byte:
				if (offset == 0) {
					_data = DCache[block_index].data[line]&(~ByteMask);
					DCache[block_index].data[line] = _data + *data;
					return;
				}
				if (offset == 1) {
					_data = DCache[block_index].data[line]&(~ByteMask2);
					DCache[block_index].data[line] = _data + (*data << 8);
					return;
				}
				if (offset == 2) {
					_data = DCache[block_index].data[line]&(~ByteMask3);
					DCache[block_index].data[line] = _data + (*data << 16);
					return;
				}
				if (offset == 3) {
					_data = DCache[block_index].data[line]&(~ByteMask4);
					DCache[block_index].data[line] = _data + (*data << 24);
					return;
				}
			default:
				printf("ERROR: Tried to store something that isn't a 'word', 'half', or 'byte'\n");
				break;
		}
}



void writeInstCache(unsigned int *inst, unsigned int address, data_length size) {
	unsigned int block_index, tag, line, offset;
	decodeAddress(INST_CACHE, address, &tag, &block_index, &line, &offset);
	
	ICache[block_index].tag = tag;
	ICache[block_index].data[line] = *inst;
	ICache[block_index].valid = true;
}

void fillCacheBlock(cache_type type, unsigned int address) {
	unsigned int block_index, tag, line, offset;
	decodeAddress(type, address, &tag, &block_index, &line, &offset);
	unsigned int _address = (address&(~CacheLineMask))>>2;
	
	for (int i=0; i<BLOCK_SIZE; i++) {
		
		if (type == DATA_CACHE) {
			DCache[block_index].data[i] = memory[_address+i];
		} else {
			ICache[block_index].data[i] = memory[_address+i];
		}
	}
	
} 

void decodeAddress(cache_type type, unsigned int addr, 
						unsigned int *tag, unsigned int *block_index, 
						unsigned int *line, unsigned int *offset) {
	// TODO: Configure for unified cache
	
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
			printf("ERROR: Block size was not specified!\n");
			break;
	}
	
	unsigned int loc_addr = addr;
	
	*offset = loc_addr&CacheOffsetMask;
	*line = (loc_addr&CacheLineMask) >> 2;
	
	if (type == DATA_CACHE) {
		*block_index = loc_addr&dCacheBlockMask >> (2 + blockIndexShift);
		*tag = loc_addr&dCacheTagMask;
	} else {
		*block_index = loc_addr&iCacheBlockMask >> (2 + blockIndexShift);
		*tag = loc_addr&iCacheTagMask;
	}
}

void initialize_cache_masks() {
	// TODO: Configure for unified cache
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
			printf("ERROR: Block size was not specified!\n");
			break;
	}
	
	ByteMask = 255;
	ByteMask2 = 65280;
	ByteMask3 = 16711680;
	ByteMask4 = 4278190080;
	HalfMask = 65535;
	HalfMaskTop = 4294901760;
	
	CacheOffsetMask = 3;
	CacheLineMask = (BLOCK_SIZE - 1) << 2;
	
	iCacheBlockMask = (ICACHE_SIZE/BLOCK_SIZE - 1) << (2 + blockIndexShift);
	iCacheTagMask = (UINT_MAX - iCacheBlockMask - CacheLineMask - CacheOffsetMask);
	
	dCacheBlockMask = (DCACHE_SIZE/BLOCK_SIZE - 1) << (2 + blockIndexShift);
	dCacheTagMask = (UINT_MAX - dCacheBlockMask - CacheLineMask - CacheOffsetMask);	
}


