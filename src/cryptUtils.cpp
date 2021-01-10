#include <bitset>
#include "../include/cryptUtils.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


//TODO define constants for things like height and width

void leftShift(std::vector<unsigned>& inVector, int shiftSize) {
  int vectorLength = inVector.size();
  if (shiftSize >= vectorLength) {
    shiftSize = shiftSize % vectorLength;
  }

  int hold[shiftSize];
  for (int i = 0; i < vectorLength; i++) {
    if (i<shiftSize){
      hold[i] = inVector[i];
    }
    else {
      inVector[i-shiftSize] = inVector[i];
    }
  }

  int offset = vectorLength - shiftSize;
  for (int i = 0; i < shiftSize; i++) {
    inVector[offset + i] = hold[i];
  }
}

bool leftShift(std::vector<unsigned>& inVector, int shiftSize, int startPosition, int endPosition) {
  if (startPosition >= endPosition || endPosition <= startPosition || startPosition < 0 || endPosition > inVector.size()-1){
    return false;
  }

  int workingVectorLength = endPosition - startPosition + 1;
  if (shiftSize >= workingVectorLength) {
    shiftSize = shiftSize % workingVectorLength;
  }

  int hold[shiftSize];
  for (int i = 0; i < workingVectorLength; i++) {
    if (i<shiftSize){
      hold[i] = inVector[i + startPosition];
    }
    else {
      inVector[i-shiftSize + startPosition] = inVector[i + startPosition];
    }
  }

  int offset = workingVectorLength - shiftSize;
  for (int i = 0; i < shiftSize; i++) {
    inVector[offset + i + startPosition] = hold[i];
  }
  
  return true;
}

// TODO, could this function not just return the bitset
int charToBit(std::string inString, std::vector<unsigned>& outVector) {
  int stringLength = inString.size();
  for (int i = 0; i < stringLength; i++) {

    std::bitset<8> tempBitset(inString.c_str()[i]);
    for (int j = 0; j < 8; j++) {
      outVector[(i*8)+j] = tempBitset[8-1-j];
    }
  }
  return 0;
}

// TODO, could this function not just return the bitset
// This is for 2D vectors, hopefully I won't need this
int charToBit(std::string inString, std::vector<std::vector<unsigned>>& outVector) {
  int stringLength = inString.size();
  for (int i = 0; i < stringLength; i++) {
    std::bitset<8> tempBitset(inString.c_str()[i]);
    for (int k = 7; k > -1; k--) {
      outVector[i/8][((i*8)+k)%64] = tempBitset[k];
    }
  }
  return 0;
}

std::string vectorToString(std::vector<unsigned>& vect){
  std::stringstream returnString;
  for (auto i = vect.begin(); i != vect.end(); ++i){
      if (*i == 1){
        returnString << "\033[34m" << *i << "\033[0m" << "";
      } else {
        returnString << "\033[31m" << *i << "\033[0m" << "";
      }
  }
  return returnString.str();
};

std::string vectorToString2D(std::vector<std::vector<unsigned>>& vect){
  std::stringstream returnString;
  for (auto i = vect.begin(); i != vect.end(); ++i) 
  {
    for (std::vector<unsigned>::const_iterator j = (*i).begin(); j != (*i).end(); ++j) 
    {
      if (*j == 1){
        returnString << "\033[34m" << *j << "\033[0m" << "";
      } else {
        returnString << "\033[31m" << *j << "\033[0m" << "";
      }
    }
    returnString << "\n";
  }
  return returnString.str();
};

