//
// Created by jacel on 2/18/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cache.h"

// Initialize Structs
CacheBlock cacheBlock = { .valid = 0, .dirty = 0, .tag = 0, .cacheBlock = {} };
CacheSet cacheSet = { .tagQueue = {}, .cacheBlocks = {} };
Cache cache = { .cacheSets = {}, .write = WRITE_BACK };

int main() {
    initialize();
    output();
    Word word = {};
    //  TESTING FOR testAwb.out / testAwt.out
    printf("Several addresses that map to the same set:");
    readWord(1152);
    readWord(2176);
    readWord(3200);
    readWord(4224);
    readWord(5248);
    readWord(7296);
    readWord(4224);
    readWord(3200);

    word.word = 17;
    writeWord(7312, word);

    readWord(7320);
    readWord(4228);
    readWord(3212);

    word.word = 5;
    writeWord(5248, word);

    readWord(5248);

    word.word = 7;
    writeWord(8320, word);

    readWord(9344);
    readWord(11392);
    readWord(16512);
    readWord(17536);
    readWord(8320);
    readWord(17536);
    readWord(17532);

    // TESTING FOR testBwb.out / testBwt.out
//    readWord(46916);
//    readWord(46932);
//    readWord(12936);
//    readWord(13964);
//
//    word.word = 40;
//    writeWord(46956, word);
//
//    readWord(46956);
//    readWord(56132);

    // TESTING FOR testCwb.out / testCwt.out
//    readWord(0);
//    readWord(13432);
//    readWord(13388);
//    readWord(62540);
//    readWord(4);
//    readWord(16452);
//    readWord(32900);
//    readWord(49348);
//    readWord(260);
//    readWord(16708);
//    readWord(33156);
//    readWord(49604);
//    readWord(516);

    return 0;
}

void initialize() {
    int size = sizeof memory / sizeof memory[0];
    int i, j;
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
    // Print Memory Array
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
    for (i = 0; i < ((CACHE_SIZE / BLOCK_SIZE) / ((CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY)); ++i) {
        cacheSet.tagQueue[i] = -1;
    }
    // # of sets
    for (i = 0; i < ((CACHE_SIZE / BLOCK_SIZE) / ((CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY)); ++i) {
        cache.cacheSets[i] = cacheSet;
    }
    // Initialize all tagQueues to -1
    for (i = 0; i < ((CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY); ++i) {
        for (j = 0; j < ((CACHE_SIZE / BLOCK_SIZE) / ((CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY)); ++j) {
            cache.cacheSets[i].tagQueue[j] = -1;
        }
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
    // Get binary signed 2's for address
    char bistr[1000];
    itoa(n, bistr, 2);
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
    int byte_range = memory_block * BLOCK_SIZE;
    float tagLength = (CACHE_SIZE / BLOCK_SIZE) - (log2((CACHE_SIZE / BLOCK_SIZE) / ASSOCIATIVITY)) - (log2(BLOCK_SIZE));
    int slot = ((int)address / BLOCK_SIZE) % ((CACHE_SIZE / BLOCK_SIZE) / ASSOCIATIVITY);
    int tag = ((int)address / BLOCK_SIZE) / ((CACHE_SIZE / BLOCK_SIZE) / ASSOCIATIVITY);
    int blockIndex = -1;
    int evictedTag = -1;
    int replace = 0;
    int replacedByteRange = 0;
    int i;

    // Update Tag Queue
    int tagAdded = 0;
    int readHit = 0;
    for (i = 0; i < ((CACHE_SIZE / BLOCK_SIZE) / ((CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY)); ++i) {
        // Check for read hit
        if(cache.cacheSets[slot].tagQueue[i] == tag) {
            readHit = 1;
            blockIndex = i;
            break;
        }
        if (cache.cacheSets[slot].tagQueue[i] == -1) {
            cache.cacheSets[slot].tagQueue[i] = tag;
            tagAdded = 1;
            blockIndex = i;
            break;
        }
    }

    // Evicting Tag
    if (tagAdded == 0 && readHit == 0) {
        if (lastReplacedIndex == -1) {
            lastReplacedIndex = 0;
        } else {
            if (lastReplacedIndex == ((CACHE_SIZE / BLOCK_SIZE) / ((CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY)) - 1) {
                lastReplacedIndex = 0;
            } else {
                ++lastReplacedIndex;
            }
        }
        blockIndex = lastReplacedIndex;
        replacedByteRange = cache.cacheSets[slot].cacheBlocks->cacheBlock[0];
        evictedTag = cache.cacheSets[slot].tagQueue[lastReplacedIndex];
        cache.cacheSets[slot].tagQueue[lastReplacedIndex] = tag;
        replace = 1;
    }

    // Move from memory on read miss
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

    // Get word
    Word word = { .word = 0};
    if (cache.cacheSets[slot].cacheBlocks->dirty == 1 && cache.cacheSets[slot].cacheBlocks->cacheBlock[address + 1] != 0) {
        word.word = cache.cacheSets[slot].cacheBlocks->cacheBlock[(int) address + 1];
    } else {
        word.word = (int) address;
    }

    // OUTPUT
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
    printf(" word=%d", word.word);
    printf(" (%d", byte_range);
    printf(" - %d)]", (byte_range + (BLOCK_SIZE - 1)));

    // Print tag evict message
    if (tagAdded == 0 && readHit == 0) {
        printf("\n[Evict tag %d", evictedTag);
        printf(", in blackIndex %d]", blockIndex);
    }

    // Write Back Message
    if (replace == 1 && WRITE_BACK == 0) {
        printf("\n[write back (%d", replacedByteRange);
        printf(" - %d)]", (replacedByteRange + (BLOCK_SIZE - 1)));
    }

    // Print tagQueue
    printf("\n[");
    for (i = ((CACHE_SIZE / BLOCK_SIZE) / ((CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY)) - 1; i >= 0; --i) {
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

    int memoryBlock = (int)address / BLOCK_SIZE;
    int byteRange = memoryBlock * BLOCK_SIZE;
    float tagLength = (CACHE_SIZE / BLOCK_SIZE) - (log2((CACHE_SIZE / BLOCK_SIZE) / ASSOCIATIVITY)) - (log2(BLOCK_SIZE));
    int slot = ((int)address / BLOCK_SIZE) % ((CACHE_SIZE / BLOCK_SIZE) / ASSOCIATIVITY);
    int tag = ((int)address / BLOCK_SIZE) / ((CACHE_SIZE / BLOCK_SIZE) / ASSOCIATIVITY);
    int blockIndex = -1;
    int evictedTag = -1;
    int replace = 0;
    int replacedByteRange = 0;
    int i;

    // Update Tag Queue
    int tagAdded = 0;
    int writeHit = 0;
    for (i = 0; i < ((CACHE_SIZE / BLOCK_SIZE) / ((CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY)); ++i) {
        // Check for read hit
        if(cache.cacheSets[slot].tagQueue[i] == tag) {
            writeHit = 1;
            blockIndex = i;
            break;
        }
        if (cache.cacheSets[slot].tagQueue[i] == -1) {
            cache.cacheSets[slot].tagQueue[i] = tag;
            tagAdded = 1;
            blockIndex = i;
            break;
        }
    }

    // Evict Tag
    if (tagAdded == 0 && writeHit == 0) {
        if (lastReplacedIndex == -1) {
            lastReplacedIndex = 0;
        } else {
            if (lastReplacedIndex == ((CACHE_SIZE / BLOCK_SIZE) / ((CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY)) - 1) {
                lastReplacedIndex = 0;
            } else {
                ++lastReplacedIndex;
            }
        }
        blockIndex = lastReplacedIndex;
        replacedByteRange = cache.cacheSets[slot].cacheBlocks->cacheBlock[0];
        evictedTag = cache.cacheSets[slot].tagQueue[lastReplacedIndex];
        cache.cacheSets[slot].tagQueue[lastReplacedIndex] = tag;
        replace = 1;
    }

    // Move from memory on write miss
    if (writeHit == 0) {
        // Move 64 byte from memory to cache block
        for (i = byteRange; i < (byteRange + (BLOCK_SIZE - 1)); ++i) {
            cache.cacheSets[slot].cacheBlocks->cacheBlock[i - byteRange] = memory[i];
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
    printf(" (%d", byteRange);
    printf(" - %d)]", (byteRange + (BLOCK_SIZE - 1)));

    // Print tag evict message
    if (tagAdded == 0 && writeHit == 0) {
        printf("\n[Evict tag %d", evictedTag);
        printf(", in blackIndex %d]", blockIndex);
    }

    // Write Back Message
    if (replace == 1 && WRITE_BACK == 0) {
        printf("\n[write back (%d", replacedByteRange);
        printf(" - %d)]", (replacedByteRange + (BLOCK_SIZE - 1)));
    }

    // Print tagQueue
    printf("\n[");
    for (i = ((CACHE_SIZE / BLOCK_SIZE) / ((CACHE_SIZE/BLOCK_SIZE) / ASSOCIATIVITY)) - 1; i >= 0; --i) {
        printf(" %d", cache.cacheSets[slot].tagQueue[i]);
    }
    printf(" ]");
    printf("\n");
}
