#include <iostream>
#include <math.h>
#include <string>
#include <bitset>
#include <sstream>
#include <fstream>
#include <vector>

template<typename T>
int arrayout(T array[],int size,int split,int file=0,bool hex=0);//prints an array to terminal where size is the length of the array and split is the length of each line
int leftshift(unsigned array[],int size,int shift);
int chartobit(std::string string,unsigned array[]);//takes string input and saves binary representation to an array
int XOR(unsigned first[],unsigned second[],unsigned result[],int size);//only to be used on arrays that are binary representations, first and second are the arrays to XOR and result will be the address they are stored, size is the size of the arrays to be XOR'd
int feistal(unsigned target[],unsigned result[],unsigned key[]);//result is the address the result will be stored, key is the subkey required for this feistal
int binarytodecimal(unsigned binarya[],int size);//converts a binary array into an integer value
void decimaltobinary(int integer,unsigned binarya[],int size);//converts a decimal integer into binary, storing the result at the address of binarya, size is the length of array storing the data
void DES(unsigned (*pta)[64],unsigned (*subkeya)[16][48],unsigned (*ct)[2][32],unsigned (*ctbit)[64],int i,int mode =0,int decrypt=0);//performs DES encryption on a given plaintext using 16 given keys storing the result in a resulting ct array
void DESkeygenerate(std::string &key,unsigned (*keya)[16][48]);//generates 16 48bit subkeys for DES from a 64 bit key
void getkey(unsigned (*keya)[16][48],int mode=0);//takes a key as input and generates the keys required for certain encryption algorithms
void getIV(unsigned (*IVa)[64]);//takes IV input and converts to binary array format
void getfile();//takes file as input with error checking, requires there to be a valid std::ifstream input to utilise
void hextobit(std::string file_contents,unsigned *array);//converts a hex string into binary array form





