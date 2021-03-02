//
// Created by jacel on 2/18/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

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
// Teg Length
#define TAG_LENGTH 8

int memory[MEMORY_SIZE];

typedef struct Word {
    int word[4];
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
    int tagQueue[(CACHE_SIZE/4) / (4 * ASSOCIATIVITY)];
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
char * binaryConverter(int address);
char * toArray(int number);


int main() {
//    CacheBlock cacheBlock = { .write = WRITE_BACK, .cache = 123 };
//    CacheSet cacheSet = { .tagQueue = [(CACHE_SIZE/BLOCK_SIZE) * ASSOCIATIVITY], .cache = 123 };
//    Cache cache = { .write = WRITE_BACK, .cache = cacheSet };
    initialize();
    output();
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
    printf("#Sets = %d\n", (CACHE_SIZE/4) / (4 * ASSOCIATIVITY));
    printf("Associativity = %d\n", ASSOCIATIVITY);
    printf("Tag Length = %d\n", TAG_LENGTH);
    if (WRITE_BACK == 0) {
        printf("Write Back\n");
    } else {
        printf("Write Through\n");
    }
    printf("-----------------------------------");
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
