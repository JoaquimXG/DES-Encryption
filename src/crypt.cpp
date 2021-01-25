#include "../include/CryptOption.h"
#include "../include/CryptParameters.h"
#include "../include/cryptUtils.h"
#include "../include/DesAlgorithm.h"
#include "../include/CryptMode.h"
#include "../include/Debugger.h"
#include <iostream>
#include <fstream>

std::string& getFileContents(std::string inputFileName, std::string& outString);
bool openOutFile(std::string fileName, std::ofstream* fileStream);

int main(int argc, char *argv[]) {
  //Parse command line options
  CryptOption opt = CryptOption();
  if (opt.parseCommandLineArguments(argc, argv) < 0) {
    opt.printUsage();
    return 0;
  };

  Debugger debug = Debugger(opt);
  debug.print(opt.toString());

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
  params.parseInputFile(inFileString);
  charToBit(opt.iv, params.ivVect);

  debug.print(params.toString());

  //Choose mode of encryption
  CryptMode* cryptMode;
  switch(opt.cryptMode){
      default:
          cryptMode = new EcbMode(&params, &opt);
  }

  cryptMode->encrypt();
  debug.print(cryptMode->toString());

  std::vector<unsigned> decimalOutput = cryptMode->resultToDecimal();
  outFileStream << hexVectorToString(decimalOutput);
}
