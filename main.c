//
//  main.c
//  MIPs_Processor
//
//  Created by Andrew Gentry on 4/6/17.
//  Copyright © 2017 agentreed. All rights reserved.
//

#include <stdio.h>
#include "Load_Program.h"
#include "config.h"

int main(int argc, const char * argv[]) {
    
    extern unsigned int memory[MEMORY_SIZE];
    
    Initialize_Simulation_Memory();
    
    for(int i=0; i<MEMORY_SIZE; i++){
        printf("Memory-%u : 0x%08x\n",i,memory[i]);
    }

    return 0;
}
