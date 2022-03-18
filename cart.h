#include "common.h"

typedef struct {
    u8 entry_point[4];
    u8 nintendo_logo[48];
    u8 title[16];
    u8 manufacturer_code[4];
    u8 cgb_flag;
    u16 new_licensee_code;
    u8 sgb_flag;
    u8 cart_type;
    u8 rom_size;
    u8 ram_size;
    u8 destination_code;
    u8 old_licensee_code;
    u8 mask_rom_vn;
    u8 header_checksum;
    u16 global_checksum;
} Cart_Header;

Cart_Header *load_cart_header(const char *name);
