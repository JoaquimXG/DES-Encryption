#include "../include/functions.h"
#include <string>
#include <iostream>
#include <bitset>
#include <math.h>
#include "../include/permutations.h"
#include <fstream>
#include <map>
#include <iomanip>
#include <getopt.h>

extern std::ofstream output;
extern std::ifstream input;

template<typename T>
int arrayout(T array[],int size,int split,int file,bool hex){//outputs the contents of an integer array of size s in groups of size (split) either to a file (1) or to the console (0) 
    if (file==0){
        std::cout<<"\n";
        std::cout<< "Size of array: "<< size<<"\n";
        for(int i=0;i<size;i++){
            if (hex==true){
                std::cout<<std::setfill('0')<<std::setw(2)<<*array;
            }
            else{
                std::cout<<*array;
            }
            array++;
            if ((i+1)%split==0){
                std::cout<<"\n";
            }
        }

    }
    if (file ==1){
        std::cout<<"\nOutputting to file";
        for(int i=0;i<size;i++){
            if (hex==true){
                output<<std::hex<<std::setfill('0')<<std::setw(2)<<*array;
            }
            else{
                output<<*array;
            }
            array++;
            //output<<",";
            if ((i+1)%split==0){
                output<<"\n";
            }
        }
        
    }
    return 0;
}
template int arrayout(unsigned array[],int size,int split,int file,bool hex);//defining templates currently in use 
template int arrayout(char array[],int size,int split,int file,bool hex);

int leftshift(unsigned array[],int size,int shift){//array to be shifted, size of array, size of shift to be performed
    if (shift>=size){
        shift=shift%size;
    }
    int hold[shift];//creates an array to hold the values that will be shifted off the left end of the array
    for (int i=0;i<shift;i++){//iterates through shift number of values in the array to save the first values that will be moved to the end of the array
        hold[i]=*array;
        array++;
    }
    for (int i=0;i<(size-shift);i++){//after all the values that will be moved to the end have been saved we can begin moving values to the beginning of the array we should be at array[shift] so *array-shift = array[0] then we iterate through 
        *(array-shift)=*array;
        array++;
    }
    //the last place will be array[last] moved to array[last-shift] the iterator then moved past the end of the array
    array--;//so we need to bring it back, we are now at the last position in the array
    for (int i =(shift-1);i>-1;i--){//iterates through the holding array starting at the end as this is where the array pointer is pointing, will run shift times for each value in the holding array
        *array=hold[i];//i= shift-1 so the first value of i will be the last value in the hold array
        array--;//iterating backwards through array
    }
    return 0;
}

int chartobit(std::string string,unsigned array[]){
    int size = string.size();
    for (int i=0;i<size;i++){//loops through key characters and turns them into 8 bit representaion, adds that to keya
        std::bitset<8>temp(string.c_str()[i]);//turn each character into 8 bit representation
        for (int j=7;j!=-1;j--){//loop through the values in 8 bit array (temp) and add to the full keya (the iterator has to start at 7 because the order of the bits in the array (temp) are in little endian order)
            *array=temp[j];
            array++;
        }
    }
    return 0;
}

int XOR(unsigned first[],unsigned second[],unsigned result[],int size){//only to be used on arrays that are binary representations, first and second are the arrays to XOR and result will be the address they are stored, size is the size of the arrays to be XOR'd
    for (int i=0;i<size;i++){
        //std::cout<<"\nfirst*: "<<*first<<"      *Second: "<<*second;
        if(*first==*second){
            //std::cout<<"\n==";
            *result=0;
        }
        else{
            //std::cout<<"\n!=";
            *result=1;
        }
        first++;
        second++;
        result++;
        //std::cout<<"\n*result: "<<*result;
    }
    return 0;
}
int feistal(unsigned *target,unsigned result[],unsigned key[]){//result is the address the result will be stored,currently feistal is resereved for arrays of 48 bits
    unsigned tempa[8][6];
    unsigned tempb[32];
    unsigned *aiterator=&tempa[0][0];
    //arrayout(target,32,8);

    for (int i=0;i<48;i++){//iterate through the target array moving each value to a new position in the temp array based on the lookupE array
        *aiterator=target[lookupE[i]];
        //std::cout<<*aiterator<<":"<<target[lookupE[i]]<<":"<<*tempa[i]<<"-----";
        aiterator++;
        if ((i+1)%6==0){
            //std::cout<<"\n";
        }
    }
    //std::cout<<"temp    :    key    :   afterXOR\n";
    //arrayout(&tempa[0][0],48,8);
    //arrayout(&key[0],48,8);
    XOR(&tempa[0][0],&key[0],&tempa[0][0],48);
    //arrayout(&tempa[0][0],48,48,1);

    for (int i=0;i<8;i++){//iterate through the 8 groups of six bits in the temp array and srhink back down to 32 bits using the s-boxes

        unsigned rowa[2] = {tempa[i][0],tempa[i][5]};
        unsigned columna[4] = {tempa[i][1],tempa[i][2],tempa[i][3],tempa[i][4]};

        int row = binarytodecimal(&rowa[0],2);
        int column = binarytodecimal(&columna[0],4);

        int sboxint = lookupSbox[i][row][column];
        unsigned binarytemp[4];
        //std::cout<<"\nsboxint: "<<sboxint;
        decimaltobinary(sboxint,&binarytemp[0],4);         
        //arrayout(&binarytemp[0],4,8);
        for (int j =0;j<4;j++){//iterate through the temporary binary array and add to temporary array
            tempb[i*4+j]=binarytemp[j];
        }
    }   
    //P Permutation
    for (int i=0;i<32;i++){
        result[i]=tempb[lookupP[i]];
    }

    return 0;
}

int binarytodecimal(unsigned binarya[],int size){//converts a binary array into an integer value where size is the size of the array
    int result =0;
    for (int i =size;i>0;i--){//reverse iteration through the binary array
        result+= binarya[i-1]*pow(2,size-i);//multiplies each value in the binary arrray by 2 to the power of its position in the array (in small endian order) so the least significant bit *2^0 then the next is *2^1 ...

    }
    return result;
}

void decimaltobinary(int integer,unsigned binarya[],int size){//converts a decimal integer into binary, storing the result at the address of binarya, size is the length of array storing the data
    //std::cout<<"\nint: "<<integer;
    for (int i=size;i>0;i--){//iterating in reverse through the binary array to calulate each position in turn
        binarya[i-1]=integer%2;
        //std::cout<<"\ninteger%2: "<<integer%2;
        integer /=2;
    }
}

void DESkeygenerate(std::string &key,unsigned (*keya)[16][48]){//generates 16 48bit subkeys for DES from a 64 bit key
    //Permutaion PC1
        unsigned keyPC1[2][28];
        unsigned keya64bit[64];
        chartobit(key,keya64bit);
        for (int i=0;i<56;i++){//permuatetes keya64bit using the lookup table
            keyPC1[0][i]=keya64bit[lookupPC1[i]];
        }

        for (int i=0;i<16;i++){//loops through each key generation stage, sub keys are left shifted then compressed
            leftshift(&keyPC1[0][0],28,bitrotationtable[i]);
            leftshift(&keyPC1[1][0],28,bitrotationtable[i]);

            //compression pc2
            for (int j=0;j<48;j++){//iterate through the subkey that will be made from the condensed/permutated PC1 keys that have just been rotated
                (*keya)[i][j]=keyPC1[0][lookupPC2[j]];//creating subkeys in subkey array where i is the subkey number and j is the position in that subkey the position each bit is taken from temp is decided by lookupPC2            
            }
        }
}

void getkey(unsigned (*keya)[16][48],int mode){//takes a key as input and generates the keys required for certain encryption algorithms
    std::string key;
    bool keycheck = 0;
    while (!keycheck){//loop to get 64 bit /8 byte key as input including error checking to ensure key is 8 bytes
        std::cout << "Please input 64 bit key: ";
        std::cin >> key;
        if (key.size()==8){
            keycheck = 1;
        }
    }
    key = "12345678";
    if (mode==0){//change to use some enumeration
        DESkeygenerate(key,keya);
    }
}

void getIV(unsigned (*IVa)[64]){//takes IV input and converts to binary array format
    std::string IV;
    bool IVcheck = 0;
    while (!IVcheck){//loop to get 64 bit /8 byte IV as input including error checking to ensure IV is 8 bytes
        std::cout << "Please input 64 bit IV: ";
        std::cin >> IV;
        if (IV.size()==8){
            IVcheck = 1;
        }
    }
    IV = "87654321";
    chartobit(IV,*IVa);
}

//TODO Delete this
void getfile(){//takes file as input with error checking, requires there to be a valid std::ifstream input to utilise
    std::string pt;
    bool ptcheck = 0;
    while(!ptcheck){
        std::cout << "Please input .txt name: ";
        std::cin >> pt;
        if (pt.find(".txt")!=std::string::npos){
            input.open(pt.c_str());
            if (!input.is_open()){
                std::cout<<"\n-------------------Input directory not found-------------------\n";
                continue;
            }
            ptcheck=1;
        }
    }
}

void hextobit(std::string file_contents,unsigned *array){
    int size = file_contents.size();
    auto fileit = &file_contents[0];
    for (int i=0;i<size;i++){//loops through key characters and turns them into 4 bit representaion, adds that to array
        auto iterator = hexconvert.find(*(fileit+i));
        for (int k =0;k<4;k++){
            *array=iterator->second[k];
            array++;
        }
    }
}

//Parses command line arguments
int parseCommandLineArguments(int argc, char* argv[], Options* encryptOpts){
    std::string outputFileName;
    std::string inputFileName;
    std::string modeString;

    //Using getopt library to parse command line arguments
    //m: for mode
    //o: for output file
    //-: stands for any arguments that don't have options and corresponds with case 1
    int opt;
    while ((opt = getopt(argc, argv, "-:m:o:i:k:")) != -1) {
        switch (opt) {
            case 'm':
                //printf("Option m has arg: %s\n", optarg);
                modeString = std::string(optarg);
                break;
            case 'o':
                //printf("Option o has arg: %s\n", optarg);
                outputFileName = optarg;
                break;
            case 'i':
                //TODO handle command line IV
                break;
            case 'k':
                //TODO handle command line key
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
                //Set the input file name to the first non-option argument
                //if inputFileName has already been set then there are too many commandline arguments
                if (inputFileName.empty()){
                    inputFileName = optarg;
                    break;
                }
                else {
                    return -1;
                }
        }
    }

    // Handle input file not being set
    if (inputFileName.empty()){
        std::cout << "[-] Please provide a file to encrypt\n";
        return -1;
    }
    else {
        input.open(inputFileName.c_str());
        if (!input.is_open()){
            std::cout<<"[-] ------------ Can't open input file ------------\n";
            return -2;
        }
    }

    // Handle output file not being set
    if (outputFileName.empty()){
        std::cout << "[+] Output file name was not set, using encrypted.txt as output\n";
        outputFileName = "encrypted.txt";
    }

    // Handle mode option, either a mode arguement was never passed and the default will be chosen 
    // or a mode has been passed and should be parsed
    if (modeString.empty()){
        std::cout << "[+] No method of encryption was chosen, using default (ECB Mode)\n";
        (*encryptOpts).mode = ECB;
        (*encryptOpts).encrypt_size = 64;
        (*encryptOpts).encryptmethod = DES_m;
    }
    else {
        if (parseEncryptionModeArg(modeString, encryptOpts) == -1){
            std::cout<<"[-] No valid encryption mode selected\n";
            return -1;
        }
    }

    //Open output file
    output.open(outputFileName);
    if (!output.is_open()){
        std::cout<<"[-] ------------ Can't open output file ------------\n";
        return -2;
    }

    return 0;
}

int parseEncryptionModeArg(std::string encryptionArg, Options* encryptOpts){
    if (encryptionArg == "ecb" || encryptionArg == "ECB"){
        std::cout<<"[+] Utilising ECB Mode of operation\n";
        (*encryptOpts).mode = ECB;
        (*encryptOpts).encrypt_size = 64;
    }
    else if (encryptionArg == "cbc" || encryptionArg == "CBC"){
        std::cout<<"[+] Utilising CBC Mode of operation\n";
        (*encryptOpts).mode = CBC;
    }
    else if (encryptionArg == "pcbc" || encryptionArg == "PCBC"){
        std::cout<<"[+] Utilising PCBC Mode of operation\n";
        (*encryptOpts).mode = PCBC;
    }
    else if (encryptionArg == "cfb" || encryptionArg == "CFB"){
        std::cout<<"[+] Utilising CFB Mode of operation\n";
        (*encryptOpts).mode = CFB;
        (*encryptOpts).encrypt_size = 8;
    }
    else if (encryptionArg == "ofb" || encryptionArg == "OFB"){
        std::cout<<"[+] Utilising OFB Mode of operation\n";
        (*encryptOpts).mode = OFB;
        (*encryptOpts).encrypt_size = 8;
    }
    else {
        return -1;
    }

    //Currently hardcoding this as only one encryption method has been developed for the time being
    (*encryptOpts).encryptmethod = DES_m;
    return 0;
}

void printUsage(){
    std::cout << "DES Encryptor\n\n\
Usage:\n\
    encrypt inFile [-o outFile] [-m mode]\n\
\n\
Options:\n\
    -h --help     Show this screen.\n\
    -o outFile    Output file\n\
    -m mode       Method of encryption [ECB | CBC | PCBC | CFB | OFB]\n";
}
