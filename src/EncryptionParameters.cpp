#include "../include/EncryptionParameters.h"
#include "../include/functions.h"
#include "../include/permutations.h"


EncryptionParameters::EncryptionParameters(std::string iv, std::string key){
  this->generateSubKeys(key);
  charToBit(iv, this->ivArray);
}

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
void EncryptionParameters::generateSubKeys(std::string &key) {
  unsigned keyPC1[2][28];
  unsigned keya64bit[64];
  charToBit(key, keya64bit);

  for (int i = 0; i < 56; i++) {
    keyPC1[0][i] = keya64bit[permutedChoice1[i]];
  }

  for (int i = 0; i < 16; i++) {
    leftShift(&keyPC1[0][0], 28, bitRotationTable[i]);
    leftShift(&keyPC1[1][0], 28, bitRotationTable[i]);

    for (int j = 0; j < 48; j++) {
      //(*keya)[i][j] = keyPC1[0][permutedChoice2[j]];
      this -> keyArray[i][j] = keyPC1[0][permutedChoice2[j]];
    }
  }
}

int EncryptionParameters::parseInputFile(std::string inputFileName){
  std::ifstream inputFile;
    inputFile.open(inputFileName.c_str());
    if (!inputFile.is_open()) {
      std::cout << "[-] ------------ Can't open input file ------------\n";
      //TODO handle this
      return -2;
    }

  // Consrtuct string varaible by iterating through file buffer
  std::string inputFileString{std::istreambuf_iterator<char>(inputFile),
                            std::istreambuf_iterator<char>()};

  // calculate the length of the last chunk of data to be encrypted and the
  // difference from 8 bytes
  this -> fileLength = inputFileString.size();
  this -> padding = (8 - (this -> fileLength % 8)) % 8;
  this -> numberOfBlocks = (this -> fileLength + this -> padding) / 8;
  unsigned pta[this -> numberOfBlocks][64];

  charToBit(inputFileString, &pta[0][0]);

  if (this-> padding > 0) {
    // pointer to the last position in array
    unsigned *tempit = &pta[(this -> numberOfBlocks)-1][64 - 1];
    for (int i = 0; i < this -> padding * 8; i++) {
      *tempit = 0;
      tempit--;
    }
  }
  *(this -> plainTextArray) = new unsigned[this->numberOfBlocks *64];
}
