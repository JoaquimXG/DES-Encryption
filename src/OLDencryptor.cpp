#include "../include/OLDOptions.h"
#include "../include/OLDfunctions.h"
#include "../include/OLDpermutations.h"
#include <iomanip>

extern std::ofstream output;
extern std::ifstream input;

int main(int argc, char *argv[]) {

  Options encryptOpts;

  int result = parseCommandLineArguments(argc, argv, &encryptOpts);
  switch (result) {
  case -1:
    printUsage();
    return -1;
  case -2:
    return -2;
  }

  int mode = encryptOpts.mode;
  int encryptmethod = encryptOpts.encryptmethod;
  int encrypt_size = encryptOpts.encrypt_size;

  unsigned IVa[64];
  charToBit(encryptOpts.iv, IVa);

  unsigned keya[16][48];
  generateSubKeys(encryptOpts.key, &keya);

  // Consrtuct string varaible by iterating through file buffer
  std::string inputFileString{std::istreambuf_iterator<char>(input),
                            std::istreambuf_iterator<char>()};

  // calculate the length of the last chunk of data to be encrypted and the
  // difference from 8 bytes
  int fileLength = inputFileString.size();
  int padding = (8 - (fileLength % 8)) % 8;
  int encryption_block_num = (fileLength + padding) / 8;
  unsigned pta[encryption_block_num][64];

  std::cout << "Should be converting string here ----------------------------------------------";
  charToBit(inputFileString, &pta[0][0]);

  if (padding > 0) {
    // pointer to the last position in array
    unsigned *tempit = &pta[(encryption_block_num)-1][64 - 1];
    for (int i = 0; i < padding * 8; i++) {
      *tempit = 0;
      tempit--;
    }
  }


  // Text encryption
  // cypher text binary output to be converted to hex or ascii
  unsigned ctbit[encryption_block_num][64];

  // working array for temporary data
  unsigned w_array[64];

  // working array for inside DES function, also used for storing the result of
  // DES when the result can't be saved to ctbit
  unsigned ct[2][32];

  // iterate through the data 8 bytes at a time, add is the padding required so
  // the string variable is a multiple of 8 bytes
  for (int i = 0; i < encryption_block_num; i++) {

    // operations performed before any encryption
    if (i == 0) {
      // XOR plaintext with IV before any encyption (currently using for CBC and
      // PCBC)
      if (mode == CBC || mode == PCBC) {
        std::copy(&pta[i][0], &pta[i][64], &w_array[0]);
        XOR(&pta[i][0], &IVa[0], &pta[i][0], 64);
      }
      // IV encrypted using DES
      else if (mode == CFB || mode == OFB) {
        // iterate through each encryption block that is a portion of the
        // current block(64) if encrypt size is 64 then this will only go
        // through a single loop if e_size is 16 then it will loop 4 times
        // (16*4)=64
        for (int k = 0; k < (64 / encrypt_size); k++) {
          if (k == 0) {
            // encrypt the iv using DES and store the result in ct
            DES(&IVa, &keya, &ct, &ctbit[0], i, 1);
            // XOR the result of above encryption with the plaintext and store
            // the result at the current ctbit address
            XOR(&pta[i][k * encrypt_size], &ct[0][0],
                &ctbit[i][k * encrypt_size], encrypt_size);
          }
          if (k != 0) {
            // change iva to hold the results of the above XOR
            leftShift(IVa, 64, encrypt_size);
            // after lefshift this loop fills the LSB's in IVa with the result
            // of recent xor in ctbit
            for (int l = 0; l < encrypt_size; l++) {
              if (mode == CFB) {
                // fills encrypt size number of bits
                IVa[64 - encrypt_size + l] =
                    ctbit[i][((k - 1) * encrypt_size) + l];
              } else if (mode == OFB) {
                IVa[64 - encrypt_size + l] = ct[0][l];
              }
            }
            // encrypt the iv using DES and store the result in ct
            DES(&IVa, &keya, &ct, &ctbit[0], i, 1);
            // XOR the result of above encryption with the plaintext and store
            // the result at the current ctbit address
            XOR(&pta[i][k * encrypt_size], &ct[0][0],
                &ctbit[i][k * encrypt_size], encrypt_size);
          }
        }
      }
    }

    // operations performed after first round of encryption and for all rounds
    // following
    else if (i != 0) {
      if (mode == CBC) {
        // XOR current plaintext with previous cypher text
        XOR(&pta[i][0], &ctbit[i - 1][0], &pta[i][0], 64);
      }
      // operations that require previous plaintext come first
      else if (mode == PCBC) {
        std::cout << "here\n";
        // XOR previous plaintext with current plaintext storing the result in
        // the cyphertext array temporarily
        XOR(&w_array[0], &pta[i][0], &ctbit[i][0], 64);
        std::copy(&pta[i][0], &pta[i][64], &w_array[0]);
        // XOR result of above with previous cypher text storing the address in
        // the "plaintext array", ready for encryption
        XOR(&ctbit[i][0], &ctbit[i - 1][0], &pta[i][0], 64);
      } else if (mode == CFB || mode == OFB) {
        for (int k = 0; k < 64 / encrypt_size; k++) {
          leftShift(IVa, 64, encrypt_size);
          // after lefshift this loop fills the LSB's in IVa with the result of
          // recent xor in ctbit
          for (int l = 0; l < encrypt_size; l++) {
            if (mode == CFB) {
              // fills encrypt size number of bits
              IVa[64 - encrypt_size + l] =
                  ctbit[i][((k - 1) * encrypt_size) + l];
            }
            if (mode == OFB) {
              IVa[64 - encrypt_size + l] = ct[0][l];
            }
          }
          // encrypt the iv using DES and store the result in ct
          DES(&IVa, &keya, &ct, &ctbit[0], i, 1);
          // XOR the result of above encryption with the plaintext and store the
          // result at the current ctbit address
          XOR(&pta[i][k * encrypt_size], &ct[0][0], &ctbit[i][k * encrypt_size],
              encrypt_size);
        }
      }
    }

    // Block encryption algorithm being used (only used for the block cyphers)
    if (encryptmethod == 0 && (mode == ECB || mode == CBC || mode == PCBC)) {
      DES(&pta[0], &keya, &ct, &ctbit[0], i);
    }
  }

  // Convert binary ctbit array into characters
  unsigned int output[fileLength];
  for (int i = 0; i < fileLength + padding; i++) {
    unsigned int tempc = binarytodecimal(&ctbit[0][i * 8], 8);
    output[i] = tempc;
  }
  std::cout << std::hex;

  arrayout(output, fileLength + padding, 9999999, 1, 1);
  arrayout(output, fileLength + padding, 16, 0, 1);

  return 0;
}
