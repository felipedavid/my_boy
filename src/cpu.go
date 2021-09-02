package main

type register struct {
    value uint16
}

// gets the higher byte of the register
func (reg *register) GetHigh() byte {
    return byte(reg.value >> 8)
}

// gets the lower byte of the register
func (reg *register) GetLow() byte {
    return byte(reg.value & 0xFF)
}

// set the high byte of the register
func (reg *register) Set(value uint16) {
    reg.value = value
}

// set the high byte of the register
func (reg *register) SetHigh(value byte) {
    reg.value = uint16(value) << 8 | (reg.value & 0xFF)
}

// set the lower byte of the register
func (reg *register) SetLow(value byte) {
    reg.value = uint16(value) | (reg.value & 0xFF00)
}

type CPU struct {
    AF, BC, DE, HL, PC, SP register
}

// Setting up the CPU initial state
func (cpu *CPU) Init() {
    cpu.AF.Set(0x1180)
    cpu.BC.Set(0x0000)
    cpu.DE.Set(0xFF56)
    cpu.HL.Set(0x000D)
    cpu.PC.Set(0x0180)
    cpu.SP.Set(0xFFFE)
}

// Set the value of the flag on the FA register
func (cpu *CPU) setFlag(index byte, on bool) {
    if on {
        cpu.AF.SetLow(cpu.AF.GetLow() | (1 << index))
    } else {
        cpu.AF.SetLow(cpu.AF.GetLow() & ^(1 << index))
    }
}

// Set the Zero flag
func (cpu *CPU) SetZFlag(on bool) {
    cpu.setFlag(7, on)
}

// Set the Subtraction flag (Used only in the DAA instruction)
func (cpu *CPU) SetNFlag(on bool) {
    cpu.setFlag(6, on)
}

// Set the Half Carry flag (Used only in the DAA instruction)
func (cpu *CPU) SetHFlag(on bool) {
    cpu.setFlag(5, on)
}

// Set the Carry flag
func (cpu *CPU) CetCFlag(on bool) {
    cpu.setFlag(4, on)
}

// Check if the Zero flag is active
func (cpu *CPU) CheckZFlag() bool {
    return ((cpu.AF.GetLow() >> 7) & 1) == 1
}

// Check if the Subtraction flag is active
func (cpu *CPU) CheckNFlag() bool {
    return ((cpu.AF.GetLow() >> 6) & 1) == 1
}

// Check if the Half Carry flag is active
func (cpu *CPU) CheckHFlag() bool {
    return ((cpu.AF.GetLow() >> 5) & 1) == 1
}

// Check if the Carry flag is active
func (cpu *CPU) CheckCFlag() bool {
    return ((cpu.AF.GetLow() >> 4) & 1) == 1
}

