#include "../include/CryptMode.h"
#include "../include/DesAlgorithm.h"
#include <vector>
#include <iostream>

EcbMode::EcbMode(CryptParameters* params, CryptOption* opt)
    : CryptMode(params, opt){
        switch(opt->cryptMethod){
            case DES:
                this->cryptAlgo = new DesAlgorithm();
        }
    };

//TODO documentation
//ECB mode just loops through each block
void EcbMode::encrypt(){
  std::vector<unsigned>::iterator inputIt = (*params->inputTextVect).begin();
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

