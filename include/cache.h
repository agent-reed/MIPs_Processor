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

unsigned int CacheLineMask;
unsigned int iCacheBlockMask;
unsigned int iCacheTagMask;
unsigned int dCacheBlockMask;
unsigned int dCacheTagMask;

bool readDataCache(unsigned int *data, unsigned int address);
bool readInstCache(unsigned int *inst, unsigned int address);
void writeDataCache(unsigned int data, unsigned int address);
void writeInstCache(unsigned int inst, unsigned int address);

void decodeAddress(cache_type type, unsigned int addr, 
					unsigned int *tag, unsigned int *block_index, unsigned int *line);
void initialize_cache_masks(void);

#endif /* cache_h */
