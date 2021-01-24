#include "../include/CryptMode.h"
#include "../include/CryptAlgorithm.h"
#include <vector>
#include <iostream>

EcbMode::EcbMode(CryptParameters* params, CryptAlgorithm* cryptAlgo)
    : CryptMode(params, cryptAlgo){};

//TODO documentation
//ECB mode just loops through each block
void EcbMode::encrypt(){
  std::vector<unsigned>::iterator inputIt = (*params->plainTextVect).begin();
  std::vector<unsigned> encryptedBlock(64, 0);
  for (int i = 0; i < this->params->numberOfBlocks; i++){
    encryptedBlock = this->cryptAlgo->encrypt(params->keyVect, inputIt);
    resultVect.insert(resultVect.end(), encryptedBlock.begin(), encryptedBlock.end());
    inputIt = inputIt + 64;
  }
}

void EcbMode::decrypt(){
  for (int i = 0; i < this->params->numberOfBlocks; i++){
    //this->cryptAlgo->decrypt();
  }
}

