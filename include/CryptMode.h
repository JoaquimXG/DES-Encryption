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

    /*
     * Gets the algorithm that should currently be in use.
     *
     * Currently only one algorithm is supported but this is preparing for extension in the future.
     *
     * @param cryptMethod The method of encryption(algorithm) enum that was passed as an argument.
     */
    static CryptAlgorithm* getAlgorithm(int cryptMethod){
        CryptAlgorithm* cryptAlgo;
        switch(cryptMethod){
            case DES:
                cryptAlgo = new DesAlgorithm();
        }
        return cryptAlgo;
    }
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

/*
 * CBC Mode of encryption
 *
 * No preprocessing or post processing is required.
 *
 * The encryption and decryption algorithms are not symmetrical.
 * This is because during the encryption process the the plainText is XOR'd initially with 
 * the IV for the first round, then with the preceeding ciphertext for remaining rounds.
 *
 * This process requires that during decryption, the first decrypted block must be XOR'd with 
 * the IV before decryption occurs to mirror the encryption. 
 * Remiaining blocks require to be XOR'd with the preceeding cypher text before decryption.
 * This allows for the decryption in CBC to be parallelized as all the information required to 
 * each block is available before decryption begins.
 * Encryption must be performed sequentially as each block requires the previous ciphertext.
 *
 */
class CbcMode: public CryptMode {
    std::vector<unsigned> xorVect;
  public:
    CbcMode(CryptParameters* params, CryptOption* opt);

    /*
     * Encrypts the entire input
     *
     * Loops through each block of plaintext.
     * XORs the block with the IV for the first round, with the previous ciphertext
     * for remaining rounds.
     * Resulting of XOR is encrypted using the given encryption algorithm.
     * The cyphertext block is stored to use for XOR in the next round.
     *
     */
    void encrypt() override;

    /*
     * Decrypts the entire input
     *
     * Loops through each block of ciphertext.
     * Runs the block through the provided decryption algorithm.
     * XORs result of decryption initially with the IV then with the previous ciphertext for remaining rounds.
     *
     * This version stores the current ciphertext for use in XOR in the next round.
     * This is just for convenience, if this algorithm were to be parallelized then each round should retrieve
     * either the IV or previous ciphertext as required.
     *
     */
    void decrypt() override;
};

/*
 * PCBC Mode of encryption
 *
 * //TODO rewrite Documentation
 * No preprocessing or post processing is required.
 *
 * The encryption and decryption algorithms are not symmetrical.
 * This is because during the encryption process the the plainText is XOR'd initially with 
 * the IV for the first round, then with the preceeding ciphertext for remaining rounds.
 *
 * This process requires that during decryption, the first decrypted block must be XOR'd with 
 * the IV before decryption occurs to mirror the encryption. 
 * Remiaining blocks require to be XOR'd with the preceeding cypher text before decryption.
 * This allows for the decryption in CBC to be parallelized as all the information required to 
 * each block is available before decryption begins.
 * Encryption must be performed sequentially as each block requires the previous ciphertext.
 *
 */
class PcbcMode: public CryptMode {
    std::vector<unsigned> xorVect;
  public:
    PcbcMode(CryptParameters* params, CryptOption* opt);

    /*
     * Encrypts the entire input
     * //TODO rewrite Documentation
     *
     * Loops through each block of plaintext.
     * XORs the block with the IV for the first round, with the previous ciphertext
     * for remaining rounds.
     * Resulting of XOR is encrypted using the given encryption algorithm.
     * The cyphertext block is stored to use for XOR in the next round.
     *
     */
    void encrypt() override;

    /*
     * Decrypts the entire input
     * //TODO rewrite Documentation
     *
     * Loops through each block of ciphertext.
     * Runs the block through the provided decryption algorithm.
     * XORs result of decryption initially with the IV then with the previous ciphertext for remaining rounds.
     *
     * This version stores the current ciphertext for use in XOR in the next round.
     * This is just for convenience, if this algorithm were to be parallelized then each round should retrieve
     * either the IV or previous ciphertext as required.
     *
     */
    void decrypt() override;
};

#endif
