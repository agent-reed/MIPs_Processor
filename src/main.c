//
//  main.c
//  MIPs_Processor
//
//  Created by Andrew Gentry on 4/6/17.
//  Copyright © 2017 agentreed. All rights reserved.
// make change

#include <stdio.h>
#include "pipe.h"


int main(int argc, const char * argv[]) {
    int clock_cycle = 0;
    initialize_simulation_memory(); // Must happen before initializing components
    initialize_components();

    while(clock_cycle < 20) {
    	printf("clock_cycle(%d) \n",clock_cycle);
    	step();
    	clock_cycle++;
    }


    //print_memoryTrace();

    return 0;
}
