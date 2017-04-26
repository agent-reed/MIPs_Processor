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
    
    initialize_simulation_memory(); // Must happen before initializing components
    initialize_components();

    int i = 0;
    while(i <10) {
    	printf("step(%d) \n",i);
    	step();
    	i++;
    }


    //print_memoryTrace();

    return 0;
}
