//
//  cache.c
//  MIPs_Processor
//
#include <stdio.h>
#include <cache.h>
#include <limits.h>

bool readDataCache(unsigned int *data, unsigned int address) {
		unsigned int block_index, tag, line;
		decodeAddress(DATA_CACHE, address, &tag, &block_index, &line);
		
		//cache *temp_cache = DCache[block_index];
		
		return false;
}

bool readInstCache( unsigned int *inst, unsigned int address) {
		unsigned int block_index, tag, line;
		decodeAddress(INST_CACHE, address, &tag, &block_index, &line);
		
		return false;
}

void writeDataCache(unsigned int data, unsigned int address) {
		unsigned int block_index, tag, line;
		decodeAddress(DATA_CACHE, address, &tag, &block_index, &line);
		
		DCache[block_index].tag = tag;
		DCache[block_index].data[line] = data;
}

void writeInstCache(unsigned int inst, unsigned int address) {
		unsigned int block_index, tag, line;
		decodeAddress(INST_CACHE, address, &tag, &block_index, &line);
	
		ICache[block_index].tag = tag;
		ICache[block_index].data[line] = inst;	
}

<<<<<<< HEAD
void decodeAddress(cache_type type, unsigned int addr, 
					unsigned int *tag, unsigned int *block_index, unsigned int *line) {
=======
// unsigned int *tag?
void decodeAddress(cache_type type, unsigned int addr, int *tag, int *block_index, int *line) {
>>>>>>> e75c8cedcc9583daf4fc8e149dd76c234c0d423a
	unsigned int loc_addr = addr;
	*line = loc_addr&CacheLineMask;
	
	if (type == DATA_CACHE) {
		*block_index = loc_addr&dCacheBlockMask;
		*tag = loc_addr&dCacheTagMask;
	} else {
		*block_index = loc_addr&iCacheBlockMask;
		*tag = loc_addr&iCacheTagMask;
	}
}

void initialize_cache_masks() {
	CacheLineMask = BLOCK_SIZE - 1;
	
	iCacheBlockMask = ICACHE_SIZE/BLOCK_SIZE - 1;
	iCacheTagMask = UINT_MAX - iCacheBlockMask - CacheLineMask - 1;
	
	dCacheBlockMask = DCACHE_SIZE/BLOCK_SIZE - 1;
	dCacheTagMask = UINT_MAX - dCacheBlockMask - CacheLineMask - 1;
}


