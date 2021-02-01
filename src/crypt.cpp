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
  CryptOption option = CryptOption();
  if (option.parseCommandLineArguments(argc, argv) < 0) {
    option.printUsage();
    return 0;
  };

  Debugger debug = Debugger(option);
  debug.print(option.toString());

  // Get input file contents
  std::string inFileString = getFileContents(option.inputFileName);
  if (inFileString.compare("") == 0){
    return -2;
  }

  // Test opening output file
  std::ofstream outFileStream;
  if (!openOutFile(option.outputFileName, &outFileStream))
    return -2;

  // Prepares encryption parameters
  CryptParameters params = CryptParameters(inFileString.size(), option.toDecrypt);
  params.generateSubKeys(option.key);
  params.parseInputFile(inFileString);
  charToBin(option.iv, params.ivVect);

  debug.print(params.toString());

  //Choose mode of encryption
  CryptMode* cryptMode;
  switch(option.cryptMode){
    case CryptMode::Mode::CBC:
      cryptMode = new CbcMode(&params, &option);
      break;
    case CryptMode::Mode::PCBC:
      cryptMode = new PcbcMode(&params, &option);
      break;
    case CryptMode::Mode::CFB:
      cryptMode = new CfbMode(&params, &option);
      break;
    case CryptMode::Mode::OFB:
      cryptMode = new OfbMode(&params, &option);
      break;
    default:
      cryptMode = new EcbMode(&params, &option);
  }

  //Perform encryption or decryption
  std::vector<unsigned> output;
  if (option.toDecrypt){
    cryptMode->decrypt();
    output = cryptMode->resultToDecimal();
    outFileStream << decVectorToCharString(output);
  }
  else {
    cryptMode->encrypt();
    output = cryptMode->resultToDecimal();
    outFileStream << decVectorToHexString(output);
  }

  debug.print(cryptMode->toString());

  return 0;
}
