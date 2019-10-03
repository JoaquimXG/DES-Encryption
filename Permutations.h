#include <map>
#include <string>

#ifndef GLOBALS
#define GLOBALS
extern int lookupPC1[56],bitrotationtable[16],lookupPC2[48],lookupIP[64],lookupfp[64],lookupE[48],lookupP[32],lookupSbox[8][4][16];
extern std::map<char,std::array<int,4>> hexconvert;


#endif //GLOBALS