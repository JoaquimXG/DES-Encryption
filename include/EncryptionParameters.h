#include <string>
#include <vector>

#ifndef DESENCRYPTION_INCLUDE_ENCRYPTIONPARAMETERS_H_
#define DESENCRYPTION_INCLUDE_ENCRYPTIONPARAMETERS_H_

class EncryptionParameters {
  private:



  public:
    EncryptionParameters(std::string iv, std::string key);

    //IV converted to binary
    unsigned ivArray[64];
    //Sixteen 48 bit subkeys for DES
    unsigned keyArray[16][48];

    int numberOfBlocks;
    int padding;
    int fileLength;

    //std::vector<unsigned> plainTextArray;
    unsigned* plainTextArray[64];

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

    //TODO WRITE FULL DOC STRING
    int parseInputFile(std::string inputFileName);

};
#endif
