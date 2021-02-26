//
// Created by jacel on 2/18/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// the size of a memory address, in bits
const int MEMORY_SIZE = 1024;
// the size of the cache, in bytes
const int CACHE_SIZE = 1000;
// this size of a cache block, in bytes
const int BLOCK_SIZE = 64;
// the associativity of the cache
const int ASSOCIATIVITY = 1;
// Write-Back = 0
// Write-Through = 1
const int WRITE_BACK = 0;

const int TAG_LENGTH = 8;

int memory[MEMORY_SIZE];
int cache[CACHE_SIZE];
void initialize();

typedef struct Word {
    int word;
} Word;

Word readWord(unsigned int address);
void writeWord(unsigned int address, Word word);

int main() {
    initialize();
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
    for (i = 0; i < size; ++i) {
        if (!(i & 3) && i != 0) {
            printf(" ");
        }
        printf("%d", memory[i]);
    }
}

Word readWord(unsigned int address) {
    int memory_block = address / BLOCK_SIZE;
    int byte_range = memory_block * BLOCK_SIZE;
    int slot = (byte_range / BLOCK_SIZE) % TAG_LENGTH;
    int tag = (byte_range / BLOCK_SIZE) / TAG_LENGTH;

}

void writeWord(unsigned int address, Word word) {

}
