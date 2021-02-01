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
  std::vector<unsigned> ciphertextBlock(64, 0);
  xorVect = params->ivVect;

  for (int i = 0; i < this->params->numberOfBlocks; i++){
    XOR(inputIt, xorVect.begin(), xorVect.begin(), 64);

    ciphertextBlock = this->cryptAlgo->encrypt(params->keyVect, xorVect.begin());
    resultVect.insert(resultVect.end(), ciphertextBlock.begin(), ciphertextBlock.end());

    std::vector<unsigned> plaintextBlock(inputIt, inputIt+64);
    XOR(plaintextBlock, ciphertextBlock, xorVect, 64);

    inputIt = inputIt + 64;
  }
}

void PcbcMode::decrypt(){
  std::vector<unsigned>::iterator inputIt = (*params->inputTextVect).begin();
  std::vector<unsigned> plaintextBlock(64, 0);
  xorVect = params->ivVect;

  for (int i = 0; i < this->params->numberOfBlocks; i++){
    plaintextBlock = this->cryptAlgo->decrypt(params->keyVect, inputIt);
    XOR(plaintextBlock.begin(), xorVect.begin(), plaintextBlock.begin(), 64);

    resultVect.insert(resultVect.end(), plaintextBlock.begin(), plaintextBlock.end());

    std::vector<unsigned> ciphertextBlock(inputIt, inputIt+64);
    XOR(ciphertextBlock, plaintextBlock, xorVect, 64);

    inputIt = inputIt + 64;
  }
}
