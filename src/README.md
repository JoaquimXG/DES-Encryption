# Source Narrative

Much of the functionality required for encryption and decryption is shared.

Following is a breakdown of the operation of both encryptor and decryptor.

## Encryptor

The encryptor takes a plaintext input file, a key and an initialisation vector (IV).
Optionally an output file can also be provided.


1. The 8 Byte key and 8 Byte IV is converted from strings to 64 bit binary arrays. 
2. 16 48 bit keys are generated from the single 64 bit key provided.
    - 56 bits are selected from the 64 bit key, using Permuted Choice 1 table
    - The 56 bit key is split in half into two 28 bit keys
    - Then for a 16 iterations
        - Each key is left shifted either 1 or 2 bits in a predefined order as defined by bitRotationTable
        - 48 bits are then selected from a combination of the 28 bit keys using the Permuted Choice 2 table.
3. Parse the input file.
    - Padding is added to the end of the file to ensure it is a multiple of 8 bytes in length. Padding consists of null bytes.
    - The number of encryption blocks is calculated, numberOfBlocks
    - The input file is converted into binary into a 2 dimensional array, `[numberOfBlocks][64]`
4. 




3. 



# Notes

## Objects
- Sixteen 48 bit keys
- 64 bit IV
