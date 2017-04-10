//
//  main.c
//  MIPs_Processor
//
//  Created by Andrew Gentry on 4/6/17.
//  Copyright © 2017 agentreed. All rights reserved.
// make change

#include <stdio.h>
#include "instructions.h"
#include "pipe.h"

int main(int argc, const char * argv[]) {
    
    initialize_simulation_memory(); // Must happen before initializing components
    initialize_components();
    step();

    for(int i=0; i<MEMORY_SIZE; i++){
        printf("Memory-%u : 0x%08x | ",i,memory[i]);
        InstructionFormat(memory[i]);
    }

    return 0;
}
