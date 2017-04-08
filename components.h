//
//  components.h
//  MIPs_Processor
//
//  Created by Andrew Gentry on 4/6/17.
//  Copyright © 2017 agentreed. All rights reserved.
//

#ifndef components_h
#define components_h
#include "config.h"


unsigned int reg_file[REGISTER_COUNT];
unsigned int memory[MEMORY_SIZE];
unsigned int imemory[IMEMORY_SIZE];
unsigned int dmemory[DMEMORY_SIZE];


unsigned int PC;

IFID_Register ifid_reg;
IFID_Register ifid_shadow;

IDEX_Register idex_reg;
IDEX_Register idex_shadow;

EXMEM_Register exmem_reg;
EXMEM_Register exmem_shadow;

MEMWB_Register memwb_reg;
MEMWB_Register memwb_shadow;

void init_units();

#endif /* components_h */
