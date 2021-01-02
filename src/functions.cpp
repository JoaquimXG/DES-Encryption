#include "../include/functions.h"
#include <string>
#include <iostream>
#include <bitset>
#include <math.h>
#include "../include/permutations.h"
#include <fstream>
#include <map>
#include <iomanip>

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

