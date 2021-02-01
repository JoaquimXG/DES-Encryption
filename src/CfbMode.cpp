#include "../include/CryptMode.h"
#include "../include/DesAlgorithm.h"
#include "../include/cryptUtils.h"
#include <vector>
#include <iostream>


//TODO allow block size to be set through command line arguments
int BLOCKSIZE = 8;

CfbMode::CfbMode(CryptParameters* params, CryptOption* opt)
    : CryptMode(params, opt), xorVect(64, 0){
        this->cryptAlgo = getAlgorithm(opt->cryptMethod);
    };

void CfbMode::encrypt(){
  std::vector<unsigned>::iterator inputIt = (*params->inputTextVect).begin();
  std::vector<unsigned> ciphertextBlock(64, 0);
  xorVect = params->ivVect;

  //TODO calculate number of blocks before we get to this function
  for (int i = 0; i < (this->params->numberOfBlocks * 64/BLOCKSIZE); i++){
    ciphertextBlock = this->cryptAlgo->encrypt(params->keyVect, xorVect.begin());

    XOR(inputIt, ciphertextBlock.begin(), ciphertextBlock.begin(), BLOCKSIZE);

    resultVect.insert(resultVect.end(), ciphertextBlock.begin(), ciphertextBlock.begin() + BLOCKSIZE);

    leftShift(xorVect, BLOCKSIZE % 64);
    std::copy(ciphertextBlock.begin(), ciphertextBlock.begin()+BLOCKSIZE, xorVect.end()-BLOCKSIZE);

    inputIt = inputIt + BLOCKSIZE;
  }
}

void CfbMode::decrypt(){
  std::vector<unsigned>::iterator inputIt = (*params->inputTextVect).begin();
  std::vector<unsigned> plaintextBlock(64, 0);
  xorVect = params->ivVect;

  for (int i = 0; i < (this->params->numberOfBlocks * 64/BLOCKSIZE); i++){
    plaintextBlock = this->cryptAlgo->encrypt(params->keyVect, xorVect.begin());
    XOR(inputIt, plaintextBlock.begin(), plaintextBlock.begin(), BLOCKSIZE);

    resultVect.insert(resultVect.end(), plaintextBlock.begin(), plaintextBlock.begin()+BLOCKSIZE);

    leftShift(xorVect, BLOCKSIZE % 64);
    std::copy(inputIt, inputIt+BLOCKSIZE, xorVect.end()-BLOCKSIZE);

    inputIt = inputIt + BLOCKSIZE;
  }
}
