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

void EcbMode::encrypt(){
    ecbStructure(&CryptAlgorithm::encrypt);
}

void EcbMode::decrypt(){
    ecbStructure(&CryptAlgorithm::decrypt);
}

void EcbMode::ecbStructure(std::vector<unsigned> (CryptAlgorithm::*crypt)(std::vector<std::vector<unsigned>>& keyGroup, std::vector<unsigned>::const_iterator plaintextIterator)){
  std::vector<unsigned>::iterator inputIt = (*params->inputTextVect).begin();
  std::vector<unsigned> decryptedBlock(64, 0);
  for (int i = 0; i < this->params->numberOfBlocks; i++){
    decryptedBlock = (this->cryptAlgo->*crypt)(params->keyVect, inputIt);
    resultVect.insert(resultVect.end(), decryptedBlock.begin(), decryptedBlock.end());
    inputIt = inputIt + 64;
  }
  return;
}
