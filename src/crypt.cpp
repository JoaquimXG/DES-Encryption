#include "../include/CryptOption.h"
#include "../include/CryptParameters.h"
#include "../include/cryptUtils.h"
#include <fstream>
#include <iostream>

std::string& getFileContents(std::string inputFileName, std::string& outString);
bool openOutFile(std::string fileName, std::ofstream* fileStream);

int main(int argc, char *argv[]) {
  std::cout << "Starting Crypt \n";

  //Parse command line options
  CryptOption opt = CryptOption();
  if (opt.parseCommandLineArguments(opt, argc, argv) < 0) {
    opt.printUsage();
  };

  // DEBUG -- Print command line options
  //std::cout << opt.toString(opt);

  // Get input file contents
  std::string inFileString;
  inFileString = getFileContents(opt.inputFileName, inFileString);
  if (inFileString.compare("") == 0){
    return -2;
  }

  // Test opening output file
  std::ofstream outFileStream;
  if (!openOutFile(opt.outputFileName, &outFileStream))
    return -2;

  // Prepares encryption parameters
  CryptParameters params = CryptParameters(inFileString.size());
  params.generateSubKeys(opt.key);
  charToBit(opt.iv, params.ivVect);
  params.parseInputFile(inFileString);

  std::cout << params.toString();

}


//TODO Move these functions to an appropriate file and add docstrings

std::string& getFileContents(std::string fileName, std::string& dumpString){
  std::ifstream fileStream;
  dumpString = "";

  std::cout << "[+] Opening File: "<< fileName << "\n";
  fileStream.open(fileName);
  if (!fileStream.is_open()) {
    std::cout << "\n[-] ------------ Can't open input file ------------\n";
    return dumpString;
  }

  dumpString = std::string{std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>()};

  return dumpString;
}

bool openOutFile(std::string fileName, std::ofstream* fileStream){
  std::cout << "[+] Opening File: "<< fileName << "\n";
  (*fileStream).open(fileName);
  if (!(*fileStream).is_open()) {
    std::cout << "\n[-] ------------ Can't open output file ------------";
    return false;
  }
  return true;
}
