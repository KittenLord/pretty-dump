#include "prettydump.c"
#include <stdio.h>
#include <stdlib.h>

// TODO: Actually accept arguments lol
int main() {
    size_t length = 0;
    size_t capacity = 100;
    char *input = malloc(capacity);

    char c;
    while((c = getchar()) != EOF) {
        input[length++] = c;
        if(length == capacity) {
            capacity *= 2;
            input = realloc(input, capacity);
        }
    }

    prdump(input, length, PRD_ASCII, PRD_ONLY_OFFSET, 1, 8, 2);

    return 0;
}
