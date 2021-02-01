#include "../include/CryptMode.h"
#include "../include/DesAlgorithm.h"
#include "../include/cryptUtils.h"
#include <vector>
#include <iostream>


//TODO allow block size to be set through command line arguments
int MYBLOCKSIZE = 8;

OfbMode::OfbMode(CryptParameters* params, CryptOption* opt)
    : CryptMode(params, opt), xorVect(64, 0){
        this->cryptAlgo = getAlgorithm(opt->cryptMethod);
    };

void OfbMode::encrypt(){
  std::vector<unsigned>::iterator inputIt = (*params->inputTextVect).begin();
  std::vector<unsigned> ciphertextBlock(64, 0);
  xorVect = params->ivVect;

  //TODO calculate number of blocks before we get to this function
  for (int i = 0; i < (this->params->numberOfBlocks * 64/MYBLOCKSIZE); i++){
    ciphertextBlock = this->cryptAlgo->encrypt(params->keyVect, xorVect.begin());

    leftShift(xorVect, MYBLOCKSIZE % 64);
    std::copy(ciphertextBlock.begin(), ciphertextBlock.begin()+MYBLOCKSIZE, xorVect.end()-MYBLOCKSIZE);

    XOR(inputIt, ciphertextBlock.begin(), ciphertextBlock.begin(), MYBLOCKSIZE);

    resultVect.insert(resultVect.end(), ciphertextBlock.begin(), ciphertextBlock.begin() + MYBLOCKSIZE);

    inputIt = inputIt + MYBLOCKSIZE;
  }
}

void OfbMode::decrypt(){
  this->encrypt();
}
