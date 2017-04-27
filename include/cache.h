//
//  cache.h
//  MIPs_Processor
//
//  Created by Andrew Gentry on 4/6/17.
//  Copyright © 2017 agentreed. All rights reserved.
//

#ifndef cache_h
#define cache_h
#include "pipe.h"

#define MISS_PENALTY		8			// clock cycles

void readDataCache(unsigned int data, unsigned int address);
void readInstCache(unsigned int inst, unsigned int address);
void writeDataCache(unsigned int data, unsigned int address);
void writeInstCache(unsigned int inst, unsigned int address);

void initialize_data_cache(cache *d_cache);
void initialize_inst_cache(cache *i_cache);


#endif /* cache_h */
