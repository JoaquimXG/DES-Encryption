#include "../include/CryptOption.h"
#include "../include/CryptParameters.h"
#include "../include/cryptUtils.h"
#include "../include/DesAlgorithm.h"
#include "../include/CryptMode.h"
#include <iostream>
#include <fstream>

std::string& getFileContents(std::string inputFileName, std::string& outString);
bool openOutFile(std::string fileName, std::ofstream* fileStream);

int main(int argc, char *argv[]) {
  //Parse command line options
  CryptOption opt = CryptOption();
  if (opt.parseCommandLineArguments(opt, argc, argv) < 0) {
    opt.printUsage();
    return 0;
  };

  // DEBUG -- Print command line options
  //std::cout << opt.toString(opt);

  // Get input file contents
  std::string inFileString = getFileContents(opt.inputFileName);
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

  DesAlgorithm cryptAlgo = DesAlgorithm();
  EcbMode cryptMode = EcbMode(&params, &cryptAlgo);
  cryptMode.encrypt();
  std::cout << cryptMode.toString();

  std::vector<unsigned> decimalOutput = cryptMode.resultToDecimal();

  outFileStream << hexVectorToString(decimalOutput);
}
