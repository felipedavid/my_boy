package main

type Memory struct {
    // Cartridge
    CRom []byte
    CRam []byte

    WRam []byte
}

func (m *Memory) Write(addr uint64, value byte) {
    switch {
    case addr < 0x8000: // Cartridge ROM
        m.CRom[addr] = value
    case addr < 0xA000: // VRAM
    case addr < 0xC000: // Cartridge RAM
        m.CRam[addr & 0x1FFF] = value
    case addr < 0xE000: // Internal RAM
        m.WRam[addr & 0x1FFF] = value
    case addr < 0xFE00: // Internal RAM (shadow)
    }
}

func (m *Memory) Read(addr uint64) byte {
    switch {
    case addr < 0x8000: // Cartridge ROM
        return m.CRom[addr]
    case addr < 0xA000: // VRAM
        return 0
    case addr < 0xC000: // Cartridge RAM
        return m.CRam[addr & 0x1FFF]
    case addr < 0xE000: // Internal RAM
        return m.WRam[addr & 0x1FFF]
    case addr < 0xFE00: // Internal RAM (shadow)
        return 0

    }
    return 0;
}

func (m *Memory) Init(rom_data []byte) {
    m.CRom = rom_data
}
