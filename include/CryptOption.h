#include <string>

#ifndef DESENCRYPTION_INCLUDE_OPTIONS_H_
#define DESENCRYPTION_INCLUDE_OPTIONS_H_

enum CryptMode { ECB, CBC, PCBC, CFB, OFB };

enum CryptMethod { DES, AES };

/*
 * Primarily a container for the command line options passed to crypt
 * Also contains functions required to collect the arguments from the user
 *
 */
class CryptOption {
  int cryptMode;

  bool decrypt; 

  //For future use
  int cryptMethod = DES;

  // decides the size of each block of encryption , used for the stream cypher
  // modes (OFB ,OFB)
  int cryptSize;

  // User input value for iv and key
  std::string iv;

  std::string key;

  std::string inputFileName;
  std::string outputFileName;

  int parseCommandLineArguments(CryptOption& opt, int argc, char* argv[]);

  int parseNonOptionArguments(CryptOption& opt, int argc, char* argv[]);

  int checkEncryptionModeArgument(CryptOption& opt, std::string cryptModeArgument);

  int checkFileArguments(CryptOption& opt);

  void parseKeyAndIvArguments(CryptOption& opt);

  void printUsage();

  public:
  CryptOption(int argc, char* argv[]);

  std::string toString(CryptOption& opt);

};

#endif
