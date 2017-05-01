//
//  cache.h
//  MIPs_Processor
//
//  Created by Andrew Gentry on 4/6/17.
//  Copyright © 2017 agentreed. All rights reserved.
//

#ifndef cache_h
#define cache_h
#include "tools.h"

#define MISS_PENALTY		8			// clock cycles

unsigned int ByteMask;
unsigned int ByteMask2;
unsigned int ByteMask3;
unsigned int ByteMask4;
unsigned int HalfMask;
unsigned int HalfMaskTop;
unsigned int CacheOffsetMask;
unsigned int CacheLineMask;
unsigned int iCacheBlockMask;
unsigned int iCacheTagMask;
unsigned int dCacheBlockMask;
unsigned int dCacheTagMask;

bool readCache(cache_type type, unsigned int *data, unsigned int address, data_length size);
bool writeCache(cache_type type, unsigned int *data, unsigned int address, data_length size);
bool readDataCache(unsigned int *data, unsigned int address, data_length size);
bool readInstCache(unsigned int *inst, unsigned int address, data_length size);
void writeDataCache(unsigned int *data, unsigned int address, data_length size);
void writeInstCache(unsigned int *inst, unsigned int address, data_length size);

void fillCacheBlock(cache_type type, unsigned int address);
void decodeAddress(cache_type type, unsigned int addr, 
						unsigned int *tag, unsigned int *block_index, 
						unsigned int *line, unsigned int *offset);
void initialize_cache_masks(void);

#endif /* cache_h */
