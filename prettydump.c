#ifndef PRETTYDUMP__
#define PRETTYDUMP__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define PRD_NONE 0

#define PRD_BINARY 1
#define PRD_HEX 2
#define PRD_ASCII 4

#define PRD_NO_ADDRESS 1
#define PRD_FIXED_ADDRESS 2
#define PRD_RAW_ADDRESS 4
#define PRD_OFFSET 8

#define PRD_ONLY_OFFSET (PRD_NO_ADDRESS | PRD_OFFSET)
#define PRD_ADDRESS_AND_OFFSET (PRD_FIXED_ADDRESS | PRD_OFFSET)

// fine enough
int __numlen(int n) {
    if(n < 10) return 1;
    if(n < 100) return 2;
    if(n < 1000) return 3;
    if(n < 10000) return 4;
    if(n < 100000) return 5;
    if(n < 1000000) return 6;
    return 0;
}

// TODO: add support for fprintf and sprintf/snprintf (somehow)
int prdump(void *_ptr, size_t len, uint8_t datafmt, uint8_t rowfmt, int grp, int grprow, int grpgap) {

    size_t plen = len;
    int8_t *ptr = (int8_t *)_ptr;

    int bytesPerRow = grp * grprow;
    int offsetPerRow = len / bytesPerRow;

    for(int i = 0; i < 10; i++) { putchar('-'); }
    putchar('\n');

    int offset = 0;
    while(len) {
        printf("| ");

        if(rowfmt) {
            printf("[ ");

            if(rowfmt & PRD_NO_ADDRESS) putchar('@');
            else if(rowfmt & PRD_FIXED_ADDRESS) printf("%p", _ptr);
            else if(rowfmt & PRD_RAW_ADDRESS) printf("%p", ptr);

            if(rowfmt & PRD_OFFSET) {
                if(rowfmt & PRD_RAW_ADDRESS) printf(" / +"); else printf(" + ");
                printf("%d", offset);
                int extraSpaces = __numlen(plen) - __numlen(offset);
                for(int i = 0; i < extraSpaces; i++) { putchar(' '); }
            }

            printf(" ] | ");
        }
        offset += bytesPerRow;

        for(int i = 0; i < grprow; i++) {
            if(i) {
                for(int i = 0; i < grpgap - 1; i++) putchar(' ');
            }
            for(int i = 0; i < grp; i++) {
                if(!len) { goto exitloop; }

                int8_t byte = *ptr;
                ptr++; len--;

                if(datafmt == PRD_BINARY) {
                    for(int i = 0; i < 8; i++) {
                        putchar(((byte >> (8 - 1 - i)) & 1) + '0');
                    }
                }
                else if(datafmt == PRD_HEX) {
                    char lo = byte & 0x0F;
                    char hi = (byte & 0xF0) >> 4;

                    if(hi < 10) putchar(hi + '0'); else putchar(hi - 10 + 'a');
                    if(lo < 10) putchar(lo + '0'); else putchar(lo - 10 + 'a');
                }
                else if(datafmt == PRD_ASCII) {
                    bool normal = false;

                    char buf[4];

                         if(byte == '\0') sprintf(buf, "\\0");
                    else if(byte == '\n') sprintf(buf, "\\n");
                    else if(byte == '\r') sprintf(buf, "\\r");
                    else if(byte == '\a') sprintf(buf, "\\a");
                    else if(byte == '\t') sprintf(buf, "\\t");
                    else { sprintf(buf, "%c", byte); normal = true; }

                    if(normal) putchar(' ');
                    printf("%s", buf);
                }
            }
            putchar(' ');
        }

exitloop:
        printf("|\n");
    }
}

#else
#endif
