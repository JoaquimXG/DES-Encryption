#include "../include/CryptMode.h"
#include "../include/DesAlgorithm.h"
#include "../include/cryptUtils.h"
#include <vector>
#include <iostream>

CfbMode::CfbMode(CryptParameters* params, CryptOption* opt)
    : CryptMode(params, opt), xorVect(64, 0), cryptSize(opt->cryptSize){
        this->cryptAlgo = getAlgorithm(opt->cryptMethod);
    };

void CfbMode::encrypt(){
  std::vector<unsigned>::iterator inputIt = (*params->inputTextVect).begin();
  std::vector<unsigned> ciphertextBlock(64, 0);
  xorVect = params->ivVect;

  for (int i = 0; i < (this->params->numberOfBlocks * 64/this->cryptSize); i++){
    ciphertextBlock = this->cryptAlgo->encrypt(params->keyVect, xorVect.begin());

    XOR(inputIt, ciphertextBlock.begin(), ciphertextBlock.begin(), this->cryptSize);

    resultVect.insert(resultVect.end(), ciphertextBlock.begin(), ciphertextBlock.begin() + this->cryptSize);

    leftShift(xorVect, this->cryptSize % 64);
    std::copy(ciphertextBlock.begin(), ciphertextBlock.begin()+this->cryptSize, xorVect.end()-this->cryptSize);

    inputIt = inputIt + this->cryptSize;
  }
}

void CfbMode::decrypt(){
  std::vector<unsigned>::iterator inputIt = (*params->inputTextVect).begin();
  std::vector<unsigned> plaintextBlock(64, 0);
  xorVect = params->ivVect;

  for (int i = 0; i < (this->params->numberOfBlocks * 64/this->cryptSize); i++){
    plaintextBlock = this->cryptAlgo->encrypt(params->keyVect, xorVect.begin());
    XOR(inputIt, plaintextBlock.begin(), plaintextBlock.begin(), this->cryptSize);

    resultVect.insert(resultVect.end(), plaintextBlock.begin(), plaintextBlock.begin()+this->cryptSize);

    leftShift(xorVect, this->cryptSize % 64);
    std::copy(inputIt, inputIt+this->cryptSize, xorVect.end()-this->cryptSize);

    inputIt = inputIt + this->cryptSize;
  }
}
