#include "../include/CryptMode.h"
#include "../include/cryptUtils.h"
#include <sstream>

CryptMode::CryptMode(CryptParameters* params, CryptAlgorithm* cryptAlgo)
    : resultVect(params-> numberOfBlocks * 64, 0), params(params), cryptAlgo(cryptAlgo){};

std::string CryptMode::toString(CryptMode& modeHandler){
  std::stringstream returnString;
  returnString << vectorToString(modeHandler.resultVect);
  return returnString.str();
}
