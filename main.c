//
//  main.c
//  MIPs_Processor
//
//  Created by Andrew Gentry on 4/6/17.
//  Copyright © 2017 agentreed. All rights reserved.
//

#include <stdio.h>
#include "components.h"
#include "instructions.h"

int main(int argc, const char * argv[]) {
    
    extern unsigned int memory[MEMORY_SIZE]; // must use extern to referece symbol declared in components
    
    initialize_simulation_memory(); // Must happen before initializing components
    initialize_components();
    
    for(int i=0; i<MEMORY_SIZE; i++){
        printf("Memory-%u : 0x%08x | ",i,memory[i]);
        InstructionFormat(memory[i]);
        printf("\n");
    }

    return 0;
}
