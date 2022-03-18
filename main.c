#include <stdio.h>
#include <stdlib.h>

#include "cart.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("[!] Usage: ./%s <rom>\n", argv[0]);
        return 0;
    }

    Cart_Header *ch = load_cart_header(argv[1]);

    printf("[*] %s has been loaded!\n", ch->title);

    return 0;
}
