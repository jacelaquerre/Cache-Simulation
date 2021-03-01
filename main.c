//
// Created by jacel on 2/18/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// the size of a memory address, in bits
#define MEMORY_SIZE 65536
// the size of the cache, in bytes
#define CACHE_SIZE  1024
// this size of a cache block, in bytes
#define BLOCK_SIZE 64
// the associativity of the cache
#define ASSOCIATIVITY 1
// Write-Back = 0
// Write-Through = 1
#define WRITE_BACK 0

#define TAG_LENGTH 8

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
    int tagQueue[(CACHE_SIZE/BLOCK_SIZE) * ASSOCIATIVITY];  // # of sets = # blocks * associativity
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

int main() {
    initialize();
    output();
    return 0;
}

void initialize() {
    int size = sizeof memory / sizeof memory[0];
    int i;
    int ct = 0;
    for (i = 0; i < size; ++i) {
        if (!(i & 3) || i == 0) {
            ++ct;
            memory[i] = ct;
        } else {
            memory[i] = 0;
        }
    }
    // Print Array
//    for (i = 0; i < size; ++i) {
//        if (!(i & 3) && i != 0) {
//            printf(" ");
//        }
//        printf("%d", memory[i]);
//    }
}

void output() {
    printf("-----------------------------------\n");
    printf("Cache Size = %d\n", CACHE_SIZE);
    printf("Block Size = %d\n", BLOCK_SIZE);
    printf("#Blocks = %d\n", CACHE_SIZE / BLOCK_SIZE);
    printf("#Sets = %d\n", 0);
    printf("Associativity = %d\n", ASSOCIATIVITY);
    if (WRITE_BACK == 0) {
        printf("Write Back\n");
    } else {
        printf("Write Through\n");
    }
    printf("-----------------------------------");
}

Word readWord(unsigned int address) {
    int memory_block = address / BLOCK_SIZE;
    int byte_range = memory_block * BLOCK_SIZE;
    int slot = (byte_range / BLOCK_SIZE) % TAG_LENGTH;
    int tag = (byte_range / BLOCK_SIZE) / TAG_LENGTH;
    int i;
    for (i = 0; i < (byte_range + (BLOCK_SIZE - 1)); ++i) {
        //memory[i] = cache[slot];
    }
}

void writeWord(unsigned int address, Word word) {

}
