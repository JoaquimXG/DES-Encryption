#include <string>
#include <vector>

/*
 * Opens a file with a given name and reads the contents into a String.
 *
 * @param fileName The name of the file to be opened.
 * @param dumpString The string variable where results will be placed.
 * @return String The string variable where file contents are placed.
 */
std::string getFileContents(std::string fileName);

/*
 * Opens a file with a given name using a given ofstream
 *
 * @param fileStream An ofstream pointer which will be used to manage the output file.
 * @param fileName The name of the file to open.
 * @return bool True if file could be opened, else false.
 */
bool openOutFile(std::string fileName, std::ofstream* fileStream);

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
 * @param shiftSize The number of left shifts to be performed.
 */
void leftShift(std::vector<unsigned>& inVector, int shiftSize);
bool leftShift(std::vector<unsigned>& inVector, int shiftSize, int startPosition, int endPosition);

/*
 * Converts a string to binary.
 * Each charcter is converted to an array of bits, length 8.
 * std::bitset is used to convert each character to an arry of bits.
 * as bitset stores bits in little-endian and big-endian is desired 
 * the bitset is reversed when converted to an arry.
 *
 * @param inString The string to be converted to binary.
 * @param outVector A vector to store the results
 */
void charToBin(std::string inString, std::vector<unsigned>& outVector);
void charToBin(std::string inString, std::vector<std::vector<unsigned>>& outVector);

/*
 * Converts a hex string to binary using a lookup table.
 *
 * Each character is searched for within map<char, array<int, 4>>
 * The corresponding array contains the binary representation of the char.
 * The values in the array are looped over and written into the outvector.
 *
 * @param inString The string to be converted to binary.
 * @param outVector A vector to store the results
 */
void hexToBin(std::string inString, std::vector<unsigned>& outVector);

/*
 * Performs an XOR on each value in 2 Vectors, storing the result in a third.
 * The third array can be one of the 2 input Vector, in which case the result will overwrite the intial values.
 *
 * @param firstInput The first input Vector
 * @param secondInput The second input Vector
 * @param result A vector where the result will be stored, can be one of the 2 input Vectors.
 * @param size The number of values in the Vectors to perform XOR on.
 */
void XOR(std::vector<unsigned>& firstInput, std::vector<unsigned>& secondInput, std::vector<unsigned>& result, int size);

/*
 * Performs an XOR on each value in 2 Vectors, storing the result in a third.
 * The third array can be one of the 2 input Vector, in which case the result will overwrite the intial values.
 *
 * This version takes iterators for starting points in each vector and for the starting point in the result.
 *
 * @param startLeft An iterator pointing to the first value to XOR in the left hand side vector
 * @param startRight An iterator pointing to the first value to XOR in the right hand side vector
 * @param result An iterator pointing to the first index to place a value in a result vector. This could be a position in one of the two input vectors.
 * @param size The number of values in the Vectors to perform XOR on.
 */
void XOR(std::vector<unsigned>::iterator startLeft, std::vector<unsigned>::iterator startRight, std::vector<unsigned>::iterator result, int size);

/*
 * Converts a Vector containing a binary representaion of an integer into the integer.
 * 
 * @param input A Vector containing a binary representation of a decimal integer.
 * @param size The length of the binary Vector.
 * @return The decimal integer
 */
int binToDec(std::vector<unsigned>& input, int size);

/*
 * Converts a section of a Vector containing a binary representaion of an integer into the integer.
 * 
 * @param start An iterator pointing to the start of the section of Vector.
 * @param size The number of bits to be converted to decimal.
 * @return The decimal integer
 */
int binToDec(std::vector<unsigned>::iterator start, int size);

/*
 * Converts a decimal integer into its binary representation.
 * 
 * @param integer An integer to be converted to binary.
 * @param size The maximum number of binary digits required to represent the provided integer.
 * @return A Vector containing the a binary representation of the provided integer.
 */
std::vector<unsigned> decToBin(int integer, int size);

/*
 * Converts a vector containing binary values into a colour formatted string.
 * Useful for troubleshooting functions which handle binary Vectors.
 *
 * @param A vector to be converted to a string.
 * @return A colour formatted string containing each value in the Vector.
 */
std::string binaryVectorToString(std::vector<unsigned>& vect);

/*
 * Converts a vector containing decimal representations of hex values into a string of hex digits.
 *
 * @param A vector to be converted to a string.
 * @return A hex string.
 */
std::string decVectorToHexString(std::vector<unsigned>& vect);

/*
 * Converts a vector containing decimal representations of char values into a string of chars.
 *
 * @param A vector to be converted to a string.
 * @return A char string.
 */
std::string decVectorToCharString(std::vector<unsigned>& vect);


/*
 * Converts a 2D vector containing binary values into a colour formatted string.
 * Useful for troubleshooting functions which handle binary 2D Vectors.
 *
 * @param A 2D vector to be converted to a string.
 * @return A colour formatted string containing each value in the 2D Vector.
 */
std::string binaryVectorToString2D(std::vector<std::vector<unsigned>>& vect);
