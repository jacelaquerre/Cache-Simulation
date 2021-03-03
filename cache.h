//
// Created by jacel on 3/2/2021.
//

#ifndef CAHCE_SIMULATION_CACHE_H
#define CAHCE_SIMULATION_CACHE_H

// the size of a memory address, in bits
#define MEMORY_SIZE  65536
// the size of the cache, in bytes
#define CACHE_SIZE  1024
// this size of a cache block, in bytes
#define BLOCK_SIZE 64
// the associativity of the cache
#define ASSOCIATIVITY 1
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
    // 0 - not valid, 1 = valid
    int valid;
    // 0 - not dirty, 1 = dirty
    int dirty;
} CacheBlock;

typedef struct CacheSet {
    CacheBlock cacheBlock[CACHE_SIZE/BLOCK_SIZE];  // # of blocks
    int tagQueue[(CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY]; // # of sets
} CacheSet;

typedef struct Cache {
    CacheSet cache[CACHE_SIZE];
    // Write-Back = 0, Write-Through = 1
    int write;
} Cache;

Word readWord(unsigned int address);
void writeWord(unsigned int address, Word word);
void initialize();
void output();
int validation(int address);
void displayBinary(int n);

#endif //CAHCE_SIMULATION_CACHE_H

