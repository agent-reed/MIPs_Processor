//
//  components.h
//  MIPs_Processor
//
//  Created by Andrew Gentry on 4/6/17.
//  Copyright © 2017 agentreed. All rights reserved.
//

#ifndef components_h
#define components_h

#define MEMORY_SIZE			4096
#define DMEMORY_SIZE		4096
#define IMEMORY_SIZE		4096

#define REGISTER_COUNT		32

// Registers
#define SP					29
#define FP					30

unsigned int PC;
unsigned int PC_zero;
unsigned int PC_one;

// Memory Declarations
unsigned int reg_file[REGISTER_COUNT];
unsigned int memory[MEMORY_SIZE];
unsigned int imemory[IMEMORY_SIZE];
unsigned int dmemory[DMEMORY_SIZE];

//Type Definitions
typedef enum {ALUOP_LWSW = 0, ALUOP_BEQ, ALUOP_R, ALUOP_NOP} alu_op;
typedef enum {STAGE_IF=0x0, STAGE_ID, STAGE_EX, STAGE_MEM, STAGE_WB} stage;
typedef enum {IFID = 0, IDEX, EXMEM, MEMWB} pipeline_reg;
typedef enum {false = 0, true = 1} bool;

bool PC_branch;
// Register Definitions
typedef struct {
    bool pcWrite;
    unsigned int nextPC;
    unsigned int instruction;
    
} IFID_Register;

typedef struct {
    // WriteBack
    bool MemtoReg;
    bool RegWrite;

    // Memory
    bool Branch;
    bool MemRead;
    bool MemWrite;

    // Execution
    bool ALUSrc;
    bool RegDst;
    alu_op ALUOp;
    unsigned opCode;
    
    unsigned int read_reg1;
    unsigned int read_reg2;
    unsigned int signextended;
    
    unsigned short rs;
    unsigned short rt;
    unsigned short rd;
    
} IDEX_Register;

typedef struct {
    // WriteBack
    bool MemtoReg;
    bool RegWrite;
    // Memory
    bool Branch;
    bool MemRead;
    bool MemWrite;
    
    bool zero;
    unsigned int aluResult;
    unsigned int dataToMem;
    unsigned short rd;
    
} EXMEM_Register;

typedef struct {
    // WB
    bool MemtoReg;
    bool RegWrite;
    
    unsigned int memValue;
    unsigned int aluResult;
    unsigned short rd;
    
} MEMWB_Register;


IFID_Register ifid_reg;
IFID_Register ifid_shadow;

IDEX_Register idex_reg;
IDEX_Register idex_shadow;

EXMEM_Register exmem_reg;
EXMEM_Register exmem_shadow;

MEMWB_Register memwb_reg;
MEMWB_Register memwb_shadow;

void initialize_components(void);
void initialize_simulation_memory(void);

#endif /* components_h */
