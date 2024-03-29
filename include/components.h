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

// Cache Configuration
#define CACHE_ENABLE		1  			// 1 = Caching enabled,  0 = No Caching
#define UNIFIED_CACHE		0			// 1 = Single cache,   0 = Seperate data & instruction caches
#define DCACHE_SIZE			64			// Size in Words
#define ICACHE_SIZE			64			// Size in Words
#define BLOCK_SIZE			16			// Size in words (4-bytes)
#define WRITE_THROUGH		0 			// 1 = Write Through,  0 = Write Back

// Registers
#define SP					29
#define FP					30

#define REGISTER_COUNT		32

unsigned int PC;
unsigned int PC_zero;
unsigned int PC_one;

// Memory Declarations
unsigned int reg_file[REGISTER_COUNT];
unsigned int memory[MEMORY_SIZE];

//Type Definitions
typedef enum {ALUOP_LWSW = 0, ALUOP_BEQ, ALUOP_R, ALUOP_NOP} alu_op;
typedef enum {STAGE_IF=0x0, STAGE_ID, STAGE_EX, STAGE_MEM, STAGE_WB} stage;
typedef enum {IFID = 0, IDEX, EXMEM, MEMWB} pipeline_reg;
typedef enum {false = 0, true = 1} bool;
typedef enum {INST_CACHE, DATA_CACHE} cache_type;
typedef enum {word = 32, half = 16, byte = 8} data_length; 

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

    int zero;
    
    unsigned int alu_Result;
    unsigned int dataToMem;
    unsigned short rd;
    
} EXMEM_Register;

typedef struct {
    // WB
    bool MemtoReg;
    bool RegWrite;
    
    unsigned int memValue;
    unsigned int alu_Result;
    unsigned short rd;
    
} MEMWB_Register;

// Cache Metadata, each cache gets one.
typedef struct {
	unsigned int hits;
	unsigned int misses;
	int size;
	int block_num;
} cache_config;

// Cache Structure
typedef struct {
	bool valid;
	bool dirty;
	unsigned int tag;
	unsigned int *data;
} cache;

typedef struct {
	unsigned int address;
	unsigned int data[BLOCK_SIZE];
} write_buffer;

cache_config *ICache_config;
cache_config *DCache_config;

cache *ICache;
cache *DCache;

write_buffer *WriteBuffer;

IFID_Register ifid_reg;
IFID_Register ifid_shadow;

IDEX_Register idex_reg;
IDEX_Register idex_shadow;

EXMEM_Register exmem_reg;
EXMEM_Register exmem_shadow;

MEMWB_Register memwb_reg;
MEMWB_Register memwb_shadow;

void initialize_components(void);
void initialize_caches(void);
cache *createCache(int size, int block_num);
cache_config *createCacheConfig(int size, int block_num);

void initialize_simulation_memory(void);

#endif /* components_h */
