#include "../include/CryptOption.h"
#include <getopt.h>
#include <iostream>
#include <sstream>
#include <string.h>

CryptOption::CryptOption(int argc, char *argv[]) {
  if (this->parseCommandLineArguments(*this, argc, argv) < 0) {
    this->printUsage();
  };
}

// Parses command line arguments
// Returns 0 if arguments are correct, -1 if there is a general error with
// arguemnts, -2 if a file can't be opened
// Using getopt library to parse command line arguments
//-: stands for any arguments that don't have options and corresponds with
// case 1
// TODO should you return if the IV is the wrong length???
// Maybe the IV should just be padded, the problem is that it would
// have to be padded in the same manner the next time as well
int CryptOption::parseCommandLineArguments(CryptOption &opt, int argc,
                                           char *argv[]) {
  std::string cryptModeArgument;

  int option;
  while ((option = getopt(argc, argv, "m:o:i:k:")) != -1) {
    switch (option) {
    case 'm':
      cryptModeArgument = std::string(optarg);
      break;
    case 'o':
      opt.outputFileName = optarg;
      break;
    case 'i':
      if (strlen(optarg) != 8) {
        return -1;
      };
      opt.iv = optarg;
      break;
    case 'k':
      if (strlen(optarg) != 8) {
        return -1;
      };
      opt.key = optarg;
      break;
    case '?':
      printf("Unknown option: -%c\n", optopt);
      return -1;
    case ':':
      printf("Missing arg for -%c\n", optopt);
      return -1;
    }
  }

  if (parseNonOptionArguments(opt, argc, argv) == -1)
    return -1;

  parseKeyAndIvArguments(opt);

  if (checkEncryptionModeArgument(opt, cryptModeArgument) == -1)
    return -1;

  if (checkFileArguments(opt) == -1)
    return -2;

  return 0;

}

//TODO Document parseNonOptionArguments
int CryptOption::parseNonOptionArguments(CryptOption& opt, int argc, char* argv[]) {
  int nonOptionArgumentIndex = 0;
  int argumentIndex;
  for (argumentIndex = optind; argumentIndex < argc; argumentIndex++) {
    std::string nonOptionArgument = std::string(argv[argumentIndex]);
    if (nonOptionArgumentIndex == 0) {
      if (nonOptionArgument == "encrypt") {
        opt.decrypt = false;
      } else if (nonOptionArgument == "decrypt") {
        opt.decrypt = true;
      } else {
        std::cout << "\n[-] Please choose either encrypt or decrypt";
        return -1;
      }
      nonOptionArgumentIndex++;
    }

    else if (nonOptionArgumentIndex == 1) {
      opt.inputFileName = nonOptionArgument;
      nonOptionArgumentIndex++;
    } else {
      return -1;
    }
  }
  return 0;
}

// TODO generate a random key and IV and display them to the user at the end
// TODO Document parseKeyAndIvArguments
void CryptOption::parseKeyAndIvArguments(CryptOption &opt) {
  if (opt.key.empty()) {
    std::cout << "\n[-] No key was chosen, using default (00000000)";
    opt.key = "00000000";
  }

  if (opt.iv.empty()) {
    std::cout << "\n[-] No IV was chosen, using default (00000000)";
    opt.iv = "00000000";
  }
}

//TODO document checkFileArguments
int CryptOption::checkFileArguments(CryptOption &opt) {
  // Handle input file not being set
  if (opt.inputFileName.empty()) {
    if (opt.decrypt) {
      std::cout << "\n[-] Please provide a file to decrypt";
    } else {
      std::cout << "\n[-] Please provide a file to encrypt";
    }
    return -1;
  }

  // Handle output file not being set
  if (opt.outputFileName.empty()) {
    if (decrypt) {
      std::cout << "\n[+] No output file chosen, using default (decrypted.txt)";
      opt.outputFileName = "decrypted.txt";
    } else {
      std::cout << "\n[+] No output file chosen, using default (encrypted.txt)";
      opt.outputFileName = "encrypted.txt";
    }
  }

  return 0;
}

//TODO document checkEncryptionModeArgument
int CryptOption::checkEncryptionModeArgument(CryptOption &opt,
                                             std::string cryptModeArgument) {
  if (cryptModeArgument.empty()) {
    std::cout
        << "\n[+] No mode of operation was chosen, using default (ECB Mode)";
    opt.cryptMode = ECB;
    opt.cryptSize = 64;
  } else if (cryptModeArgument == "ecb" || cryptModeArgument == "ECB") {
    std::cout << "\n[+] Utilising ECB Mode of operation";
    opt.cryptMode = ECB;
    opt.cryptSize = 64;
  } else if (cryptModeArgument == "cbc" || cryptModeArgument == "CBC") {
    std::cout << "\n[+] Utilising CBC Mode of operation";
    opt.cryptMode = CBC;
  } else if (cryptModeArgument == "pcbc" || cryptModeArgument == "PCBC") {
    std::cout << "\n[+] Utilising PCBC Mode of operation";
    opt.cryptMode = PCBC;
  } else if (cryptModeArgument == "cfb" || cryptModeArgument == "CFB") {
    std::cout << "\n[+] Utilising CFB Mode of operation";
    opt.cryptMode = CFB;
    opt.cryptSize = 8;
  } else if (cryptModeArgument == "ofb" || cryptModeArgument == "OFB") {
    std::cout << "\n[+] Utilising OFB Mode of operation";
    opt.cryptMode = OFB;
    opt.cryptSize = 8;
  } else {
    std::cout << "\n[-] No valid encryption mode selected";
    return -1;
  }

  return 0;
}

//TODO document printUsage
void CryptOption::printUsage() {
  std::cout << "\nDES Decryptor\n\n\
Usage:\n\
    crypt encrypt <plain text file> [-k key] [-i IV] [-o outFile] [-m mode]\n\
    crypt decrypt <cypher text file> [-k key] [-i IV] [-o outFile] [-m mode]\n\
\n\
Options:\n\
    -h --help     Show this screen.\n\
    -i IV         Initialisation vector for encryption\n\
    -k key        Encryption key\n\
    -o outFile    Output file\n\
    -m mode       Mode of operation [ECB | CBC | PCBC | CFB | OFB]\n";
}

//TODO document toString
std::string CryptOption::toString(CryptOption &opt) {
  std::ostringstream returnString;
  returnString << "\n\nIV: " << opt.iv << "\nKey: " << opt.key
               << "\nInput File: " << opt.inputFileName
               << "\nOutput File: " << opt.outputFileName
               << "\nCrypt Size: " << opt.cryptSize
               << "\nCrypt Mode: " << opt.cryptMode << "\n";
  return returnString.str();
}
