//
// Created by jacel on 2/18/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int memory[1024];
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
    for(i = 0; i < size; ++i) {
        if (!(i & 3) && i != 0) {
            printf(" ");
        }
        printf("%d", memory[i]);
    }
}

Word readWord(unsigned int address) {

}

void writeWord(unsigned int address, Word word) {

}
