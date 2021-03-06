#include <map>
#include <string>

#ifndef GLOBALS
#define GLOBALS
extern int keyPermutedChoice1Table[56], keyBitRotationTable[16], keyPermutedChoice2Table[48], lookupInitalPermutation[64],
    lookupFinalPermutation[64], lookupFeistalExpansion[48], lookupFeistalPermutation[32], lookupSBox[8][4][16];
extern std::map<char, std::array<int, 4>> hexLookupTable;

#endif // GLOBALS
