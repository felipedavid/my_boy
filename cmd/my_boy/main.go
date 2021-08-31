package main

import (
    "flag"
    "os"
    "path/filepath"
    "fmt"
    "errors"
)

func main() {
    flag.Parse()

    path := flag.Arg(0)
    data, err := ReadROM(path)
    if err != nil {
        fmt.Fprintf(os.Stderr, "cannot read ROM data: %s\n", err)
        os.Exit(1)
    }

    fmt.Println(data)
}

func ReadROM(path string) ([]byte, error) {
    if path == "" {
        return []byte{}, errors.New("specify .gb file path")
    }

    if filepath.Ext(path) != ".gb" {
        return []byte{}, errors.New("select a .gb file")
    }

    bytes, err := os.ReadFile(path)
    if err != nil {
        return []byte{}, errors.New("fail ro read file")
    }
    return bytes, nil
}
