//
//  cache.c
//  MIPs_Processor
//
#include <stdio.h>
#include <cache.h>


void initialize_data_cache(cache *d_cache) {
	d_cache.misses = 0;
	d_cache.hits = 0;
	d_cache.size = DCACHE_SIZE;
	d_cache.block_size = BLOCK_SIZE;
}

void initialize_inst_cache(cache *i_cache) {
	i_cache.misses = 0;
	i_cache.hits = 0;
	i_cache.size = ICACHE_SIZE;
	i_cache.block_size = BLOCK_SIZE;
}

void readDataCache(unsigned int data, unsigned int address) {
		
}

void readInstCache( unsigned int inst, unsigned int address) {

}

void writeDataCache(unsigned int data, unsigned int address) {

}

void writeInstCache(unsigned int inst, unsigned int address) {

}

void decodeAddress(cache *cache_op, unsigned int *tag, unsigned int *block)


