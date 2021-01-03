#include "../include/Options.h"
#include <bitset>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

template <typename T>
// prints an array to terminal where size is the length of the array and split
// is the length of each line
int arrayout(T array[], int size, int split, int file = 0, bool hex = 0);

int leftshift(unsigned array[], int size, int shift);

// takes string input and saves binary representation to an array
int chartobit(std::string string, unsigned array[]);

// only to be used on arrays that are binary representations, first and second
// are the arrays to XOR and result will be the address they are stored, size is
// the size of the arrays to be XOR'd
int XOR(unsigned first[], unsigned second[], unsigned result[], int size);

// result is the address the result will be stored, key is the subkey required
// for this feistal
int feistal(unsigned target[], unsigned result[], unsigned key[]);

// converts a binary array into an integer value
int binarytodecimal(unsigned binarya[], int size);

// converts a decimal integer into binary, storing the result at the address of
// binarya, size is the length of array storing the data
void decimaltobinary(int integer, unsigned binarya[], int size);

// performs DES encryption on a given plaintext using 16 given keys storing the
// result in a resulting ct array
void DES(unsigned (*pta)[64], unsigned (*subkeya)[16][48], unsigned (*ct)[2][32], unsigned (*ctbit)[64], int i, int mode = 0, int decrypt = 0);

// generates 16 48bit subkeys for DES from a 64 bit key
void DESkeygenerate(std::string &key, unsigned (*keya)[16][48]);

// takes a key as input and generates the keys required for certain encryption
// algorithms
void getkey(std::string key, unsigned (*keya)[16][48]);

// takes file as input with error checking, requires there to be a valid
// std::ifstream input to utilise
void getfile();

// converts a hex string into binary array form
void hextobit(std::string file_contents, unsigned *array);

// Uses getopt to parse command line argumemts
int parseCommandLineArguments(int argc, char *argv[], Options *encryptOpts,
                              bool decrypt = false);

// Just parses the encryption method argument
int parseEncryptionModeArg(std::string, Options *encryptOpts);

// Prints the usage for this program
void printUsage(bool decrypt = false);
