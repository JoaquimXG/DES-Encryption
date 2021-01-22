#include "./CryptAlgorithm.h"
#include <vector>

#ifndef DESENCRYPTION_INCLUDE_DESALGORITHM_H_
#define DESENCRYPTION_INCLUDE_DESALGORITHM_H_

class DesAlgorithm: public CryptAlgorithm {
    //TODO Document Feistal function
    std::vector<unsigned> feistal(std::vector<unsigned>& input, std::vector<unsigned>& key);
    //TODO Document DESEncrypt function
    std::vector<unsigned> DES(std::vector<std::vector<unsigned>>& keyGroup, std::vector<unsigned>::const_iterator plaintextIterator, bool decrypt=false);

  public:
    std::vector<unsigned> encrypt(std::vector<std::vector<unsigned>>& keyGroup, std::vector<unsigned>::const_iterator plaintextIterator);

    std::vector<unsigned> decrypt(std::vector<std::vector<unsigned>>& keyGroup, std::vector<unsigned>::const_iterator plaintextIterator);
};
#endif
