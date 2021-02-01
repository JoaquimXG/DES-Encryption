#include "../include/DesAlgorithm.h"
#include "../include/permutations.h"
#include "../include/cryptUtils.h"

std::vector<unsigned> DesAlgorithm::encrypt(std::vector<std::vector<unsigned>>& keyGroup, std::vector<unsigned>::const_iterator plaintextIterator){
  return DES(keyGroup, plaintextIterator, false);
}

std::vector<unsigned> DesAlgorithm::decrypt(std::vector<std::vector<unsigned>>& keyGroup, std::vector<unsigned>::const_iterator plaintextIterator){
  return DES(keyGroup, plaintextIterator, true);
}

std::vector<unsigned> DesAlgorithm::DES(std::vector<std::vector<unsigned>>& keyGroup, std::vector<unsigned>::const_iterator plaintextIterator, bool decrypt){
  std::vector<unsigned> initialLeftHalf(32);
  std::vector<unsigned> initialRightHalf(32);

  for (int i = 0; i < 32; i++) {
    initialLeftHalf[i] = *(plaintextIterator + lookupInitalPermutation[i]);
  }
  for (int i = 0; i < 32; i++) {
    initialRightHalf[i] = *(plaintextIterator + lookupInitalPermutation[i+32]);
  }

  std::vector<unsigned>* feistalHalf = &initialRightHalf;
  std::vector<unsigned>* xorHalf = &initialLeftHalf;
  std::vector<unsigned> feistalResult(32);
  for (int k = 0; k < 16; k++) {
    if (decrypt){
      feistalResult = feistal(*feistalHalf, keyGroup[15-k]);
    }else{
      feistalResult = feistal(*feistalHalf, keyGroup[k]);
    }

    XOR(feistalResult, *xorHalf, *xorHalf, 32);

    //Swapping left half with right half
    std::vector<unsigned>* tempPointer = feistalHalf;
    feistalHalf = xorHalf;
    xorHalf = tempPointer;
  }

  initialRightHalf.insert(initialRightHalf.end(), initialLeftHalf.begin(), initialLeftHalf.end());
  std::vector<unsigned> returnVect(64);
  for (int i = 0; i < 64; i++) {
    returnVect[i] = initialRightHalf[lookupFinalPermutation[i]];
  }

  return returnVect;
}


std::vector<unsigned> DesAlgorithm::feistal(std::vector<unsigned>& input, std::vector<unsigned>& key){
  std::vector<unsigned> expandedInput(48);
  std::vector<unsigned> prePermutationP(32);
  std::vector<unsigned> result(32);

  for (int i = 0; i < 48; i++) {
    expandedInput[i] = input[lookupFeistalExpansion[i]];
  }

  //XOR with key
  XOR(expandedInput, key, expandedInput, 48);

  //eight 6 bit sections passed through s-box
  //First and last digits used for the row number
  //middle four digits used for the column number
  //result of s-box is converted back to binary
  //result is always 4 bits
  for (int i = 0; i < 8; i++) {
    int offset = i*6;

    std::vector<unsigned> rowBinary = {expandedInput[offset+0], expandedInput[offset+5]};
    std::vector<unsigned> columnBinary = {expandedInput[offset+1], expandedInput[offset+2], expandedInput[offset+3], expandedInput[offset+4]};

    int rowInt = binToDec(rowBinary, 2);
    int columnInt = binToDec(columnBinary, 4);

    int sBoxResult = lookupSBox[i][rowInt][columnInt];

    std::vector<unsigned> binarySBoxResult = decToBin(sBoxResult, 4);

    // iterate through the temporary binary array and add to temporary array
    for (int j = 0; j < 4; j++) {
      prePermutationP[i * 4 + j] = binarySBoxResult[j];
    }
  }

  for (int i = 0; i < 32; i++) {
    result[i] = prePermutationP[lookupFeistalPermutation[i]];
  }

  return result;
}
