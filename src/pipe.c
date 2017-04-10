//
//  pipe.c
//  MIPs_Processor
//
//  Created by Andrew Gentry on 4/8/17.
//  Copyright © 2017 agentreed. All rights reserved.
// make change

#include <stdio.h>
#include "pipe.h"

void IF_op(void){
	// Places result into IFID_shadow
	printf("Executing IF_op\n");
}
void ID_op(void){
	// Places result into IDEX_shadow
	printf("Executing ID_op\n");
}
void EX_op(void){
	// Places result into EXMEM_shadow
	printf("Executing EX_op\n");
}
void MEM_op(void){
	// Places result into MEMWB_shadow
	// And probably also stuff into memory[]
	printf("Executing MEM_op\n");
}
void WB_op(void){
    // Places result into IDEX_shadow (I think?)
    printf("Executing WB_op\n");
}

void move_shadows_to_reg(void){
	// set each Stage_reg to be respective Stage_shadow
}

void step(void){
	move_shadows_to_reg();
	IF_op();
	ID_op();
	EX_op();
	MEM_op();
	WB_op();
}


