#include <stdio.h>
#include <stdlib.h>

#include "cart.h"

Cart_Header *load_cart_header(const char *name) {
    // Open file
    FILE *romf = fopen(name, "r");
    if (romf == NULL) {
        fprintf(stderr, "[!] Cannot open file '%s'\n", name);
        exit(1);
    }

    // Get file size
    fseek(romf, 0, SEEK_END);
    size_t rom_size = ftell(romf);
    rewind(romf);

    // Read file's content
    u8 *rom = malloc(rom_size);
    if (!fread(rom, rom_size, 1, romf)) {
        fprintf(stderr, "[!] Cannot read file\n");
        exit(1);
    }
    
    // Setup header
    Cart_Header *header = (Cart_Header *) (rom + 0x100);
    header->title[15] = 0; // Zero terminate ROM's title

    return header;
}
