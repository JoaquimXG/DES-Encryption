#include "../include/CryptMode.h"
#include "../include/CryptAlgorithm.h"

EcbMode::EcbMode(CryptParameters* params, CryptAlgorithm* cryptAlgo)
    : CryptMode(params, cryptAlgo){};

//TODO documentation
//ECB mode just loops through each block
void EcbMode::encrypt(){
  for (int i = 0; i < this->params->numberOfBlocks; i++){
    this->cryptAlgo->encrypt();
  }
}

void EcbMode::decrypt(){
  for (int i = 0; i < this->params->numberOfBlocks; i++){
    this->cryptAlgo->decrypt();
  }
}

