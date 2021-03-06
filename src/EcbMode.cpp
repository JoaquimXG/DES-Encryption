#include "../include/CryptMode.h"
#include "../include/DesAlgorithm.h"
#include <vector>
#include <iostream>

EcbMode::EcbMode(CryptParameters* params, CryptOption* opt)
    : CryptMode(params, opt){
        this->cryptAlgo = CryptMode::getAlgorithm(opt->cryptMethod);
    };

void EcbMode::encrypt(){
    ecbStructure(&CryptAlgorithm::encrypt);
}

void EcbMode::decrypt(){
    ecbStructure(&CryptAlgorithm::decrypt);
}

void EcbMode::ecbStructure(std::vector<unsigned> (CryptAlgorithm::*crypt)(std::vector<std::vector<unsigned>>& keyGroup, std::vector<unsigned>::const_iterator plaintextIterator)){
  std::vector<unsigned>::iterator inputIt = (*params->inputTextVect).begin();
  std::vector<unsigned> resultBlock(64, 0);
  for (int i = 0; i < this->params->numberOfBlocks; i++){
    resultBlock = (this->cryptAlgo->*crypt)(params->keyVect, inputIt);
    resultVect.insert(resultVect.end(), resultBlock.begin(), resultBlock.end());
    inputIt = inputIt + 64;
  }
  return;
}
