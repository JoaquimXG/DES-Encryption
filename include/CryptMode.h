#ifndef DESENCRYPTION_INCLUDE_CRYPTMODE_H_
#define DESENCRYPTION_INCLUDE_CRYPTMODE_H_
#include <vector>
#include "./CryptParameters.h"
#include "./CryptAlgorithm.h"
#include "./CryptOption.h"

class CryptMode {
  public:

  enum Mode { ECB, CBC, PCBC, CFB, OFB };

    CryptMode(CryptParameters* params, CryptOption* opt);
    std::vector<unsigned> resultVect;
    CryptParameters* params;
    CryptAlgorithm* cryptAlgo;


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

    /*
     * Converts the value of the binary results Vector to a string.
     *
     * @return std::string A formatted string representation of the resultsVect.
     */
    std::string toString();
};

class EcbMode: public CryptMode {
  public:
    EcbMode(CryptParameters* params, CryptOption* opt);

    void encrypt() override;

    void decrypt() override;
};

#endif
