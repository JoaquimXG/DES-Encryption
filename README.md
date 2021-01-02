# DES-Encryption

A set of binaries to encrypt and decrypt using the DES encryption algorithm.

# Installation

1. Clone Repository 
2. Run `make`
3. Utilise `encryptor` to encrypt
4. Utilise `decryptor` to decrypt

# Usage

## DES Encryptor

```
DES Encryptor

Usage:
    encryptor inFile [-k key] [-i IV] [-o outFile] [-m mode]

Options:
    -h --help     Show this screen.
    -i IV         Initialisation vector for encryption
    -k key        Encryption key
    -o outFile    Output file
    -m mode       Mode of operation [ECB | CBC | PCBC | CFB | OFB]
```

## DES Decryptor

```
DES Decryptor

Usage:
    decryptor inFile [-k key] [-i IV] [-o outFile] [-m mode]

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
The makefile will generate a short file, encrypt the file with `encryptor` then decrypt with `decryptor`. 
The files are then compared and the output is displayed. 

## Cleanup

Running `make clean` will remove all object files, binaries and test files.


# Roadmap

Increase efficiency of functions like leftshift
and remove the necessity for converting integers to binary array representations by utilising bit manipulation

Add functionality for AES and other encryption algorithms
