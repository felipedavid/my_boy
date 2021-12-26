#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;

void *xmalloc(size_t n_bytes) {
    void *ptr = malloc(n_bytes);
    if (!ptr) {
        fprintf(stderr, "[!] malloc failed.\n");
        exit(1);
    }
    return ptr;
}

typedef struct {
    u8 entry_point[4];
    u8 nintendo_logo[48];
    u8 title[16];
    u8 license_code[2];
    u8 sgb_flag;
    u8 cartridge_type;
    u8 rom_size;
    u8 ram_type;
    u8 destination_code;
    u8 old_license_code;
    u8 mask_rom_v_n;
    u8 checksum;
    u8 global_checksum;
} Cartridge_Header;

typedef struct {
    u8 *rom_data;
    Cartridge_Header *header;
} Cartridge;

Cartridge cart;

void load_cartridge(const char *cartridge_name) {
    // Open file
    FILE *f = fopen(cartridge_name, "r");
    if (f == NULL) {
        fprintf(stderr, "[!] Can't open rom file \"%s\"\n", cartridge_name);
        exit(1);
    }

    // Get file size
    fseek(f, 0L, SEEK_END);
    size_t f_size = ftell(f);
    rewind(f);

    // Read file's content
    cart.rom_data = xmalloc(f_size);
    if (fread(cart.rom_data, f_size, 1, f) < 1) {
        fprintf(stderr, "[!] Can't read file's contents.\n");
        exit(1);
    }
    cart.header = (Cartridge_Header *) (cart.rom_data + 0x100);
    cart.rom_data[0x143] = '\0'; // Zero terminate the ROM's title

    printf("Name: %s\n", cart.header->title);
    printf("Checksum Test: ");

    // Check if ROM is valid
    u16 x = 0;
    for (u16 i = 0x134; i <= 0x14C; x = x - cart.rom_data[i++] - 1);
    if (cart.header->checksum == (x & 0xFF)) {
        printf("PASSED!\n");
    } else {
        printf("NOT PASSED!\n");
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "[!] Usage: %s <rom>\n", argv[0]);
        return 1;
    }

    load_cartridge(argv[1]);
}
