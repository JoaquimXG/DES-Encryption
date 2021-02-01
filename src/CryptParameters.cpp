#include "../include/CryptParameters.h"
#include "../include/OLDpermutations.h"
#include "../include/cryptUtils.h"
#include "../include/constants.h"
#include <fstream>
#include <iostream>
#include <sstream>

CryptParameters::CryptParameters(int inputTextLength, bool toDecrypt)
    : numberOfBlocks(0), padding(0), inputTextLength(inputTextLength),
      toDecrypt(toDecrypt), inputTextVect(nullptr),
      keyVect(DESKEYNUMBER, std::vector<unsigned>(DESKEYSIZE, 0)), ivVect(DESBLOCKSIZE, 0){}; 

void CryptParameters::generateSubKeys(std::string &key) {
  std::vector<unsigned> keyPermutedChoice1(KEYPERMUTEDCHOICE1SIZE, 0);

  std::vector<unsigned> temporary64BitKey(64);
  charToBin(key, temporary64BitKey);
  for (int i = 0; i < KEYPERMUTEDCHOICE1SIZE; i++) {
    keyPermutedChoice1[i] = temporary64BitKey[keyPermutedChoice1Table[i]];
  }

  for (int i = 0; i < DESKEYNUMBER; i++) {
    leftShift(keyPermutedChoice1, keyBitRotationTable[i], 28, 55);
    leftShift(keyPermutedChoice1, keyBitRotationTable[i], 0, 27);

    for (int j = 0; j < DESKEYSIZE; j++) {
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
  this->numberOfBlocks = this->inputTextLength / HEXCHARBITS;
  this->inputTextVect = new std::vector<unsigned>(numberOfBlocks * DESBLOCKSIZE, 0);
  
  hexToBin(inFileString, *this->inputTextVect);
}

void CryptParameters::parseCharInputFile(std::string inFileString){
  this->inputTextLength = inFileString.size();
  this->padding = (CHARBITS - (this->inputTextLength % CHARBITS)) % CHARBITS;
  this->numberOfBlocks = (this->inputTextLength + this->padding) / CHARBITS;
  this->inputTextVect = new std::vector<unsigned>(numberOfBlocks * DESBLOCKSIZE, 0);
  
  charToBin(inFileString, *this->inputTextVect);

  if (this->padding > 0) {
    for (int i = 0; i < this->padding * CHARBITS; i++) {
      inputTextVect[(this->numberOfBlocks) - 1][DESBLOCKSIZE - 1 - i];
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
