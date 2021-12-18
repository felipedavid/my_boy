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

const char *inst_mm[] = {
    "NOP",
    "LD BC,d16",
    "LD (BC),A",
    "INC BC",
    "INC B",
    "DEC B",
    "LD B,d8",
    "RLCA",
    "LD (a16),SP",
    "ADD HL,BC",
    "LD A,(BC)",
    "DEC BC",
    "INC C",
    "DEC C",
    "LD C,d8",
    "RRCA",
    "STOP 0",
    "LD DE,d16",
    "LD (DE),A",
    "INC DE",
    "INC D",
    "DEC D",
    "LD D,d8",
    "RLA",
    "JR r8",
    "ADD HL,DE",
    "LD A,(DE)",
    "DEC DE",
    "INC E",
    "DEC E",
    "LD E,d8",
    "RRA",
    "JR NZ,r8",
    "LD HL,d16",
    "LD (HL+),A",
    "INC HL",
    "INC H",
    "DEC H",
    "LD H,d8",
    "DAA",
    "JR Z,r8",
    "ADD HL,HL",
    "LD A,(HL+)",
    "DEC HL",
    "INC L",
    "DEC L",
    "LD L,d8",
    "CPL",
    "JR NC,r8",
    "LD SP,d16",
    "LD (HL-),A",
    "INC SP",
    "INC (HL)",
    "DEC (HL)",
    "LD (HL),d8",
    "SCF",
    "JR C,r8",
    "ADD HL,SP",
    "LD A,(HL-)",
    "DEC SP",
    "INC A",
    "DEC A",
    "LD A,d8",
    "CCF",
    "LD B,B",
    "LD B,C",
    "LD B,D",
    "LD B,E",
    "LD B,H",
    "LD B,L",
    "LD B,(HL)",
    "LD B,A",
    "LD C,B",
    "LD C,C",
    "LD C,D",
    "LD C,E",
    "LD C,H",
    "LD C,L",
    "LD C,(HL)",
    "LD C,A",
    "LD D,B",
    "LD D,C",
    "LD D,D",
    "LD D,E",
    "LD D,H",
    "LD D,L",
    "LD D,(HL)",
    "LD D,A",
    "LD E,B",
    "LD E,C",
    "LD E,D",
    "LD E,E",
    "LD E,H",
    "LD E,L",
    "LD E,(HL)",
    "LD E,A",
    "LD H,B",
    "LD H,C",
    "LD H,D",
    "LD H,E",
    "LD H,H",
    "LD H,L",
    "LD H,(HL)",
    "LD H,A",
    "LD L,B",
    "LD L,C",
    "LD L,D",
    "LD L,E",
    "LD L,H",
    "LD L,L",
    "LD L,(HL)",
    "LD L,A",
    "LD (HL),B",
    "LD (HL),C",
    "LD (HL),D",
    "LD (HL),E",
    "LD (HL),H",
    "LD (HL),L",
    "HALT",
    "LD (HL),A",
    "LD A,B",
    "LD A,C",
    "LD A,D",
    "LD A,E",
    "LD A,H",
    "LD A,L",
    "LD A,(HL)",
    "LD A,A",
    "ADD A,B",
    "ADD A,C",
    "ADD A,D",
    "ADD A,E",
    "ADD A,H",
    "ADD A,L",
    "ADD A,(HL)",
    "ADD A,A",
    "ADC A,B",
    "ADC A,C",
    "ADC A,D",
    "ADC A,E",
    "ADC A,H",
    "ADC A,L",
    "ADC A,(HL)",
    "ADC A,A",
    "SUB B",
    "SUB C",
    "SUB D",
    "SUB E",
    "SUB H",
    "SUB L",
    "SUB (HL)",
    "SUB A",
    "SBC A,B",
    "SBC A,C",
    "SBC A,D",
    "SBC A,E",
    "SBC A,H",
    "SBC A,L",
    "SBC A,(HL)",
    "SBC A,A",
    "AND B",
    "AND C",
    "AND D",
    "AND E",
    "AND H",
    "AND L",
    "AND (HL)",
    "AND A",
    "XOR B",
    "XOR C",
    "XOR D",
    "XOR E",
    "XOR H",
    "XOR L",
    "XOR (HL)",
    "XOR A",
    "OR B",
    "OR C",
    "OR D",
    "OR E",
    "OR H",
    "OR L",
    "OR (HL)",
    "OR A",
    "CP B",
    "CP C",
    "CP D",
    "CP E",
    "CP H",
    "CP L",
    "CP (HL)",
    "CP A",
    "RET NZ",
    "POP BC",
    "JP NZ,a16",
    "JP a16",
    "CALL NZ,a16",
    "PUSH BC",
    "ADD A,d8",
    "RST 00H",
    "RET Z",
    "RET",
    "JP Z,a16",
    "PREFIX CB",
    "CALL Z,a16",
    "CALL a16",
    "ADC A,d8",
    "RST 08H",
    "RET NC",
    "POP DE",
    "JP NC,a16",
	"NONE",
    "CALL NC,a16",
    "PUSH DE",
    "SUB d8",
    "RST 10H",
    "RET C",
    "RETI",
    "JP C,a16",
	"NONE",
    "CALL C,a16",
	"NONE",
    "SBC A,d8",
    "RST 18H",
    "LDH (a8),A",
    "POP HL",
    "LD (C),A",
	"NONE",
	"NONE",
    "PUSH HL",
    "AND d8",
    "RST 20H",
    "ADD SP,r8",
    "JP (HL)",
    "LD (a16),A",
	"NONE",
	"NONE",
	"NONE",
    "XOR d8",
    "RST 28H",
    "LDH A,(a8)",
    "POP AF",
    "LD A,(C)",
    "DI",
	"NONE",
    "PUSH AF",
    "OR d8",
    "RST 30H",
    "LD HL,SP+r8",
    "LD SP,HL",
    "LD A,(a16)",
    "EI",
	"NONE",
	"NONE",
    "CP d8",
    "RST 38H",
};

void log_cartridge_info() {
    printf("Title: %s\n", cart.header->title);
    printf("Type: %2.2x (%s)\n", cart.header->type, cart_type_string());
    printf("ROM Size: %d KB\n", 32 << cart.header->rom_size);
    printf("RAM Size: %2.2x\n", cart.header->ram_size);
    printf("License Code: %2.2x (%s)\n", cart.header->old_license_code, cart_license_code_string());
    printf("ROM Version: %2.2x\n\n", cart.header->version);
}

void load_cartridge(const char *filename) {
    // Open file
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "[!] Unable to open file \"%s\".", filename);
        exit(1);
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    cart.rom_size = ftell(fp);
    rewind(fp);

    // Read ROM data and setup struct
    cart.rom_data = xmalloc(cart.rom_size);
    fread(cart.rom_data, cart.rom_size, 1, fp);
    fclose(fp);

    // TODO: Check if it is a valid ROM

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
    }

    printf("[*] Cartridge loaded.\n");
    log_cartridge_info();
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

typedef struct {
    Register_File rf;
    u8 ie_reg;
    bool halted;
    bool disable_interrupts;
} CPU;
CPU cpu;

typedef struct {
    u8 wram[0x2000];
    u8 hram[0x80];
} RAM;

RAM ram;

u8 mbus_read(u16 addr) {
    if (addr < 0x8000) { // ROM Data
        return cart.rom_data[addr];
    } else if (addr < 0xA000) { // VRAM
    } else if (addr < 0xC000) { // External RAM
    } else if (addr < 0xE000) { // WRAM
        return ram.wram[addr - 0xC000];
    } else if (addr < 0xFE00) { // ECHO RAM
    } else if (addr < 0xFEA0) { // OAM
    } else if (addr < 0xFF00) { // Not usable
    } else if (addr < 0xFF80) { // I/O Registers
    } else if (addr < 0xFFFF) { // High RAM
        return ram.hram[addr - 0xFF80];
    } else if (addr == 0xFFFF) { // IE Register
        return cpu.ie_reg;
    }
    fprintf(stderr, "[!] Can't read from address %hx\n\n", addr);
}

inline u16 mbus_read16(u16 addr) {
    return mbus_read(addr) | (mbus_read(addr+1) << 8);
}

void mbus_write(u16 addr, u8 value) {
    if (addr < 0x8000) { // ROM Data
        cart.rom_data[addr] = value;
        return;
    } else if (addr < 0xA000) { // VRAM
    } else if (addr < 0xC000) { // External RAM
    } else if (addr < 0xE000) { // WRAM
        ram.wram[addr - 0xC000] = value;
        return;
    } else if (addr < 0xFE00) { // ECHO RAM
    } else if (addr < 0xFEA0) { // OAM
    } else if (addr < 0xFF00) { // Not usable
    } else if (addr < 0xFF80) { // I/O Registers
    } else if (addr < 0xFFFF) { // High RAM
        ram.hram[addr - 0xFF80] = value;
        return;
    } else if (addr == 0xFFFF) { // IE Register
        cpu.ie_reg = value;
        return;
    }
    fprintf(stderr, "[!] Can't write to address %hx\n\n", addr);
}

inline void mbus_write16(u16 addr, u16 value) {
    mbus_write(addr, value & 0xFF);
    mbus_write(addr+1, value >> 8);
}

#define bit(n, b) ((n & (1 << b)) >> b)
#define set_bit(n, b) (n |= (1 << b))
#define unset_bit(n, b) (n &= (~(1 << b)))

#define get_z_flag() (bit(cpu.rf.f, 7))
#define set_z_flag() (set_bit(cpu.rf.f, 7))
#define unset_z_flag() (unset_bit(cpu.rf.f, 7))

#define get_n_flag() (bit(cpu.rf.f, 6))
#define set_n_flag() (set_bit(cpu.rf.f, 6))
#define unset_n_flag() (unset_bit(cpu.rf.f, 6))

#define get_h_flag() (bit(cpu.rf.f, 5))
#define set_h_flag() (set_bit(cpu.rf.f, 5))
#define unset_h_flag() (unset_bit(cpu.rf.f, 5))

#define get_c_flag() (bit(cpu.rf.f, 4))
#define set_c_flag() (set_bit(cpu.rf.f, 4))
#define unset_c_flag() (unset_bit(cpu.rf.f, 4))


void init_cpu() {
    cpu.rf.pc = 0x100;
    cpu.rf.a = 0x1;
    cpu.halted = false;
}

bool cpu_step() {
    if (cpu.halted) return false;

    printf("AF: %04x   BC: %04x   DE: %04x   HL: %04x   SP: %04x   PC: %04x\n",
        cpu.rf.af, cpu.rf.bc, cpu.rf.de, cpu.rf.hl, cpu.rf.sp, cpu.rf.pc);
    printf("> %02x: %02x %02x %02x [%s]\n\n", 
        cpu.rf.pc, mbus_read(cpu.rf.pc), mbus_read(cpu.rf.pc+1), mbus_read(cpu.rf.pc+2), 
		inst_mm[mbus_read(cpu.rf.pc)]);

    switch (cart.rom_data[cpu.rf.pc++]) {
    case 0x00: { // NOP
    } break;
    case 0x01: { // LD BC,d16
        cpu.rf.bc = mbus_read16(cpu.rf.pc);
        cpu.rf.pc += 2;
    } break;
    case 0x02: { // LD (BC),A
        mbus_write(cpu.rf.pc, cpu.rf.a);
    } break;
    case 0x21: { // LD HL,d16
        cpu.rf.hl = mbus_read16(cpu.rf.pc);
        cpu.rf.pc += 2;
    } break;
    case 0x31: { // LD SP,d16
        cpu.rf.sp = mbus_read16(cpu.rf.pc);
        cpu.rf.pc += 2;
    }; break;
    case 0x3E: {
        cpu.rf.a = mbus_read(cpu.rf.pc++);
    } break;
    case 0xAF: { // XOR A
       cpu.rf.a = 0;
       unset_z_flag();
       unset_n_flag();
       unset_h_flag();
       unset_c_flag();
    }; break;
    case 0xC3: { // JP NZ,a16
        if (!get_z_flag()) 
            cpu.rf.pc = mbus_read16(cpu.rf.pc);
    } break;
    case 0xD6: { // SUB d8
        cpu.rf.a -= mbus_read(cpu.rf.pc++);
    } break;
    case 0xE0: { // LDH (a8),A
        mbus_write(0xFF00 + mbus_read(cpu.rf.pc++), cpu.rf.a);
    } break;
    case 0xEA: { // LD (a16),A
        u16 addr = mbus_read16(cpu.rf.pc);
        mbus_write(addr, cpu.rf.a);
        cpu.rf.pc += 2;
    }; break;
    case 0xF3: {
        cpu.disable_interrupts = false;
    } break;
    default: {
        fprintf(stderr, "Unknown opcode: 0x%2.2x\n", cart.rom_data[cpu.rf.pc-1]);
        cpu.rf.pc++;
        return false;
    }
    }
    return true;
}

inline void stack_push(u8 value) {
    mbus_write(--cpu.rf.sp, value);
}

inline u8 stack_pop() {
    return mbus_read(cpu.rf.sp++);
}

inline void stach_push16(u16 value) {
    cpu.rf.sp -= 2;
    mbus_write16(cpu.rf.sp, value);
}

inline u16 stack_pop16() {
    return mbus_read16(cpu.rf.sp);
    cpu.rf.sp += 2;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "[!] Usage: %s <rom file>\n", argv[0]);
        exit(1);
    }

    load_cartridge(argv[1]);
    init_cpu();
    while (cpu_step());
    return 0;
}
