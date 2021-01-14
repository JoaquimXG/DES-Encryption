#include <vector>
#include "./CryptParameters.h"
#include "./CryptAlgorithm.h"

#ifndef DESENCRYPTION_INCLUDE_CRYPTMODE_H_
#define DESENCRYPTION_INCLUDE_CRYPTMODE_H_

class CryptMode {
  public:
    CryptMode(CryptParameters* params, CryptAlgorithm* cryptAlgo);
    std::vector<unsigned> resultVect;
    CryptParameters* params;
    CryptAlgorithm* cryptAlgo;

    std::string toString(CryptMode& modeHandler);

    virtual void encrypt() = 0;

    virtual void decrypt() = 0;
};

class EcbMode: public CryptMode {
  public:
    EcbMode(CryptParameters* params, CryptAlgorithm* cryptAlgo);

    void encrypt() override;

    void decrypt() override;
};

#endif
