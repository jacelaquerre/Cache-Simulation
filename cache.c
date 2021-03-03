//
// Created by jacel on 2/18/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "cache.h"

int main() {
//    CacheBlock cacheBlock = { .write = WRITE_BACK, .cache = 123 };
//    CacheSet cacheSet = { .tagQueue = [(CACHE_SIZE/BLOCK_SIZE) * ASSOCIATIVITY], .cache = 123 };
//    Cache cache = { .write = WRITE_BACK, .cache = cacheSet };
    initialize();
    output();
    readWord(46916);
    displayBinary(46916);
    //printf("%s", binaryConverter(2460));
    return 0;
}

void initialize() {
    int size = sizeof memory / sizeof memory[0];
    int i;
    int ct = 0;
    for (i = 0; i < size; ++i) {
        if (!(i & 3) || i == 0) {
            if (i == 0) {
                memory[i] = 0;
            } else {
                ct = ct + 4;
                memory[i] = ct;
            }
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
    printf("#Sets = %d\n", (CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY);
    printf("Associativity = %d\n", ASSOCIATIVITY);
    printf("Tag Length = %.0f\n", (CACHE_SIZE/BLOCK_SIZE) - (log2((CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY)) - (log2(BLOCK_SIZE)));
    if (WRITE_BACK == 0) {
        printf("Write Back\n");
    } else {
        printf("Write Through\n");
    }
    printf("-----------------------------------\n");
}

void displayBinary(int n) {
    char bistr[1000];
    itoa(n, bistr, 2); //2 means binary u can convert n upto base 36
    printf("%s", bistr);
}

int validation(int address) {
    // 0 = false, 1 = true
    int valid = 0;
    // check if address is in memory range
    if (address >= 0 && address < MEMORY_SIZE) {
        // check 4 byte alignment
        if (!(address & 0x3)) {
            valid = 1;
        }
    }
    return valid;
}

Word readWord(unsigned int address) {
    if (validation((int)address) == 0) {
        printf("Address not 4 byte aligned or in valid memory range!");
        Word word = { .word = -1};
        return word;
    }
    int memory_block = (int)address / BLOCK_SIZE;
    printf("memory_block %d\n", memory_block);
    int byte_range = memory_block * BLOCK_SIZE;
    printf("byte_range %d\n", byte_range);
    float tagLength = (CACHE_SIZE/BLOCK_SIZE) - (log2((CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY)) - (log2(BLOCK_SIZE));
    printf("tegLength %d\n", (int) tagLength);
    int slot = ((int)address / BLOCK_SIZE) % (int) tagLength;
    printf("slot %d\n", slot);
    int tag = (((int)address / BLOCK_SIZE) / (int) tagLength);
    printf("tag %d\n", tag);
    int blockIndex = 0;
    // OUTPUT
    printf("[addr=%d", (int)address);
    printf(" index=%d", slot);
    printf(" blockIndex=%d", blockIndex);
    printf(" tag=%d", tag);
    // read miss etc
    printf(" word=%d", (int)address);
    printf(" (%d", byte_range);
    printf(" - %d)]\n", (byte_range + (BLOCK_SIZE - 1)));
    int i;
//    for (i = 0; i < (byte_range + (BLOCK_SIZE - 1)); ++i) {
//        //memory[i] = cache[slot];
//    }
    //int wordArray[4] =
    //for (i = 0; i < (byte_range + (BLOCK_SIZE - 1)); ++i) {
    Word word = { .word = (int)address};
    return word;
}

void writeWord(unsigned int address, Word word) {
    if (validation((int)address) == 0) {
        printf("Address not 4 byte aligned or in valid memory range!");
    }
}
