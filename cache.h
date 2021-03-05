//
// Created by jacel on 3/2/2021.
//

#ifndef CAHCE_SIMULATION_CACHE_H
#define CAHCE_SIMULATION_CACHE_H

// the size of a memory address, in bits
#define MEMORY_SIZE  65536
// the size of the cacheSets, in bytes
#define CACHE_SIZE  1024
// this size of a cacheSets block, in bytes
#define BLOCK_SIZE 64
// the associativity of the cacheSets
#define ASSOCIATIVITY 4
// Write-Back = 0
// Write-Through = 1
#define WRITE_BACK 0

int memory[MEMORY_SIZE];

typedef struct Word {
    int word;
} Word;

typedef struct CacheBlock {
    int cacheBlock[BLOCK_SIZE];
    int tag;
    // 0 - valid, 1 = not valid
    int valid;
    // 0 - not dirty, 1 = dirty
    int dirty;
} CacheBlock;

typedef struct CacheSet {
    CacheBlock cacheBlocks[((CACHE_SIZE / BLOCK_SIZE) / ((CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY))];  // # of blocks divided by sets
    int tagQueue[(CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY]; // # of sets
} CacheSet;

typedef struct Cache {
    CacheSet cacheSets[(CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY];
    // Write-Back = 0, Write-Through = 1
    int write;
} Cache;

Word readWord(unsigned int address);
void writeWord(unsigned int address, Word word);
void initialize();
void output();
int validation(int address);
void displayBinary(int n);

// For Evicting
int lastReplacedIndex = -1;

#endif //CAHCE_SIMULATION_CACHE_H

