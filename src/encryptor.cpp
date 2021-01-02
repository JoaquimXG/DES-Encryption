#include "../include/permutations.h"
#include "../include/functions.h"
#include "../include/Options.h"
#include <iomanip>
#include <getopt.h>

std::ofstream output;
std::ifstream input;

int handleCommandLineArguments(int argc, char* argv[], Options* encryptOpts);
int parseEncryptionModeArg(std::string, Options* encryptOpts);
void printUsage();

int main (int argc, char* argv[]){

    Options encryptOpts;
    if (handleCommandLineArguments(argc, argv, &encryptOpts) == -1){
        printUsage();
        return -1;
    };

    int mode = encryptOpts.mode;
    int encryptmethod = encryptOpts.encryptmethod;
    int encrypt_size = encryptOpts.encrypt_size;
    return 0;

    unsigned IVa[64];
    getIV(&IVa);//takes IV input and converts to binary array
    
    unsigned keya[16][48];
    getkey(&keya);//gets key and generates subkeys

    getfile();//gets file with error checking

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

    arrayout(output,file_contents.size()+padding,555500,1,1);
    arrayout(output,file_contents.size()+padding,16,0,1);

    return 0;
}


//Parses command line arguments
int handleCommandLineArguments(int argc, char* argv[], Options* encryptOpts){
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
