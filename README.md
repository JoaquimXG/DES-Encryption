# DES-Encryption

Functions utilised by both the Encryptor and Decryptor stored in functions.cpp

DES encryption function in DES.cpp 

#Usage

##DES Encryptor

Usage:
    encrypt inFile [-o outFile] [-m mode]

Options:
    -h --help     Show this screen.
    -o outFile    Output hex file
    -m mode       Method of encryption [ECB | CBC | PCBC | CFB | OFB]

##DES Decryptor
TODO

# To-Do

Increase efficiency of functions like leftshift
and remove the necessity for converting integers to binary array representations by utilising bit manipulation

Add functionality for AES and other encryption algorithms

# Usage 

Encryption/Decryption [cbc] [pcbc] [cfb] [ofb]

Program will ask for 8byte key and IV and file for input

for encryption a plaintext txt is required for input 

for decryption a hex file is required (encryption outputs hex)
