#include <string>

#ifndef DESENCRYPTION_INCLUDE_OPTIONS_H_
#define DESENCRYPTION_INCLUDE_OPTIONS_H_

enum CryptMode { ECB, CBC, PCBC, CFB, OFB };

enum CryptMethod { DES, AES };

const int BYTE_SIZE = 8;

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



  /*
   * Parses all remaining arguments that aren't related to argument flags.
   * 
   * Loops through each reamining argument.
   * Checks to see whether the first argument is encrypt or decrypt.
   * Assigns the second argument to the name of the input file.
   *
   * @param opt A CryptOption argument
   * @param argc The count of command line arguments passed through the shell.
   * @param argv An array of command line argument strings passed through the shell.
   * @return bool False if neither encrypt nor decrypt is chosen or if no input file is passed.
   */
  bool parseNonOptionArguments(CryptOption& opt, int argc, char* argv[]);

  /*
   * Parses the encryption mode argument passed by the user.
   * Sets the appropriate encryption mode value if the passed argument is valid.
   * Will return false if an invalid encryption mode arguement is passed.
   *
   * @param opt A CryptOption object
   * @param cryptModeArgument The Encryption mode argument passed by the user.
   * @return bool True if passed encryption mode argument was valid, else false.
   */
  bool checkEncryptionModeArgument(CryptOption& opt, std::string cryptModeArgument);

  /*
   * Parses the passed input and output files arguments.
   *
   * @param opt A CryptOption object.
   * @return bool False if no input file was passed else true.
   */
  bool checkFileArguments(CryptOption& opt);

  /*
   * Sets default values for IV and Key if no values were passed.
   *
   * @param opt A CryptOption object containing IV and Key values.
   */
  void parseKeyAndIvArguments(CryptOption& opt);


  public:
  /* Constructor */
  CryptOption();

  /*
   * Parses command line arguments using the getopt library.
   *
   * @param opt A CryptOption argument
   * @param argc The count of command line arguments passed through the shell.
   * @param argv An array of command line argument strings passed through the shell.
   * @return int 0 if arguments are correct, -1 if there is a general error with the arguemnts, -2 if a file can't be opened 
   */
  int parseCommandLineArguments(CryptOption& opt, int argc, char* argv[]);

  /*
   * Prints the programs usage options.
   *
   * Will be displayed if -h is passed as an option or if there is an error with one of the inputs.
   */
  void printUsage();

  /*
   * Converts all of the variables contained within the class into a String
   *
   * @param opt A CryptOption object to conver to string
   * @return String A formatted string containng all of the class parameters.
   */
  std::string toString(CryptOption& opt);

  // User input value for iv and key
  std::string iv;
  std::string key;
  std::string inputFileName;
  std::string outputFileName;
};

#endif
