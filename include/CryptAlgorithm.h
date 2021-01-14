#ifndef DESENCRYPTION_INCLUDE_CRYPTALGORITHM_H_
#define DESENCRYPTION_INCLUDE_CRYPTALGORITHM_H_

class CryptAlgorithm {
  public:
    virtual void encrypt() = 0;

    virtual void decrypt() = 0;
};
#endif
