//
//  cache.c
//  MIPs_Processor
//
#include <stdio.h>
#include <cache.h>
#include <limits.h>

// Main entry point for all data reads.
bool readCache(cache_type type, unsigned int *data, unsigned int address) {
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
		return readDataCache(data, address);
	} else {
		return readInstCache(data, address);
	} 
}

// Main entry point for all data writes.
bool writeCache(cache_type type, unsigned int *data, unsigned int address){
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
		writeDataCache(data, address);
		return true;
	} else {
		writeInstCache(data, address);
		return true;
	}
}

bool readDataCache(unsigned int *data, unsigned int address) {
	unsigned int block_index, tag, line;
	decodeAddress(DATA_CACHE, address, &tag, &block_index, &line);
	
	if ((DCache[block_index].tag == tag) && (DCache[block_index].valid[line])) {
		// HIT!
		*data = DCache[block_index].data[line];
		return true;
	}
	return false;
}

bool readInstCache( unsigned int *inst, unsigned int address) {
	unsigned int block_index, tag, line;
	decodeAddress(INST_CACHE, address, &tag, &block_index, &line);
	
	if ((ICache[block_index].tag == tag) && (ICache[block_index].valid[line])) {
		// HIT!
		*inst = ICache[block_index].data[line];
		return true;
	}	
	return false;
}

void writeDataCache(unsigned int *data, unsigned int address) {
	unsigned int block_index, tag, line;
	decodeAddress(DATA_CACHE, address, &tag, &block_index, &line);
		
	DCache[block_index].tag = tag;
	DCache[block_index].data[line] = *data;
	DCache[block_index].valid[line] = true;
}

void writeInstCache(unsigned int *inst, unsigned int address) {
	unsigned int block_index, tag, line;
	decodeAddress(INST_CACHE, address, &tag, &block_index, &line);
	
	ICache[block_index].tag = tag;
	ICache[block_index].data[line] = *inst;
	ICache[block_index].valid[line] = true;	
}

void decodeAddress(cache_type type, unsigned int addr, 
						unsigned int *tag, unsigned int *block_index, unsigned int *line) {
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


