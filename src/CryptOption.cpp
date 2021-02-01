#include "../include/CryptOption.h"
#include "../include/CryptMode.h"
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <sstream>
#include <string.h>

CryptOption::CryptOption()
    : cryptMode(CryptMode::ECB), toDecrypt(false), cryptMethod(DES), cryptSize(0),
      inputFileName(), outputFileName(){};

int CryptOption::parseCommandLineArguments(int argc, char *argv[]) {
  std::string cryptModeArgument;

  int option;
  while ((option = getopt(argc, argv, "m:o:i:k:s:hd")) != -1) {
    switch (option) {
    case 'm':
      cryptModeArgument = std::string(optarg);
      break;
    case 'o':
      this->outputFileName = optarg;
      break;
    case 'i':
      if (strlen(optarg) != 8) {
        return -1;
      };
      this->iv = optarg;
      break;
    case 'k':
      if (strlen(optarg) != 8) {
        return -1;
      };
      this->key = optarg;
      break;
    case 's':
      this->cryptSize = std::atoi(optarg);
      break;
    case 'd':
      this->debug = true;
      break;
    case 'h':
      return -1;
    case '?':
      printf("Unknown option: -%c\n", optopt);
      return -1;
    case ':':
      printf("Missing arg for -%c\n", optopt);
      return -1;
    }
  }

  if (!parseNonOptionArguments(argc, argv))
    return -1;

  parseKeyAndIvArguments();

  if (!checkEncryptionModeArgument(cryptModeArgument))
    return -1;

  if (!checkFileArguments())
    return -2;

  return 0;
}

bool CryptOption::parseNonOptionArguments(int argc, char *argv[]) {
  int nonOptionArgumentIndex = 0;
  int argumentIndex;
  for (argumentIndex = optind; argumentIndex < argc; argumentIndex++) {
    std::string nonOptionArgument = std::string(argv[argumentIndex]);
    if (nonOptionArgumentIndex == 0) {
      if (nonOptionArgument == "encrypt") {
        this->toDecrypt = false;
      } else if (nonOptionArgument == "decrypt") {
        this->toDecrypt = true;
      } else {
        std::cout << "\n[-] Please choose to either encrypt or decrypt";
        return false;
      }
      nonOptionArgumentIndex++;
    }

    else if (nonOptionArgumentIndex == 1) {
      this->inputFileName = nonOptionArgument;
      nonOptionArgumentIndex++;
    } else {
      return false;
    }
  }
  return true;
}

void CryptOption::parseKeyAndIvArguments() {
  if (this->key.empty()) {
    std::cout << "\n[-] No key was chosen, using default (00000000)";
    this->key = "00000000";
  }

  if (this->iv.empty()) {
    std::cout << "\n[-] No IV was chosen, using default (00000000)";
    this->iv = "00000000";
  }
}

bool CryptOption::checkFileArguments() {
  // Handle input file not being set
  if (this->inputFileName.empty()) {
    if (this->toDecrypt) {
      std::cout << "\n[-] Please provide a file to decrypt";
    } else {
      std::cout << "\n[-] Please provide a file to encrypt";
    }
    return false;
  }

  // Handle output file not being set
  if (this->outputFileName.empty()) {
    if (this->toDecrypt) {
      std::cout << "\n[+] No output file chosen, using default (decrypted.txt)";
      this->outputFileName = "decrypted.txt";
    } else {
      std::cout << "\n[+] No output file chosen, using default (encrypted.txt)";
      this->outputFileName = "encrypted.txt";
    }
  }

  return true;
}

bool CryptOption::checkEncryptionModeArgument(std::string cryptModeArgument) {
  int localCryptSize = 64;
  if (cryptModeArgument.empty()) {
    std::cout
        << "\n[+] No mode of operation was chosen, using default (ECB Mode)";
  } else if (cryptModeArgument == "ecb" || cryptModeArgument == "ECB") {
    std::cout << "\n[+] Utilising ECB Mode of operation";
  } else if (cryptModeArgument == "cbc" || cryptModeArgument == "CBC") {
    std::cout << "\n[+] Utilising CBC Mode of operation";
    this->cryptMode = CryptMode::CBC;
  } else if (cryptModeArgument == "pcbc" || cryptModeArgument == "PCBC") {
    std::cout << "\n[+] Utilising PCBC Mode of operation";
    this->cryptMode = CryptMode::PCBC;
  } else if (cryptModeArgument == "cfb" || cryptModeArgument == "CFB") {
    std::cout << "\n[+] Utilising CFB Mode of operation";
    this->cryptMode = CryptMode::CFB;
    localCryptSize = 8;
  } else if (cryptModeArgument == "ofb" || cryptModeArgument == "OFB") {
    std::cout << "\n[+] Utilising OFB Mode of operation";
    this->cryptMode = CryptMode::OFB;
    localCryptSize = 8;
  } else {
    std::cout << "\n[-] No valid encryption mode selected";
    return false;
  }
  if (this->cryptSize == 0){
    this->cryptSize = localCryptSize;
  }

  return true;
}

void CryptOption::printUsage() {
  std::cout << "\nDES Decryptor\n\
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

std::string CryptOption::toString() {
  std::ostringstream returnString;
  returnString << "\n\nIV: " << this->iv << "\nKey: " << this->key
               << "\nInput File: " << this->inputFileName
               << "\nOutput File: " << this->outputFileName
               << "\nCrypt Size: " << this->cryptSize
               << "\nCrypt Mode: " << this->cryptMode << "\n\n";
  return returnString.str();
}
