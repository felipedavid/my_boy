#include <stdio.h>

#include "cartridge.h"

extern Cartridge loaded_game;

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: ./my_boy <rom_file>\n");
        return -1;
    }

    load_cart(argv[1]);

    print_cart_info(loaded_game);
}
