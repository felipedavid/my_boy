
func init() {
    // Fill the empty elements of the array with nop instructions.
    for k, v := range instructions {
        if v == nil {
            opcode := k
            instrucitons[k] = func(gb *Gameboy) {
                log.Printf("Unimplemented opcode: %#2x", opcode)
                os.Exit(1)
            }
        }
    }
}

var instructions = []func(gb *Gameboy) {
    0x00: func(gb *Gameboy) {},
}
