#include <bitset>
#include "../include/cryptUtils.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>


//TODO define constants for things like height and width

std::string getFileContents(std::string fileName){
  std::ifstream fileStream;
  std::string dumpString = "";

  std::cout << "[+] Opening File: "<< fileName << "\n";
  fileStream.open(fileName);
  if (!fileStream.is_open()) {
    std::cout << "\n[-] ------------ Can't open input file ------------\n";
    return dumpString;
  }

  dumpString = std::string{std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>()};

  return dumpString;
}

bool openOutFile(std::string fileName, std::ofstream* fileStream){
  std::cout << "[+] Opening File: "<< fileName << "\n";
  (*fileStream).open(fileName);
  if (!(*fileStream).is_open()) {
    std::cout << "\n[-] ------------ Can't open output file ------------";
    return false;
  }
  return true;
}

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

//TODO, this will need an offset if it is ever to operate on the middle of an array
void XOR(std::vector<unsigned>& firstInput, std::vector<unsigned>& secondInput, std::vector<unsigned>& result, int size) {
  for (int i = 0; i < size; i++) {
    if (firstInput[i] == secondInput[i]) {
      result[i] = 0;
    } else {
      result[i] = 1;
    }
  }
}

int binToDec(std::vector<unsigned>& input, int size) {
  int result = 0;
  for (int i = size; i > 0; i--) {
    result += input[i - 1] * pow(2, size - i);
  }
  return result;
}

int binToDec(std::vector<unsigned>::iterator start, int size) {
  int result = 0;
  for (int i = size; i > 0; i--) {
    result += *(start+i-1) * pow(2, size - i);
  }
  return result;
}

std::vector<unsigned> decToBin(int integer, int size) {
  std::vector<unsigned> output(size);
  for (int i = size; i > 0; i--) {
    output[i - 1] = integer % 2;
    integer /= 2;
  }
  return output;
}

std::string binaryVectorToString(std::vector<unsigned>& vect){
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

std::string hexVectorToString(std::vector<unsigned>& vect){
  std::stringstream returnString;
  for (auto i = vect.begin(); i != vect.end(); ++i){
      returnString << std::hex << std::setfill('0') << std::setw(2) << *i;
  }
  return returnString.str();
};

std::string binaryVectorToString2D(std::vector<std::vector<unsigned>>& vect){
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
