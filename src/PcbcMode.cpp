#include "../include/CryptMode.h"
#include "../include/DesAlgorithm.h"
#include "../include/cryptUtils.h"
#include <vector>
#include <iostream>

PcbcMode::PcbcMode(CryptParameters* params, CryptOption* opt)
    : CryptMode(params, opt), xorVect(64, 0){
        this->cryptAlgo = getAlgorithm(opt->cryptMethod);
    };

void PcbcMode::encrypt(){
  std::vector<unsigned>::iterator inputIt = (*params->inputTextVect).begin();
  std::vector<unsigned> encryptedBlock(64, 0);
  xorVect = params->ivVect;

  for (int i = 0; i < this->params->numberOfBlocks; i++){
    XOR(inputIt, xorVect.begin(), inputIt, 64);

    encryptedBlock = this->cryptAlgo->encrypt(params->keyVect, inputIt);
    resultVect.insert(resultVect.end(), encryptedBlock.begin(), encryptedBlock.end());

    xorVect = encryptedBlock;
    inputIt = inputIt + 64;
  }
}

void PcbcMode::decrypt(){
  std::vector<unsigned>::iterator inputIt = (*params->inputTextVect).begin();
  std::vector<unsigned> decryptedBlock(64, 0);
  xorVect = params->ivVect;

  for (int i = 0; i < this->params->numberOfBlocks; i++){
    decryptedBlock = this->cryptAlgo->decrypt(params->keyVect, inputIt);
    XOR(decryptedBlock.begin(), xorVect.begin(), decryptedBlock.begin(), 64);

    resultVect.insert(resultVect.end(), decryptedBlock.begin(), decryptedBlock.end());

    std::vector<unsigned> tempVector(inputIt, inputIt+64);
    xorVect = tempVector;

    inputIt = inputIt + 64;
  }
}
