#include "./CryptAlgorithm.h"
#include <vector>

#ifndef DESENCRYPTION_INCLUDE_DESALGORITHM_H_
#define DESENCRYPTION_INCLUDE_DESALGORITHM_H_

class DesAlgorithm: public CryptAlgorithm {
  private:
    /*
     * Performs the feistal function on a given 32bit Vector using a given key.
     *
     * 32 bit input is expanded to 48 bits using the feistal expansion lookup table.
     * result is XOR'd with the 48 bit key.
     * result is split into 8 sections, each 6 bits.
     * Each 6 bit section is converted into a decimal integer between 1 and 0 using a substitution box.
     * first and last bits are used to determine the row for the box and the remaining 4 middle bits
     * are used to determing the column.
     * A different substitution box is used for each section.
     * resulting decimal integer is then converted back into binary.
     * Resulting in 8 sections of 4 bit binary for a total of 32 bits.
     * resulting 32 bits are permutated using a lookup box (lookupFeistalExpansion).
     * result is returned.
     *
     * @param input A reference to a 32 bit vector containing only binary data.
     * @param key A 48 bit DES key.
     * @return vector<unsigned> A 32 bit binary vector containing the result of the feistal operation.
     */
    std::vector<unsigned> feistal(std::vector<unsigned>& input, std::vector<unsigned>& key);

    /*
     * Performs the DES encryption algorithm on a given 64bit binary vector using 16 48bit DES keys.
     *
     * 64bit binary vector is put through a permutation using a lookup table (lookupInitalPermutation).
     * For convenience the vector is split into 2 32bit vectors a left half and a right half.
     * For each of the 64 bit keys:
     *   Feistal function performed on the right 32 bits
     *   Result XOR'd with left 32 bits
     *   left and right are swapped
     *   For decryption the keys are run in reverse
     * Left and right are merged.
     * result is put through a final permutation using lookup table (lookupFinalPermutation).
     * result is returned.
     *
     * @param keyGroup A 16 x 48 2D vector containing 16 48 bit DES keys
     * @param plaintextIterator An iterator pointing to the start of the plain text array block to be encrypted.
     * @param bool A flag to handle whether to run the algorithm in decryption mode. If true function will decrypt.
     * @return std::vector<unsigned> A vector containing the result of the DES algorithm.
     */
    std::vector<unsigned> DES(std::vector<std::vector<unsigned>>& keyGroup, std::vector<unsigned>::const_iterator plaintextIterator, bool decrypt=false);

  public:
    /*
     * Performs encryption using the DES algorithm
     *
     * @param keyGroup A 16 x 48 2D vector containing 16 48 bit DES keys
     * @param plaintextIterator An iterator pointing to the start of the plain text array block to be encrypted.
     * @return std::vector<unsigned> A vector containing the result of the DES algorithm.
     */
    std::vector<unsigned> encrypt(std::vector<std::vector<unsigned>>& keyGroup, std::vector<unsigned>::const_iterator plaintextIterator);

    /*
     * Performs decryption using the DES algorithm
     *
     * @param keyGroup A 16 x 48 2D vector containing 16 48 bit DES keys
     * @param plaintextIterator An iterator pointing to the start of the plain text array block to be encrypted.
     * @return std::vector<unsigned> A vector containing the result of the DES algorithm.
     */
    std::vector<unsigned> decrypt(std::vector<std::vector<unsigned>>& keyGroup, std::vector<unsigned>::const_iterator plaintextIterator);
};
#endif
