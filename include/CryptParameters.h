#include <string>
#include <vector>

#ifndef DESENCRYPTION_INCLUDE_CRYPTPARAMETERS_H_
#define DESENCRYPTION_INCLUDE_CRYPTPARAMETERS_H_

class CryptParameters {
  public:
    std::vector<unsigned> ivVect;
    std::vector<std::vector<unsigned>> keyVect;
    std::vector<unsigned>* inputTextVect;

    int inputTextLength;
    int numberOfBlocks;
    int padding;

    /* Constructor 
     * @param inputTextLength The length of the plaintext
     */
    CryptParameters(int inputTextLength);

    /* 
     * Generates 16 48bit subkeys for DES from a 64 bit key.
     * The 64 bit key is first converted from characters to binary.
     * 56 bits are then selected from the key using the Permuted Choice 1 table
     * This 56 bit key is split in half into two 28 bit keys.
     * For 16 rounds,
     * Each 28 bit key is left shifted either 1 or 2 bits in a predefined order
     * as defined by bitRotationTable.
     * 48 bits are then selected from a combination of the 28 bit keys using 
     * the Permuted Choice 2 table to form the each 48 bit key.
     *
     * @param &key A reference to std::string for the key input
     * @param params An EncryptionParameters objec which will hold keys and IV information
     */
    void generateSubKeys(std::string &key);

    /*
     * Converts the input string into a binary representation.
     *
     * First calculates the amount of padding required.
     * Then the nubmer of encryption blocks.
     * Creates a Vector of the appropriate size.
     * Converts each character to 8bit binary and places into the new Vector using charToBit.
     * Fills in the end of the array with the required padding.
     *
     * @param inFileString A string to be parsed and prepared for encryption.
     */
    void parseInputFile(std::string inputFileName);

    /*
     * Prints the IV, Key and Plain text vectors as well as all other variables in the class.
     * 
     * @return String A formatted string containing all values in the class.
     */
    std::string toString();
};
#endif
