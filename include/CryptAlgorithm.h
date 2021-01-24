#ifndef DESENCRYPTION_INCLUDE_CRYPTALGORITHM_H_
#define DESENCRYPTION_INCLUDE_CRYPTALGORITHM_H_
#include <vector>

class CryptAlgorithm {
  public:
    virtual std::vector<unsigned> encrypt(std::vector<std::vector<unsigned>>& keyGroup, std::vector<unsigned>::const_iterator plaintextIterator) = 0;

    virtual std::vector<unsigned> decrypt(std::vector<std::vector<unsigned>>& keyGroup, std::vector<unsigned>::const_iterator plaintextIterator) = 0;
};
#endif
