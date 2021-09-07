package main

import (
    "log"
    "os"
    "bufio"
)

func init() {
    // Fill the empty elements of the array with nop instructions.
    for k, v := range instructions {
        if v == nil {
            opcode := k
            instructions[k] = func(gb *Gameboy) {
                log.Printf("Unimplemented opcode: %#2x", opcode)
                // pauses the program until the user insert a new line
                bufio.NewReader(os.Stdin).ReadBytes('\n')
            }
        }
    }
}

var instructions = []func(gb *Gameboy) {
    0x00: func(gb *Gameboy) {},
}
