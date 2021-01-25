#include "../include/CryptMode.h"
#include "../include/cryptUtils.h"
#include <sstream>
#include <iostream>

CryptMode::CryptMode(CryptParameters* params, CryptOption* opt)
    : resultVect(), params(params){};


std::vector<unsigned> CryptMode::resultToDecimal(){
  std::vector<unsigned> output;

  std::vector<unsigned>::iterator it = this->resultVect.begin();
  for (int i = 0; i < this->resultVect.size()/8; i++) {
    unsigned decimalDigit = binToDec(it, 8);
    output.push_back(decimalDigit);
    it = it + 8;
  }

  return output;
}

std::string CryptMode::toString(){
  std::stringstream returnString;
  returnString << binaryVectorToString(resultVect)<< "\n";
  return returnString.str();
}
