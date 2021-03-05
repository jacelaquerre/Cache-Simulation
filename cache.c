//
// Created by jacel on 2/18/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "cache.h"

// Initialize Structs
CacheBlock cacheBlock = { .valid = 0, .dirty = 0, .tag = 0, .cacheBlock = {} };
CacheSet cacheSet = { .tagQueue = {}, .cacheBlocks = {} };
Cache cache = { .cacheSets = {}, .write = WRITE_BACK };

int main() {
    initialize();
    output();

    printf("Several addresses that map to the same set:");
    readWord(1152);
    readWord(2176);
    readWord(3200);
    readWord(4224);
    readWord(5248);
    readWord(7296);
    readWord(4224);
    readWord(3200);

    Word word = { .word = 17};
    writeWord(7312, word);

    readWord(7320);
    readWord(4228);
    readWord(3212);

    word.word = 5;
    writeWord(5248, word);

    readWord(5248);

    word.word = 7;
    writeWord(8320, word);

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
    // Initialize Structs
    // # of blocks
    for (i = 0; i < (CACHE_SIZE / BLOCK_SIZE); ++i) {
        cacheSet.cacheBlocks[i] = cacheBlock;
    }
    for (i = 0; i < (CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY; ++i) {
        cacheSet.tagQueue[i] = -1;
    }
    // # of sets
    for (i = 0; i < ((CACHE_SIZE / BLOCK_SIZE) / ((CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY)); ++i) {
        cache.cacheSets[i] = cacheSet;
    }

}

void output() {
    printf("-----------------------------------\n");
    printf("Cache Size = %d\n", CACHE_SIZE);
    printf("Block Size = %d\n", BLOCK_SIZE);
    printf("#Blocks = %d\n", CACHE_SIZE / BLOCK_SIZE);
    printf("#Sets = %d\n", (CACHE_SIZE / BLOCK_SIZE) / ASSOCIATIVITY);
    printf("Associativity = %d\n", ASSOCIATIVITY);
    printf("Tag Length = %.0f\n", (CACHE_SIZE / BLOCK_SIZE) - (log2((CACHE_SIZE / BLOCK_SIZE) / ASSOCIATIVITY)) - (log2(BLOCK_SIZE)));
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
    //    CacheBlock cacheBlock = { .valid = 1, .dirty = 0, .tag = 0, .cacheBlock = {} };
    //    CacheSet cacheSet = { .tagQueue = {}, .cacheBlocks = {} };
    //    Cache cache = { .cacheSets = {}, .write = WRITE_BACK };
    int memory_block = (int)address / BLOCK_SIZE;
    int byte_range = memory_block * BLOCK_SIZE;
    float tagLength = (CACHE_SIZE / BLOCK_SIZE) - (log2((CACHE_SIZE / BLOCK_SIZE) / ASSOCIATIVITY)) - (log2(BLOCK_SIZE));
    int slot = ((int)address / BLOCK_SIZE) % ((CACHE_SIZE / BLOCK_SIZE) / ASSOCIATIVITY);
    int tag = ((int)address / BLOCK_SIZE) / ((CACHE_SIZE / BLOCK_SIZE) / ASSOCIATIVITY);
    int blockIndex = -1;
    int evictedTag = -1;
    int i;

    // Update Tag Queue
    int tagAdded = 0;
    int readHit = 0;
    for (i = 0; i < (CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY; ++i) {
        // Check for read hit
        if(cache.cacheSets[slot].tagQueue[i] == tag) {
            readHit = 1;
            break;
        }
        if (cache.cacheSets[slot].tagQueue[i] == -1) {
            cache.cacheSets[slot].tagQueue[i] = tag;
            tagAdded = 1;
            blockIndex = i;
            break;
        }
    }
    // LRU
    if (tagAdded == 0 && readHit == 0) {
        if (lastReplacedIndex == -1) {
            lastReplacedIndex = 0;
        } else {
            ++lastReplacedIndex;
        }
        evictedTag = cache.cacheSets[slot].tagQueue[lastReplacedIndex];
        blockIndex = lastReplacedIndex;
        cache.cacheSets[slot].tagQueue[lastReplacedIndex] = tag;
    }

    if (readHit == 0) {
        // Move 64 byte from memory to cache block
        for (i = byte_range; i < (byte_range + (BLOCK_SIZE - 1)); ++i) {
            cache.cacheSets[slot].cacheBlocks->cacheBlock[i - byte_range] = memory[i];
        }
        // Update tag in block
        cache.cacheSets[slot].cacheBlocks->tag = tag;
        // Update valid block
        cache.cacheSets[slot].cacheBlocks->valid = 1;
    }
    Word word = { .word = 0};
    // Get word
    if (cache.cacheSets[slot].cacheBlocks->dirty == 1) {
        word.word = cache.cacheSets[slot].cacheBlocks->cacheBlock[(int) address + 1];
    } else {
        word.word = (int) address;
    }
    // OUTPUT
//    printf("memory_block %d\n", memory_block);
//    printf("byte_range %d\n", byte_range);
//    printf("tegLength %d\n", (int) tagLength);
//    printf("slot %d\n", slot);
//    printf("tag %d\n", tag);
    printf("\n(R) address is: %d ", (int)address);
    displayBinary((int)address);
    printf("\n[addr=%d", (int)address);
    printf(" index=%d", slot);
    printf(" blockIndex=%d", blockIndex);
    printf(" tag=%d", tag);
    if (tagAdded == 1) {
        printf(" read miss;");
    } else if (readHit == 1) {
        printf(" read hit;");
    } else {
        printf(" read miss + replace;");
    }
    // read miss etc
    printf(" word=%d", word.word);
    printf(" (%d", byte_range);
    printf(" - %d)]", (byte_range + (BLOCK_SIZE - 1)));

    // Print tag evict message
    if (tagAdded == 0 && readHit == 0) {
        printf("\nEvict tag %d", evictedTag);
        printf(", in blackIndex %d", blockIndex);
    }

    // Print tagQueue
    printf("\n[");
    for (i = 0; i < ((CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY); ++i) {
        printf(" %d", cache.cacheSets[slot].tagQueue[i]);
    }
    printf(" ]");
    printf("\n");

    return word;
}

void writeWord(unsigned int address, Word word) {
    if (validation((int)address) == 0) {
        printf("Address not 4 byte aligned or in valid memory range!");
    }
    //    CacheBlock cacheBlock = { .valid = 1, .dirty = 0, .tag = 0, .cacheBlock = {} };
    //    CacheSet cacheSet = { .tagQueue = {}, .cacheBlocks = {} };
    //    Cache cache = { .cacheSets = {}, .write = WRITE_BACK };
    int memory_block = (int)address / BLOCK_SIZE;
    int byte_range = memory_block * BLOCK_SIZE;
    float tagLength = (CACHE_SIZE / BLOCK_SIZE) - (log2((CACHE_SIZE / BLOCK_SIZE) / ASSOCIATIVITY)) - (log2(BLOCK_SIZE));
    int slot = ((int)address / BLOCK_SIZE) % ((CACHE_SIZE / BLOCK_SIZE) / ASSOCIATIVITY);
    int tag = ((int)address / BLOCK_SIZE) / ((CACHE_SIZE / BLOCK_SIZE) / ASSOCIATIVITY);
    int blockIndex = -1;
    int evictedTag = -1;
    int i;

    // Update Tag Queue
    int tagAdded = 0;
    int writeHit = 0;
    for (i = 0; i < (CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY; ++i) {
        // Check for read hit
        if(cache.cacheSets[slot].tagQueue[i] == tag) {
            writeHit = 1;
            break;
        }
        if (cache.cacheSets[slot].tagQueue[i] == -1) {
            cache.cacheSets[slot].tagQueue[i] = tag;
            tagAdded = 1;
            blockIndex = i;
            break;
        }
    }
    // LRU
    if (tagAdded == 0 && writeHit == 0) {
        if (lastReplacedIndex == -1) {
            lastReplacedIndex = 0;
        } else {
            ++lastReplacedIndex;
        }
        evictedTag = cache.cacheSets[slot].tagQueue[lastReplacedIndex];
        blockIndex = lastReplacedIndex;
        cache.cacheSets[slot].tagQueue[lastReplacedIndex] = tag;
    }

    // Move from memory on write miss
    if (writeHit == 0) {
        // Move 64 byte from memory to cache block
        for (i = byte_range; i < (byte_range + (BLOCK_SIZE - 1)); ++i) {
            cache.cacheSets[slot].cacheBlocks->cacheBlock[i - byte_range] = memory[i];
        }
        // Update tag in block
        cache.cacheSets[slot].cacheBlocks->tag = tag;
        // Update valid block
        cache.cacheSets[slot].cacheBlocks->valid = 1;
    }
    // Write Word
    cache.cacheSets[slot].cacheBlocks->cacheBlock[address + 1] = word.word;
    // Make it dirty
    cache.cacheSets[slot].cacheBlocks->dirty = 1;

    // OUTPUT
//    printf("memory_block %d\n", memory_block);
//    printf("byte_range %d\n", byte_range);
//    printf("tegLength %d\n", (int) tagLength);
//    printf("slot %d\n", slot);
//    printf("tag %d\n", tag);
    printf("\n(W) address is: %d ", (int)address);
    displayBinary((int)address);
    printf("\n[addr=%d", (int)address);
    printf(" index=%d", slot);
    printf(" blockIndex=%d", blockIndex);
    printf(" tag=%d", tag);
    if (tagAdded == 1) {
        printf(" write miss;");
    } else if (writeHit == 1) {
        printf(" write hit;");
    } else {
        printf(" write miss + replace;");
    }
    // read miss etc
    printf(" word=%d", word.word);
    printf(" (%d", byte_range);
    printf(" - %d)]", (byte_range + (BLOCK_SIZE - 1)));

    // Print tag evict message
    if (tagAdded == 0 && writeHit == 0) {
        printf("\nEvict tag %d", evictedTag);
        printf(", in blackIndex %d", blockIndex);
    }

    // Print tagQueue
    printf("\n[");
    for (i = 0; i < ((CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY); ++i) {
        printf(" %d", cache.cacheSets[slot].tagQueue[i]);
    }
    printf(" ]");
    printf("\n");
}
