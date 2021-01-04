#include "../include/OLDfunctions.h"
#include "../include/OLDpermutations.h"
#include <bitset>
#include <fstream>
#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <map>
#include <math.h>
#include <string.h>
#include <string>

std::ofstream output;
std::ifstream input;

template <typename T>
// outputs the contents of an integer array of size s in groups of size (split)
// either to a file (1) or to the console (0)
int arrayout(T array[], int size, int split, int file, bool hex) {
  if (file == 0) {
    std::cout << "\n";
    std::cout << "Size of array: " << size << "\n";
    for (int i = 0; i < size; i++) {
      if (hex == true) {
        std::cout << std::setfill('0') << std::setw(2) << *array;
      } else {
        std::cout << *array;
      }
      array++;
      if ((i + 1) % split == 0) {
        std::cout << "\n";
      }
    }
  }
  if (file == 1) {
    std::cout << "\nOutputting to file";
    for (int i = 0; i < size; i++) {
      if (hex == true) {
        output << std::hex << std::setfill('0') << std::setw(2) << *array;
      } else {
        output << *array;
      }
      array++;
      // output<<",";
      if ((i + 1) % split == 0) {
        output << "\n";
      }
    }
  }
  return 0;
}
// defining templates currently in use
template int arrayout(unsigned array[], int size, int split, int file,
                      bool hex);
template int arrayout(char array[], int size, int split, int file, bool hex);

/*
 * Arithmetic left shift, modified array in place.
 * Moves every element in an array to the left 'shiftSize' number of times.
 * Any element which would fall out of the array is moved to the end to
 * fill the new space.
 *
 * First, iterates through the input array and moves 'shiftSize' number of values
 * into a holding array. 
 * Continues to iterate through the input array, now moving values to the start of the array.
 * Then iterates through the holding array, moving each value into the next available space in the inputArray.
 *
 * @param inputArray[] An unsigned array to be left shifted, will be modified in place.
 * @param arrayLength The length of the inputArray.
 * @param shiftSize The number of left shifts to be performed.
 */
void leftShift(unsigned inputArray[], int arrayLength, int shiftSize) {
  if (shiftSize >= arrayLength) {
    shiftSize = shiftSize % arrayLength;
  }

  int hold[shiftSize];
  for (int i = 0; i < arrayLength; i++) {
    if (i<shiftSize){
      hold[i] = inputArray[i];
    }
    else {
      inputArray[i-shiftSize] = inputArray[i];
    }
  }

  int offset = arrayLength - shiftSize;
  for (int i = 0; i < shiftSize; i++) {
    inputArray[offset + i] = hold[i];
  }
}


// TODO, could this function not just return the bitset
/*
 * Converts a string to binary.
 * Each charcter is converted to an array of bits, length 8.
 * std::bitset is used to convert each character to an arry of bits.
 * as bitset stores bits in little-endian and big-endian is desired 
 * the bitset is reversed when converted to an arry.
 *
 * @param inString The string to be converted to binary.
 * @param outputArray An array where the result will be stored, the array must be long enough to store the result.
 * @return 0
 */
int charToBit(std::string inString, unsigned outArray[]) {
  std::cout << "------------------------ inString: " <<inString <<"\n";
  int stringLength = inString.size();
  for (int i = 0; i < stringLength; i++) {

    std::bitset<8> tempBitset(inString.c_str()[i]);
    for (int j = 7; j > -1; j--) {

      *outArray = tempBitset[j];
      outArray++;
    }
  }
  return 0;
}

// only to be used on arrays that are binary representations, first and second
// are the arrays to XOR and result will be the address they are stored, size is
// the size of the arrays to be XOR'd
int XOR(unsigned first[], unsigned second[], unsigned result[], int size) {
  for (int i = 0; i < size; i++) {
    if (*first == *second) {
      *result = 0;
    } else {
      *result = 1;
    }
    first++;
    second++;
    result++;
  }
  return 0;
}
// result is the address the result will be stored,currently feistal is
// resereved for arrays of 48 bits
int feistal(unsigned *target, unsigned result[], unsigned key[]) {
  unsigned tempa[8][6];
  unsigned tempb[32];
  unsigned *aiterator = &tempa[0][0];
  // arrayout(target,32,8);

  // iterate through the target array moving each value to a new position in the
  // temp array based on the lookupE array
  for (int i = 0; i < 48; i++) {
    *aiterator = target[lookupE[i]];
    // std::cout<<*aiterator<<":"<<target[lookupE[i]]<<":"<<*tempa[i]<<"-----";
    aiterator++;
    if ((i + 1) % 6 == 0) {
      // std::cout<<"\n";
    }
  }
  // std::cout<<"temp    :    key    :   afterXOR\n";
  // arrayout(&tempa[0][0],48,8);
  // arrayout(&key[0],48,8);
  XOR(&tempa[0][0], &key[0], &tempa[0][0], 48);
  // arrayout(&tempa[0][0],48,48,1);

  // iterate through the 8 groups of six bits in the temp array and srhink back
  // down to 32 bits using the s-boxes
  for (int i = 0; i < 8; i++) {

    unsigned rowa[2] = {tempa[i][0], tempa[i][5]};
    unsigned columna[4] = {tempa[i][1], tempa[i][2], tempa[i][3], tempa[i][4]};

    int row = binarytodecimal(&rowa[0], 2);
    int column = binarytodecimal(&columna[0], 4);

    int sboxint = lookupSbox[i][row][column];
    unsigned binarytemp[4];
    // std::cout<<"\nsboxint: "<<sboxint;
    decimaltobinary(sboxint, &binarytemp[0], 4);
    // arrayout(&binarytemp[0],4,8);
    // iterate through the temporary binary array and add to temporary array
    for (int j = 0; j < 4; j++) {
      tempb[i * 4 + j] = binarytemp[j];
    }
  }
  // P Permutation
  for (int i = 0; i < 32; i++) {
    result[i] = tempb[lookupP[i]];
  }

  return 0;
}

// converts a binary array into an integer value where size is the size of the
// array
int binarytodecimal(unsigned binarya[], int size) {
  int result = 0;
  // reverse iteration through the binary array
  for (int i = size; i > 0; i--) {
    // multiplies each value in the binary arrray by 2 to the power of its
    // position in the array (in small endian order) so the least significant bit
    // *2^0 then the next is *2^1 ...
    result += binarya[i - 1] * pow(2, size - i);
  }
  return result;
}

// converts a decimal integer into binary, storing the result at the address of
// binarya, size is the length of array storing the data
void decimaltobinary(int integer, unsigned binarya[], int size) {
  // std::cout<<"\nint: "<<integer;
  // iterating in reverse through the binary array to calulate each position in
  // turn
  for (int i = size; i > 0; i--) {
    binarya[i - 1] = integer % 2;
    // std::cout<<"\ninteger%2: "<<integer%2;
    integer /= 2;
  }
}

//TODO could this function also not just return the keys
/* 
 * Generates 16 48bit subkeys for DES from a 64 bit key.
 * The 64 bit key is first converted from characters to binary.
 * 56 bits are then selected from the key using the Permuted Choice 1 table
 * This 56 bit key is split in half into two 28 bit keys.
 * For 16 rounds,
 * Each 28 bit key is left shifted either 1 or 2 bits in a predefined order
 * as defined by bitRotationTable.
 * 48 bits are then selected from a combination of the 28 bit keys using 
 * the Permuted Choice 2 table to form the each 48 bit key.
 *
 * @param &key A reference to std::string for the key input
 * @param (*keya)[16][48] A pointer to a 2 dimensional array, 16 bits * 48 bit to store the 16 keys
 */
void generateSubKeys(std::string &key, unsigned (*keya)[16][48] ) {

  unsigned keyPC1[2][28];
  unsigned keya64bit[64];
  charToBit(key, keya64bit);

  for (int i = 0; i < 56; i++) {
    keyPC1[0][i] = keya64bit[permutedChoice1[i]];
  }

  for (int i = 0; i < 16; i++) {
    leftShift(&keyPC1[0][0], 28, bitRotationTable[i]);
    leftShift(&keyPC1[1][0], 28, bitRotationTable[i]);

    for (int j = 0; j < 48; j++) {
      (*keya)[i][j] = keyPC1[0][permutedChoice2[j]];
    }
  }
}
void hextobit(std::string file_contents, unsigned *array) {
  int size = file_contents.size();
  auto fileit = &file_contents[0];
  // loops through key characters and turns them into 4 bit representaion, adds
  // that to array
  for (int i = 0; i < size; i++) {
    auto iterator = hexconvert.find(*(fileit + i));
    for (int k = 0; k < 4; k++) {
      *array = iterator->second[k];
      array++;
    }
  }
}

// Parses command line arguments
// Returns 0 if arguments are correct, -1 if there is a general error with
// arguemnts, -2 if a file can't be opened
int parseCommandLineArguments(int argc, char *argv[], Options* encryptOpts,
                              bool decrypt) {
  std::string outputFileName;
  std::string modeString;

  // Using getopt library to parse command line arguments
  //-: stands for any arguments that don't have options and corresponds with
  //case 1
  int opt;
  int ivLength, keyLength;
  while ((opt = getopt(argc, argv, "-:m:o:i:k:")) != -1) {
    switch (opt) {
    case 'm':
      // printf("Option m has arg: %s\n", optarg);
      modeString = std::string(optarg);
      break;
    case 'o':
      // printf("Option o has arg: %s\n", optarg);
      outputFileName = optarg;
      break;
    case 'i':
      // TODO should you return if the IV is the wrong length???
      // Maybe the IV should just be padded, the problem is that it would
      // have to be padded in the same manner the next time as well
      if (strlen(optarg) != 8) {
        return -2;
      };
      (*encryptOpts).iv = optarg;
      break;
    case 'k':
      if (strlen(optarg) != 8) {
        return -2;
      };
      (*encryptOpts).key = optarg;
      break;

    // Return with error if either an unknown option is passed or if there are
    // missing arguments
    case '?':
      printf("Unknown option: -%c\n", optopt);
      break;
      return -1;
    case ':':
      printf("Missing arg for -%c\n", optopt);
      return -1;
    case 1:
      // Set the input file name to the first non-option argument
      // if inputFileName has already been set then there are too many
      // commandline arguments
      if ((*encryptOpts).inputFileName.empty()) {
        (*encryptOpts).inputFileName = optarg;
        break;
      } else {
        return -1;
      }
    }
  }

  // Handle input file not being set
  if ((*encryptOpts).inputFileName.empty()) {
    if (decrypt) {
      std::cout << "[-] Please provide a file to decrypt\n";
    } else {
      std::cout << "[-] Please provide a file to encrypt\n";
    }
    return -1;
  } else {
    input.open((*encryptOpts).inputFileName.c_str());
    if (!input.is_open()) {
      std::cout << "[-] ------------ Can't open input file ------------\n";
      return -2;
    }
  }

  // Handle output file not being set
  if (outputFileName.empty()) {
    if (decrypt) {
      std::cout << "[+] No output file chosen, using default (decrypted.txt)\n";
      outputFileName = "decrypted.txt";
    } else {
      std::cout << "[+] No output file chosen, using default (encrypted.txt)\n";
      outputFileName = "encrypted.txt";
    }
  }

  // Handle no Key set
  // TODO generate a random key and display it to the user at the end
  if ((*encryptOpts).key.empty()) {
    std::cout << "[-] No key was chosen, using default (00000000)\n";
    (*encryptOpts).key = "00000000";
  }

  // Handle no IV set
  // TODO generate a random IV and display it to the user at the end
  if ((*encryptOpts).iv.empty()) {
    std::cout << "[-] No IV was chosen, using default (00000000)\n";
    (*encryptOpts).iv = "00000000";
  }

  // Handle mode option, either a mode arguement was never passed and the
  // default will be chosen or a mode has been passed and should be parsed
  if (modeString.empty()) {
    std::cout
        << "[+] No mode of operation was chosen, using default (ECB Mode)\n";
    (*encryptOpts).mode = ECB;
    (*encryptOpts).encrypt_size = 64;
    (*encryptOpts).encryptmethod = DES_m;
  } else {
    if (parseEncryptionModeArg(modeString, encryptOpts) == -1) {
      std::cout << "[-] No valid encryption mode selected\n";
      return -1;
    }
  }

  // Open output file
  output.open(outputFileName);
  if (!output.is_open()) {
    std::cout << "[-] ------------ Can't open output file ------------\n";
    return -2;
  }

  return 0;
}

int parseEncryptionModeArg(std::string encryptionArg, Options *encryptOpts) {
  if (encryptionArg == "ecb" || encryptionArg == "ECB") {
    std::cout << "[+] Utilising ECB Mode of operation\n";
    (*encryptOpts).mode = ECB;
    (*encryptOpts).encrypt_size = 64;
  } else if (encryptionArg == "cbc" || encryptionArg == "CBC") {
    std::cout << "[+] Utilising CBC Mode of operation\n";
    (*encryptOpts).mode = CBC;
  } else if (encryptionArg == "pcbc" || encryptionArg == "PCBC") {
    std::cout << "[+] Utilising PCBC Mode of operation\n";
    (*encryptOpts).mode = PCBC;
  } else if (encryptionArg == "cfb" || encryptionArg == "CFB") {
    std::cout << "[+] Utilising CFB Mode of operation\n";
    (*encryptOpts).mode = CFB;
    (*encryptOpts).encrypt_size = 8;
  } else if (encryptionArg == "ofb" || encryptionArg == "OFB") {
    std::cout << "[+] Utilising OFB Mode of operation\n";
    (*encryptOpts).mode = OFB;
    (*encryptOpts).encrypt_size = 8;
  } else {
    return -1;
  }

  // Currently hardcoding this as only one encryption method has been developed
  // for the time being
  (*encryptOpts).encryptmethod = DES_m;
  return 0;
}

void printUsage(bool decrypt) {
  if (decrypt) {
    std::cout << "DES Decryptor\n\n\
Usage:\n\
    decryptor inFile [-k key] [-i IV] [-o outFile] [-m mode]\n\
\n\
Options:\n\
    -h --help     Show this screen.\n\
    -i IV         Initialisation vector for encryption\n\
    -k key        Encryption key\n\
    -o outFile    Output file\n\
    -m mode       Mode of operation [ECB | CBC | PCBC | CFB | OFB]\n";
  } else {
    std::cout << "DES Encryptor\n\n\
Usage:\n\
    encryptor inFile [-k key] [-i IV] [-o outFile] [-m mode]\n\
\n\
Options:\n\
    -h --help     Show this screen.\n\
    -i IV         Initialisation vector for encryption\n\
    -k key        Encryption key\n\
    -o outFile    Output file\n\
    -m mode       Mode of operation [ECB | CBC | PCBC | CFB | OFB]\n";
  }
}
