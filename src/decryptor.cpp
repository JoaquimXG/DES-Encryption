#include "../include/Options.h"
#include "../include/functions.h"
#include "../include/permutations.h"

extern std::ifstream input;
extern std::ofstream output;

int main(int argc, char *argv[]) {

  Options encryptOpts;

  int result = parseCommandLineArguments(argc, argv, &encryptOpts, true);
  switch (result) {
  case -1:
    printUsage();
    return -1;
  case -2:
    return -2;
  }

  // TODO potentially remove this
  int mode = encryptOpts.mode;
  int encryptmethod = encryptOpts.encryptmethod;
  int encrypt_size = encryptOpts.encrypt_size;
  std::string iv = encryptOpts.iv;
  std::string key = encryptOpts.key;

  unsigned IVa[64];
  charToBit(iv, IVa);

  unsigned keya[16][48];
  // gets key and generates subkeys
  generateSubKeys(key, &keya);

  // converting hex format txt file to string variable, measuring the length,
  // padding and creating a suitable length array to contain cypher text binary
  // consrtuct string varaible by iterating through file buffer
  std::string file_contents{std::istreambuf_iterator<char>(input),
                            std::istreambuf_iterator<char>()};

  // calculate the length of the last chunk of data to be encrypted and the
  // difference from 16 nibbles //padding = the number of characters needed to
  // add to bring the string up to a multiple of 8 bytes, i this case the
  // character is a hex so is worth 4 bits in padding
  int padding = (16 - file_contents.size() % 16) % 16;
  int encryption_block_num = (file_contents.size() + padding) / 16;
  unsigned cta[encryption_block_num][64];

  // converts hex string to binary array format
  hextobit(file_contents, &cta[0][0]);

  // if padding is required this loop will add padding to the end of the
  // plaintext array
  if (padding > 0) {
    // pointer to the last position in array
    unsigned *tempit = &cta[(encryption_block_num)-1][64 - 1];
    for (int i = 0; i < padding * 4; i++) {
      *tempit = 0;
      tempit--;
    }
  }

  // decryption stage

  // just for temporary storage
  unsigned workinga[2][32];
  // to store the result of the decryption
  unsigned ptbit[encryption_block_num][64];

  // block cypher modes
  if (mode == ECB || mode == CBC || mode == PCBC) {
    for (int i = 0; i < encryption_block_num; i++) {

      DES(&cta[0], &keya, &workinga, &ptbit[0], i, 0, 1);

      if (mode == CBC || mode == PCBC) {
        // xor current IV with current plaintext(which is the output of the
        // decryption)
        XOR(IVa, &ptbit[i][0], &ptbit[i][0], 64);
        // copy the pre-decryption cyphertext into the IV to be used next time
        std::copy(&cta[i][0], &cta[i][64], &IVa[0]);
        if (mode == PCBC) {
          XOR(IVa, &ptbit[i][0], IVa, 64);
        }
      }
    }
  }

  // stream cypher modes
  if (mode == CFB || mode == OFB) {
    for (int i = 0; i < 64 * encryption_block_num / encrypt_size; i++) {

      // encryption
      // encrypt current IV storing the result in the working array (Ek(IV))
      DES(&IVa, &keya, &workinga, &ptbit[0], i, 1);
      // XOR result of above with c(i-1) storing the result in the plaintext
      // array
      XOR(&workinga[0][0], &cta[0][(i * encrypt_size)],
          &ptbit[0][(i * encrypt_size)], encrypt_size);
      // IV preparation
      leftShift(IVa, 64, 8);

      for (int k = 0; k < encrypt_size; k++) {
        if (mode == CFB) {
          // append the most recent 8 bits of the cypher text array to the end
          // of the IV ready for the next stage of encryption
          IVa[63 - k] = cta[0][(i * encrypt_size) + ((encrypt_size - 1) - k)];
        } else {
          IVa[63 - k] = workinga[0][encrypt_size - 1 - k];
        }
      }
    }
  }

  // outputting results
  // 2 hex characters = one (char) so the output file is half the size of the
  // input
  char output[file_contents.size() / 2];
  // iterate through the results array (ptbit) and conver each 8 bit section
  // into a char
  for (int i = 0; i < file_contents.size() + padding; i++) {
    char tempc = (char)binarytodecimal(&ptbit[0][i * 8], 8);
    output[i] = tempc;
  }
  arrayout(output, file_contents.size() / 2, 500000, 1);

  return 0;
}
