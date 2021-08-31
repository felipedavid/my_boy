package gb

type register struct {
    value uint16
}

// gets the higher byte of the register
func (reg *register) High() byte {
    return byte(reg.value >> 8)
}

// gets the lower byte of the register
func (reg *register) Low() byte {
    return byte(reg.value & 0xFF)
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
}

// Set the value of the flag on the FA register
func (cpu *CPU) setFlag(index byte, on bool) {
}

// Set the Zero flag
func (cpu *CPU) SetZFlag() {
}

// Set the Subtraction flag (Used only in the DAA instruction)
func (cpu *CPU) SetNFlag() {
}

// Set the Half Carry flag (Used only in the DAA instruction)
func (cpu *CPU) SetHFlag() {
}

// Set the Carry flag
func (cpu *CPU) SetCFlag() {
}

// Check if the Zero flag is active
func (cpu *CPU) CheckZFlag() {
}

// Check if the Subtraction flag is active
func (cpu *CPU) CheckNFlag() {
}

// Check if the Half Carry flag is active
func (cpu *CPU) CheckHFlag() {
}

// Check if the Carry flag is active
func (cpu *CPU) CheckCFlag() {
}

