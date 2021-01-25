#ifndef DESENCRYPTION_INCLUDE_CRYPTMODE_H_
#define DESENCRYPTION_INCLUDE_CRYPTMODE_H_
#include <vector>
#include "./CryptParameters.h"
#include "./CryptAlgorithm.h"
#include "./DesAlgorithm.h"
#include "./CryptOption.h"

/*
 * Parent class for all Encryption mode classes.
 *
 * Presents methods used to allow for encryption and decryption to be performed
 * by any suitable implementaion algorithm.
 */
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

/*
 * ECB Mode of encryption
 *
 * Both encryption and decryption, ECB mode simply loops through each 64 bit section of input
 * and performs the selected encryption algorithms encrypt or decrypt function accordingly.
 *
 * No preprocessing or post processing is required.
 *
 */
class EcbMode: public CryptMode {
  public:
    EcbMode(CryptParameters* params, CryptOption* opt);

    /*
     * Encrypts the entire input
     *
     * Runs ecbStructure, pasing in the selected encryption algorithms encrypt function.
     */
    void encrypt() override;

    /*
     * Decrypts the entire input
     *
     * Runs ecbStructure, pasing in the selected encryption algorithms decrypt function.
     */
    void decrypt() override;

    /*
     * Loops through each block of input and runs either an encryption or decrypt function on the block.
     *
     * The encryption or decryption function is passed as a parameter through EcbMode::encrypt or EcbMode::decrypt.
     *
     * The syntax can be broken down as 
     * std::vector<unsigned> - The return value of the passed function
     * CryptAlgorithm::*crypt - A pointer to a member function of the CryptAlgorithm class. 
     * (std::vector<std::vector<unsigned>>& keyGroup, std::vector<unsigned>::const_iterator plaintextIterator) - The arguments required for the passed function.
     *
     * The function that is passed must be a class member function of the CryptAlgorithm class or its descendants and must have the same signature as is defined here.
     *
     * @param An encryption or decryption class member function from the CryptAlgorithm class family.
     *
     */
    void ecbStructure(std::vector<unsigned> (CryptAlgorithm::*crypt)(std::vector<std::vector<unsigned>>& keyGroup, std::vector<unsigned>::const_iterator plaintextIterator));
};

#endif
