#include "../include/CryptParameters.h"
#include "../include/OLDpermutations.h"
#include "../include/cryptUtils.h"
#include <fstream>
#include <iostream>
#include <sstream>

// TODO Declare constants for the dimensions of the arrays that are inuse

CryptParameters::CryptParameters(int inputTextLength, bool toDecrypt)
    : numberOfBlocks(0), padding(0), inputTextLength(inputTextLength),
      toDecrypt(toDecrypt), inputTextVect(nullptr),
      keyVect(16, std::vector<unsigned>(48, 0)), ivVect(64, 0){}; 

void CryptParameters::generateSubKeys(std::string &key) {
  std::vector<unsigned> keyPermutedChoice1(56, 0);

  std::vector<unsigned> temporary64BitKey(64);
  charToBin(key, temporary64BitKey);
  for (int i = 0; i < 56; i++) {
    keyPermutedChoice1[i] = temporary64BitKey[keyPermutedChoice1Table[i]];
  }

  for (int i = 0; i < 16; i++) {
    leftShift(keyPermutedChoice1, keyBitRotationTable[i], 28, 55);
    leftShift(keyPermutedChoice1, keyBitRotationTable[i], 0, 27);

    for (int j = 0; j < 48; j++) {
      this->keyVect[i][j] = keyPermutedChoice1[keyPermutedChoice2Table[j]];
    }
  }
}

void CryptParameters::parseInputFile(std::string inFileString) {
  if (toDecrypt){
    parseHexInputFile(inFileString);
  }
  else {
    parseCharInputFile(inFileString);
  }
}

void CryptParameters::parseHexInputFile(std::string inFileString){
  this->inputTextLength = inFileString.size();
  this->numberOfBlocks = (this->inputTextLength/2) / 8;
  this->inputTextVect = new std::vector<unsigned>(numberOfBlocks * 64, 0);
  
  hexToBin(inFileString, *this->inputTextVect);
}

void CryptParameters::parseCharInputFile(std::string inFileString){
  // calculate the length of the last chunk of data to be encrypted and the
  // difference from 8 bytes
  this->inputTextLength = inFileString.size();
  this->padding = (8 - (this->inputTextLength % 8)) % 8;
  // TODO Do I need to use the encryption size in the number of blocks
  // calculation???
  this->numberOfBlocks = (this->inputTextLength + this->padding) / 8;
  this->inputTextVect = new std::vector<unsigned>(numberOfBlocks * 64, 0);
  
  charToBin(inFileString, *this->inputTextVect);

  if (this->padding > 0) {
    for (int i = 0; i < this->padding * 8; i++) {
      inputTextVect[(this->numberOfBlocks) - 1][64 - 1 - i];
    }
  }

}

std::string CryptParameters::toString() {
  std::stringstream returnString;
  returnString << "\nIV: \n";
  returnString << binaryVectorToString(this->ivVect);

  returnString << "\n\nKey Vect: \n";
  returnString << binaryVectorToString2D(this->keyVect) << "\n";

  returnString << "\nInput Text Vect: \n";
  returnString << binaryVectorToString(*this->inputTextVect) << "\n";
  
  returnString << "\nPlain Text Length: " << this->inputTextLength
               << "\nNumber of Blocks: " << this->numberOfBlocks
               << "\nPadding: " << this->padding << "\n";
  return returnString.str();
}
