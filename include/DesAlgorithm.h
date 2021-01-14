#include "./CryptAlgorithm.h"

#ifndef DESENCRYPTION_INCLUDE_DESALGORITHM_H_
#define DESENCRYPTION_INCLUDE_DESALGORITHM_H_

class DesAlgorithm: public CryptAlgorithm {
  public:
    void encrypt() override;

    void decrypt() override;
};
#endif
