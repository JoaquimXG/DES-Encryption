#include <string>

#ifndef DESENCRYPTION_INCLUDE_OPTIONS_H_
#define DESENCRYPTION_INCLUDE_OPTIONS_H_

struct Options {
  int mode;

  // currently not in use but will be for AES and other encryption algorthms
  int encryptmethod;

  // decides the size of each block of encryption , used for the stream cypher
  // modes (OFB ,OFB)
  int encrypt_size;

  // User input value for IV
  std::string iv;

  // User input value for key
  std::string key;

  unsigned ivArray[64];
  unsigned keyArray[16][48];
};


enum Mode { ECB, CBC, PCBC, CFB, OFB };

enum Encrypt { DES_m, AES_m };

#endif
