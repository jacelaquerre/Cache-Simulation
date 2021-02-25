//
// Created by jacel on 2/18/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

char memory[1024];
void initialize();

int main() {
    initialize();
    return 0;
}

void initialize() {
    int i;
    int ct = 49;
    for (i = 0; i < sizeof(memory); ++i) {
        if (!(i & 3) || i == 0) {
            //++ct;
            memory[i] = ct;
        } else {
            memory[i] = '0';
        }
    }

    printf(memory);
}
