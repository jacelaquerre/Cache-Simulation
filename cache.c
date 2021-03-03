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
    readWord(2460);
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
    printf("Tag Length = %d\n", (CACHE_SIZE/BLOCK_SIZE) - ((CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY) - ASSOCIATIVITY);
    if (WRITE_BACK == 0) {
        printf("Write Back\n");
    } else {
        printf("Write Through\n");
    }
    printf("-----------------------------------\n");
}

char * binaryConverter(int address) {
    char *binary = (char *)malloc(sizeof(char) * 81);
    int i;
    int count = 0;
    // Find how many digits in #
    do {
        ++count;
        address /= 10;
    } while(address != 0);

    char addressArr[count];
    strncpy(addressArr, toArray(address), count);
    for(i = 0; i < strlen(addressArr); ++i){
        switch(addressArr[i]){
            case '0': strcat(binary, "0000"); break;
            case '1': strcat(binary, "0001"); break;
            case '2': strcat(binary, "0010"); break;
            case '3': strcat(binary, "0011"); break;
            case '4': strcat(binary, "0100"); break;
            case '5': strcat(binary, "0101"); break;
            case '6': strcat(binary, "0110"); break;
            case '7': strcat(binary, "0111"); break;
            case '8': strcat(binary, "1000"); break;
            case '9': strcat(binary, "1001"); break;
        }
    }
    return binary;
}

// Turn into address into array of its digits
char * toArray(int number) {
    unsigned int length = (int)(log10f((float)number)) + 1;
    char * arr = (char *) malloc(length * sizeof(char)), * curr = arr;
    do {
        *curr++ = number % 10;
        number /= 10;
    } while (number != 0);
    return arr;
}

int validation(int address) {
    // 0 = false, 1 = true
    int valid = 0;
    // check 4 byte alignment
    if (address >= 0 && address < MEMORY_SIZE) {
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
    printf("%d\n", memory_block);
    int byte_range = memory_block * BLOCK_SIZE;
    printf("%d\n", byte_range);
    int tagLength = ((CACHE_SIZE/BLOCK_SIZE) - ((CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY) - ASSOCIATIVITY);
    int slot = ((int)address / BLOCK_SIZE) % tagLength;
    printf("%d\n", slot);
    int tag = ((int)address / BLOCK_SIZE) / tagLength;
    printf("%d\n", tag);
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
