#include <stdlib.h>
#include <stdio.h>
#include "cartridge.h"

Cartridge loaded_game;

void load_cart(const char *cart_name) {
    FILE *fp = fopen(cart_name, "r");
    if (!fp) {
        printf("Can't open ROM.\n");
        exit(-1);
    }

    // Get the rom size
    fseek(fp, 0, SEEK_END);  
    loaded_game.rom_size = ftell(fp);
    rewind(fp);

    // Allocate memory for ROM
    loaded_game.rom_data = malloc(loaded_game.rom_size);
    if (!loaded_game.rom_data) {
        printf("Can't allocate memory for ROM.\n");
        exit(-1);
    }
    fread(loaded_game.rom_data, loaded_game.rom_size, 1, fp);
    fclose(fp);

    // Get a pointer to the cartridge's header
    loaded_game.header = (Rom_Header *) (loaded_game.rom_data + 0x100);
    loaded_game.header->title[15] = 0; // Make sure title is a NULL terminated string
}

void print_cart_info(Cartridge cart) {
    printf("Game title: %s\n", cart.header->title); 
}
