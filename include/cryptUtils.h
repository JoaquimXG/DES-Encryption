#include <string>
#include <vector>

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
 * @return 0
 */
int charToBit(std::string inString, std::vector<unsigned>& outVector);
int charToBit(std::string inString, std::vector<std::vector<unsigned>>& outVector);

/*
 * Converts a vector containing binary values into a colour formatted string.
 * Useful for troubleshooting functions which handle binary Vectors.
 *
 * @param A vector to be converted to a string.
 * @return A colour formatted string containing each value in the Vector.
 */
std::string binaryVectorToString(std::vector<unsigned>& vect);


/*
 * Converts a 2D vector containing binary values into a colour formatted string.
 * Useful for troubleshooting functions which handle binary 2D Vectors.
 *
 * @param A 2D vector to be converted to a string.
 * @return A colour formatted string containing each value in the 2D Vector.
 */
std::string binaryVectorToString2D(std::vector<std::vector<unsigned>>& vect);

