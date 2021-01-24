#include <vector>
#include "./CryptParameters.h"
#include "./CryptAlgorithm.h"

#ifndef DESENCRYPTION_INCLUDE_CRYPTMODE_H_
#define DESENCRYPTION_INCLUDE_CRYPTMODE_H_

class CryptMode {
  public:

  enum Mode { ECB, CBC, PCBC, CFB, OFB };

    CryptMode(CryptParameters* params, CryptAlgorithm* cryptAlgo);
    std::vector<unsigned> resultVect;
    CryptParameters* params;
    CryptAlgorithm* cryptAlgo;

    std::string toString();

    virtual void encrypt() = 0;

    virtual void decrypt() = 0;

    /*
     * Converts the currently stored resultsVect from binary to decimal ready for printing in hex.
     *
     * Loops through the resultsVect converting each 8 bit section into a decimal integer.
     * Each integer is added to a new vector and the vector is returned once all sections have been converted.
     *
     * @return std::vector<unsigned> A vector containing the resultsVect represented in decimal.
     */
    std::vector<unsigned> resultToDecimal();
};

class EcbMode: public CryptMode {
  public:
    EcbMode(CryptParameters* params, CryptAlgorithm* cryptAlgo);

    void encrypt() override;

    void decrypt() override;
};

#endif
