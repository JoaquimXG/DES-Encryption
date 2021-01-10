#include "../include/CryptParameters.h"
#include "../include/OLDpermutations.h"
#include "../include/cryptUtils.h"
#include <fstream>
#include <iostream>
#include <sstream>

// TODO Declare constants for the dimensions of the arrays that are inuse

CryptParameters::CryptParameters(int plainTextLength)
    : numberOfBlocks(0), padding(0), plainTextLength(plainTextLength),
      plainTextVect(nullptr),
      keyVect(16, std::vector<unsigned>(48, 0)), ivVect(64, 0){}; 
/*
 * Generates 16 48bit subkeys for DES from a 64 bit key.
 * The 64 bit key is first converted from characters to binary.
 * 56 bits are then selected from the key using the Permuted Choice 1 table
 * This 56 bit key is split in half into two 28 bit keys.
 * For 16 rounds,
 * Each 28 bit key is left shifted either 1 or 2 bits in a predefined order
 * as defined by bitRotationTable.
 * 48 bits are then selected from a combination of the 28 bit keys using
 * the Permuted Choice 2 table to form the each 48 bit key.
 *
 * @param &key A reference to std::string for the key input
 */
void CryptParameters::generateSubKeys(std::string &key) {
  std::vector<unsigned> keyPermutedChoice1(56, 0);

  std::vector<unsigned> temporary64BitKey(64);
  charToBit(key, temporary64BitKey);
  for (int i = 0; i < 56; i++) {
    keyPermutedChoice1[i] = temporary64BitKey[permutedChoice1[i]];
  }


  for (int i = 0; i < 16; i++) {
    leftShift(keyPermutedChoice1, bitRotationTable[i], 28, 55);
    leftShift(keyPermutedChoice1, bitRotationTable[i], 0, 27);

    for (int j = 0; j < 48; j++) {
      //(*keya)[i][j] = keyPermutedChoice1[0][permutedChoice2[j]];
      this->keyVect[i][j] = keyPermutedChoice1[permutedChoice2[j]];
    }
  }
}

int CryptParameters::parseInputFile(std::string inFileString) {

  // calculate the length of the last chunk of data to be encrypted and the
  // difference from 8 bytes
  this->plainTextLength = inFileString.size();
  this->padding = (8 - (this->plainTextLength % 8)) % 8;
  // TODO Do I need to use the encryption size in the number of blocks
  // calculation???
  this->numberOfBlocks = (this->plainTextLength + this->padding) / 8;
  std::cout << numberOfBlocks;
  this->plainTextVect = new std::vector<unsigned>(numberOfBlocks * 64, 0);
  
  charToBit(inFileString, *this->plainTextVect);

  if (this->padding > 0) {
    // pointer to the last position in array
    for (int i = 0; i < this->padding * 8; i++) {
      plainTextVect[(this->numberOfBlocks) - 1][64 - 1 - i];
    }
  }
  return 0;
}

// TODO document toString
std::string CryptParameters::toString() {
  std::stringstream returnString;
  returnString << "\n\nIV: \n";
  returnString << vectorToString(this->ivVect);

  returnString << "\n\nKey Vect: \n";
  returnString << vectorToString2D(this->keyVect) << "\n";

  returnString << "\nPlain Text Vect: \n";
  returnString << vectorToString(*this->plainTextVect) << "\n";
  
  returnString << "\nPlain Text Length: " << this->plainTextLength
               << "\nNumber of Blocks: " << this->numberOfBlocks
               << "\nPadding: " << this->padding << "\n";
  return returnString.str();
}
