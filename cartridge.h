#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "util.h"

typedef struct Rom_Header {
    u8 entry_point[0x4];
    u8 nintendo_logo[0x30];
    char title[0x10];
    u16 new_lic_code;
    u8 sgb_flag;
    u8 cart_type;
    u8 rom_size;
    u8 ram_size;
    u8 dest_code;
    u8 old_lic_code;
    u8 mask_rom_vn;
    u8 checksum;
    u16 global_checksum;
} Rom_Header;

typedef struct Cartridge {
    char filename[1024];
    u32 rom_size;
    u8 *rom_data;
    Rom_Header *header;
} Cartridge;

void load_cart(const char *cart_name);
void print_cart_info(Cartridge cart);

#endif
