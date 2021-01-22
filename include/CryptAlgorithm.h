#ifndef DESENCRYPTION_INCLUDE_CRYPTALGORITHM_H_
#define DESENCRYPTION_INCLUDE_CRYPTALGORITHM_H_
#include <vector>

class CryptAlgorithm {
  public:
    virtual std::vector<unsigned> encrypt() = 0;

    virtual std::vector<unsigned> decrypt() = 0;
};
#endif
