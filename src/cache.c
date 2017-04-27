//
//  cache.c
//  MIPs_Processor
//
#include <stdio.h>
#include <cache.h>

void readDataCache(unsigned int data, unsigned int address) {
		
}

void readInstCache( unsigned int inst, unsigned int address) {

}

void writeDataCache(unsigned int data, unsigned int address) {

}

void writeInstCache(unsigned int inst, unsigned int address) {
	// TODO: 	Write to Instruction Cache.
	// 			Deal with dirty beats.
}

void decodeAddress(cache cache_in, unsigned int *tag, unsigned int *block) {
	// TODO: Take in an address and pull out the tag, block_index, byte_offset
}


