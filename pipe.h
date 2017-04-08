//
//  pipe.h
//  MIPs_Processor
//
//  Created by Andrew Gentry on 4/6/17.
//  Copyright © 2017 agentreed. All rights reserved.
//

#ifndef pipe_h
#define pipe_h

void IF_op(void);
void ID_op(void);
void EX_op(void);
void MEM_op(void);
void WB_op(void);

void step(void);
void move_shadows_to_reg(void);

void ALU_UnitOperation(unsigned int src1, unsigned int src2);
void CTL_UnitOperation(unsigned int opCode, unsigned int regVal1, unsigned int regVal2, unsigned int extendedValue);


void initialize_pipeline();

#endif /* pipe_h */
