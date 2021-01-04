# DES-Encryption

A binary to encrypt and decrypt using the DES encryption algorithm.

# Setup

1. Clone Repository 
2. Run `make`

# Usage

```
Crypt

Usage:
    crypt decrypt <plain text file> [-k key] [-i IV] [-o outFile] [-m mode]
    crypt encrypt <cypher text file> [-k key] [-i IV] [-o outFile] [-m mode]

Options:
    -h --help     Show this screen.
    -i IV         Initialisation vector for encryption
    -k key        Encryption key
    -o outFile    Output file
    -m mode       Mode of operation [ECB | CBC | PCBC | CFB | OFB]
```

# Testing and cleanup

## Testing
You can run `make test` to initiate a very basic test of the encryption functionality.
The makefile will generate a short file, encrypt the file with `OLDencryptor` then decrypt with `OLDdecryptor`. 

These file are then encrypted and decrypted using the new binaries and the outputs are compared

## Cleanup
Running `make clean` will remove all object files, binaries and test files.

# Roadmap

Moving all functionality into class structure to improve readability and extensibility.

