#include "../include/permutations.h"
#include "../include/functions.h"
#include "../include/Options.h"
#include <iomanip>

std::ofstream output;
std::ifstream input;

int main (int argc, char* argv[]){

    Options encryptOpts;

    int result = parseCommandLineArguments(argc, argv, &encryptOpts);
    switch (result){
        //Command line arguments incorrect
        case -1:
            printUsage();
            return -1;
        //File handling failure e.g. can't open file 
        case -2:
            return -2;
    }

    //TODO potentially remove this
    int mode = encryptOpts.mode;
    int encryptmethod = encryptOpts.encryptmethod;
    int encrypt_size = encryptOpts.encrypt_size;
    std::string iv = encryptOpts.iv;
    std::string key = encryptOpts.key;

    unsigned IVa[64];
    chartobit(iv, IVa);
    
    unsigned keya[16][48];
    DESkeygenerate(key, &keya);//gets key and generates subkeys

    //converting file to string variable then to plaintext array and adding padding
    std::string file_contents { std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>() };//consrtuct string varaible by iterating through file buffer
    int padding = (8-file_contents.size()%8)%8;//calculate the length of the last chunk of data to be encrypted and the difference from 8 bytes
    int encryption_block_num = (file_contents.size()+padding)/8;
    unsigned pta[encryption_block_num][64];


    chartobit(file_contents,&pta[0][0]);
    if (padding>0){//if padding is required this loop will add padding to the end of the plaintext array
        unsigned *tempit = &pta[(encryption_block_num)-1][64-1];//pointer to the last position in array
        for (int i=0;i<padding*8;i++){
            *tempit=0;
            tempit--;
        }
    }

    //Text encryption
    unsigned ctbit[encryption_block_num][64];//cypher text binary output to be converted to hex or ascii
    unsigned w_array[64];//working array for temporary data
    unsigned ct[2][32];//working array for inside DES function, also used for storing the result of DES when the result can't be saved to ctbit

    for (int i=0;i<encryption_block_num;i++){//iterate through the data 8 bytes at a time, add is the padding required so the string variable is a multiple of 8 bytes

        if (i==0){//operations performed before any encryption
            if (mode==CBC||mode ==PCBC){//XOR plaintext with IV before any encyption (currently using for CBC and PCBC)
                std::copy(&pta[i][0], &pta[i][64], &w_array[0]);
                XOR(&pta[i][0],&IVa[0],&pta[i][0],64);
            }
            else if (mode ==CFB||mode ==OFB){//IV encrypted using DES
                for (int k=0;k<(64/encrypt_size);k++)//iterate through each encryption block that is a portion of the current block(64) if encrypt size is 64 then this will only go throuhg a single loop if e_size is 16 then it will loop 4 times (16*4)=64
                {
                    if (k==0){
                        DES(&IVa,&keya,&ct,&ctbit[0],i,1);//encrypt the iv using DES and store the result in ct
                        XOR(&pta[i][k*encrypt_size],&ct[0][0],&ctbit[i][k*encrypt_size],encrypt_size);//XOR the result of above encryption with the plaintext and store the result at the current ctbit address
                    }
                    if (k!=0){
                        leftshift(IVa,64,encrypt_size);//change iva to hold the results of the above XOR
                        for (int l=0;l<encrypt_size;l++){//after lefshift this loop fills the LSB's in IVa with the result of recent xor in ctbit
                            if (mode==CFB){
                                IVa[64-encrypt_size+l]=ctbit[i][((k-1)*encrypt_size)+l];//fills encrypt size number of bits
                            }
                            else if (mode==OFB){
                                IVa[64-encrypt_size+l]=ct[0][l];
                            }
                        }
                        DES(&IVa,&keya,&ct,&ctbit[0],i,1);//encrypt the iv using DES and store the result in ct
                        XOR(&pta[i][k*encrypt_size],&ct[0][0],&ctbit[i][k*encrypt_size],encrypt_size);//XOR the result of above encryption with the plaintext and store the result at the current ctbit address
                    }
                }
            }
        }

        else if (i!=0){//operations performed after first round of encryption and for all rounds following
            if(mode==CBC){
                XOR(&pta[i][0],&ctbit[i-1][0],&pta[i][0],64);//XOR current plaintext with previous cypher text
            }
            else if (mode ==PCBC){//operations that require previous plaintext come first
                std::cout<<"here\n";
                XOR(&w_array[0],&pta[i][0],&ctbit[i][0],64);//XOR previous plaintext with current plaintext storing the result in the cyphertext array temporarily
                std::copy(&pta[i][0], &pta[i][64], &w_array[0]);
                XOR(&ctbit[i][0],&ctbit[i-1][0],&pta[i][0],64);//XOR result of above with previous cypher text storing the address in the "plaintext array", ready for encryption
            }
            else if(mode==CFB||mode ==OFB){
                for (int k=0;k<64/encrypt_size;k++){
                    leftshift(IVa,64,encrypt_size);
                    for (int l=0;l<encrypt_size;l++){//after lefshift this loop fills the LSB's in IVa with the result of recent xor in ctbit
                        if (mode==CFB){
                            IVa[64-encrypt_size+l]=ctbit[i][((k-1)*encrypt_size)+l];//fills encrypt size number of bits
                        }
                        if (mode==OFB){
                            IVa[64-encrypt_size+l]=ct[0][l];
                        }
                    }
                    DES(&IVa,&keya,&ct,&ctbit[0],i,1);//encrypt the iv using DES and store the result in ct
                    XOR(&pta[i][k*encrypt_size],&ct[0][0],&ctbit[i][k*encrypt_size],encrypt_size);//XOR the result of above encryption with the plaintext and store the result at the current ctbit address
                }
            }
        }

        //Block encryption algorithm being used (only used for the block cyphers)
        if (encryptmethod==0&&(mode==ECB||mode==CBC||mode==PCBC)){
            DES(&pta[0],&keya,&ct,&ctbit[0],i);
        }
    }

    //Convert binary ctbit array into characters
    unsigned int output[file_contents.size()];
    for (int i =0;i<file_contents.size()+padding;i++){
        unsigned int tempc = binarytodecimal(&ctbit[0][i*8],8);
        output[i]=tempc;
    }
    std::cout<<std::hex;

    arrayout(output,file_contents.size()+padding,9999999,1,1);
    arrayout(output,file_contents.size()+padding,16,0,1);

    return 0;
}


