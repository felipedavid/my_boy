#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "common.h"

void *xmalloc(size_t n_bytes) {
    void *ptr = malloc(n_bytes);
    if (ptr == NULL) {
        fprintf(stderr, "[!] Unable to allocate memory.\n");
        exit(1);
    }
    return ptr;
}

typedef struct {
    u8 entry_point[4];
    u8 nintendo_logo[48];
    char title[16];
    u16 new_license_code;
    u8 sgb_flag;
    u8 type;
    u8 rom_size;
    u8 ram_size;
    u8 destination_code;
    u8 old_license_code;
    u8 version;
    u8 checksum;
    u16 global_checksum;
} Cartridge_Header;

typedef struct {
    size_t rom_size;
    u8 *rom_data;
    Cartridge_Header *header;
} Cartridge;

Cartridge cart;

const char *cart_type_string() {
    switch (cart.header->type) {
    case 0x00: return "ROM ONLY";
    case 0x01: return "MBC1";
    case 0x02: return "MBC1+RAM";
    case 0x03: return "MBC1+RAM+BATTERY";
    case 0x05: return "MBC2";
    case 0x06: return "MBC2+BATTERY";
    case 0x08: return "ROM+RAM 1";
    case 0x09: return "ROM+RAM+BATTERY 1";
    case 0x0B: return "MMM01";
    case 0x0C: return "MMM01+RAM";
    case 0x0D: return "MMM01+RAM+BATTERY";
    case 0x0F: return "MBC3+TIMER+BATTERY";
    case 0x10: return "MBC3+TIMER+RAM+BATTERY 2";
    case 0x11: return "MBC3";
    case 0x12: return "MBC3+RAM 2";
    case 0x13: return "MBC3+RAM+BATTERY 2";
    case 0x19: return "MBC5";
    case 0x1A: return "MBC5+RAM";
    case 0x1B: return "MBC5+RAM+BATTERY";
    case 0x1C: return "MBC5+RUMBLE";
    case 0x1D: return "MBC5+RUMBLE+RAM";
    case 0x1E: return "MBC5+RUMBLE+RAM+BATTERY";
    case 0x20: return "MBC6";
    case 0x22: return "MBC7+SENSOR+RUMBLE+RAM+BATTERY";
    case 0xFC: return "POCKET CAMERA";
    case 0xFD: return "BANDAI TAMA5";
    case 0xFE: return "HuC3";
    case 0xFF: return "HuC1+RAM+BATTERY";
    default: return "UNKNOWN";
    }
}

const char *cart_license_code_string() {
    switch (cart.header->old_license_code) {
	case 0x00: return "None";
	case 0x01: return "Nintendo R&D1";
	case 0x08: return "Capcom";
	case 0x13: return "Electronic Arts";
	case 0x18: return "Hudson Soft";
	case 0x19: return "b-ai";
	case 0x20: return "kss";
	case 0x22: return "pow";
	case 0x24: return "PCM Complete";
	case 0x25: return "san-x";
	case 0x28: return "Kemco Japan";
	case 0x29: return "seta";
	case 0x30: return "Viacom";
	case 0x31: return "Nintendo";
	case 0x32: return "Bandai";
	case 0x33: return "Ocean/Acclaim";
	case 0x34: return "Konami";
	case 0x35: return "Hector";
	case 0x37: return "Taito";
	case 0x38: return "Hudson";
	case 0x39: return "Banpresto";
	case 0x41: return "Ubi Soft";
	case 0x42: return "Atlus";
	case 0x44: return "Malibu";
	case 0x46: return "angel";
	case 0x47: return "Bullet-Proof";
	case 0x49: return "irem";
	case 0x50: return "Absolute";
	case 0x51: return "Acclaim";
	case 0x52: return "Activision";
	case 0x53: return "American sammy";
	case 0x54: return "Konami";
	case 0x55: return "Hi tech entertainment";
	case 0x56: return "LJN";
	case 0x57: return "Matchbox";
	case 0x58: return "Mattel";
	case 0x59: return "Milton Bradley";
	case 0x60: return "Titus";
	case 0x61: return "Virgin";
	case 0x64: return "LucasArts";
	case 0x67: return "Ocean";
	case 0x69: return "Electronic Arts";
	case 0x70: return "Infogrames";
	case 0x71: return "Interplay";
	case 0x72: return "Broderbund";
	case 0x73: return "sculptured";
	case 0x75: return "sci";
	case 0x78: return "THQ";
	case 0x79: return "Accolade";
	case 0x80: return "misawa";
	case 0x83: return "lozc";
	case 0x86: return "Tokuma Shoten Intermedia";
	case 0x87: return "Tsukuda Original";
	case 0x91: return "Chunsoft";
	case 0x92: return "Video system";
	case 0x93: return "Ocean/Acclaim";
	case 0x95: return "Varie";
	case 0x96: return "Yonezawa/s’pal";
	case 0x97: return "Kaneko";
	case 0x99: return "Pack in soft";
	case 0xA4: return "Konami (Yu-Gi-Oh!)";
	default: return "Unknown";
    }
}

void log_cartridge_info() {
    printf("Title: %s\n", cart.header->title);
    printf("Type: %2.2x (%s)\n", cart.header->type, cart_type_string());
    printf("ROM Size: %d KB\n", 32 << cart.header->rom_size);
    printf("RAM Size: %2.2x\n", cart.header->ram_size);
    printf("License Code: %2.2x (%s)\n", cart.header->old_license_code, cart_license_code_string());
    printf("ROM Version: %2.2x\n\n", cart.header->version);
}

bool load_cartridge(const char *filename) {
    // Open file
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "[!] Unable to open file \"%s\".", filename);
        return false;
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    cart.rom_size = ftell(fp);
    rewind(fp);

    // Read ROM data and setup struct
    cart.rom_data = xmalloc(cart.rom_size);
    fread(cart.rom_data, cart.rom_size, 1, fp);
    fclose(fp);

    cart.header = (Cartridge_Header *) (cart.rom_data + 0x100);
    cart.header->title[15] = 0; // Null terminate game's title

    // Validating checksum
    u16 x = 0;
    for (int i = 0x134; i <= 0x14C; i++) {
        x = x - cart.rom_data[i] - 1;
    }
    if ((x & 0xFF) != cart.header->checksum) {
        fprintf(stderr, "[!] The checksum dosen't match. Expected %2.2x, got %2.2x\n", 
            cart.header->checksum, x);
        //return false;
    }

    printf("[*] Cartridge loaded.\n");
    log_cartridge_info();
    return true;
}

u8 mbus_read(u16 addr) {
    if (addr < 0x8000) {
        return cart.rom_data[addr];
    }
}

typedef struct {
    union { 
        u16 af;
        struct { u8 a; u8 f; };
    };
    union {
        u16 bc;
        struct { u8 b; u8 c; };
    };
    union {
        u16 de;
        struct { u8 d; u8 e; };
    };
    union {
        u16 hl;
        struct { u8 h; u8 l; };
    };
    u16 sp;
    u16 pc;
} Register_File;

#define bit(n, b) ((n & (1 << 7)) >> 7)
#define zero_flag() (bit(cpu.rf.f, 7))

#define pc_reg (cpu.rf.pc)
#define af_reg (cpu.rf.af)
#define bc_reg (cpu.rf.bc)
#define de_reg (cpu.rf.de)
#define hl_reg (cpu.rf.hl)
#define sp_reg (cpu.rf.sp)

typedef struct {
    Register_File rf;
    bool halted;
} CPU;

CPU cpu;

void init_cpu() {
    cpu.rf.pc = 0x100;
    cpu.halted = false;
}

bool cpu_step() {
    if (cpu.halted) return false;
    //printf("About to execute opcode 0x%2.2x at 0x%2.2x\n", mbus_read(cpu.rf.pc), cpu.rf.pc);
    printf("AF: %2.2x\tBC: %2.2x\tDE: %2.2x\tHL: %2.2x\tSP: %2.2x\n",
        af_reg, bc_reg, de_reg, hl_reg, sp_reg);
    printf("> %2.2x: %2.2x %2.2x %2.2x\n\n", pc_reg, mbus_read(pc_reg), mbus_read(pc_reg+1), mbus_read(pc_reg+2));
    switch (cart.rom_data[pc_reg++]) {
    case 0x00: { // NOP
    } break;
    case 0xC3: { // JP NZ,a16
        if (!zero_flag()) {
            u16 jaddr = mbus_read(pc_reg);
            jaddr |= (mbus_read(pc_reg + 1) << 8);
            pc_reg = jaddr;
        }
    } break;
    default: {
        fprintf(stderr, "Unknown opcode: 0x%2.2x\n", cart.rom_data[pc_reg-1]);
        pc_reg++;
        return false;
    }
    }
    return true;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "[!] Usage: %s <rom file>\n", argv[0]);
        exit(1);
    }

    if (!load_cartridge(argv[1])) {
        fprintf(stderr, "[!] Unable to load Cartridge.\n");
        exit(1);
    }

    init_cpu();
    while (cpu_step());
    return 0;
}
