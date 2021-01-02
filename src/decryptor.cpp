#include "../include/permutations.h"
#include "../include/functions.h"

std::ifstream input;
std::ofstream output;

int main(int argc,char *argv[]){

    enum mode
    {
        ECB,CBC,PCBC,CFB,OFB
    };
    int mode = ECB;//0 for ECB, 1 for CBC,2 for PCBC,3 for cfb
    enum encrypt
    {
        DES_m,AES_m
    };
    int encryptmethod = DES_m;//currently not in use but will be for AES and other encryption algorthms
    int encrypt_size = 64;//decides the size of each block of encryption , used for the stream cypher modes (CFB ,OFB)

    output.open("Assisting/decryptor-dump.csv");
    if (!output.is_open()){
        std::cout<<"\n------------output directory not found-----------";
        return 1;
    }

    if (argc>1){//chosing mode of operation 
        std::string argv1 = argv[1];
        if (argv1=="cbc"){
            std::cout<<"\nUtilising CBC Mode of operation\n";
            mode = CBC;
        }
        else if (argv1=="pcbc"){
            std::cout<<"\nUtilising PCBC Mode of operation\n";
            mode =PCBC;
        }
        else if (argv1=="cfb"){
            std::cout<<"\nUtilising CFB Mode of operation\n";
            mode =CFB;
            encrypt_size=8;
        }
        else if (argv1=="ofb"){
            std::cout<<"\nUtilising OFB Mode of operation\n";
            mode =OFB;
            encrypt_size=8;
        }
        else{
            std::cout<<"\nUtilising ECB Mode of operation\n";
        }
    }

    unsigned IVa[64];
    getIV(&IVa);//takes IV input and converts to binary array

    unsigned keya[16][48];
    getkey(&keya);//gets key and generates subkeys

    getfile();//gets file with error checking

    //converting hex format txt file to string variable, measuring the length, padding and creating a suitable length array to contain cypher text binary
    std::string file_contents { std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>() };//consrtuct string varaible by iterating through file buffer
    int padding = (16-file_contents.size()%16)%16;//calculate the length of the last chunk of data to be encrypted and the difference from 16 nibbles //padding = the number of characters needed to add to bring the string up to a multiple of 8 bytes, i this case the character is a hex so is worth 4 bits in padding
    int encryption_block_num = (file_contents.size()+padding)/16;
    unsigned cta[encryption_block_num][64];

    hextobit(file_contents,&cta[0][0]);//converts hex string to binary array format

    if (padding>0){//if padding is required this loop will add padding to the end of the plaintext array
        unsigned *tempit = &cta[(encryption_block_num)-1][64-1];//pointer to the last position in array
        for (int i=0;i<padding*4;i++){
            *tempit=0;
            tempit--;
        }
    }

    //decryption stage

    unsigned workinga[2][32];//just for temporary storage
    unsigned ptbit[encryption_block_num][64];//to store the result of the decryption

    //block cypher modes
    if (mode ==ECB||mode==CBC||mode==PCBC){
        for (int i=0;i<encryption_block_num;i++){

            DES(&cta[0],&keya,&workinga,&ptbit[0],i,0,1);

            if (mode ==CBC||mode ==PCBC){
                XOR(IVa,&ptbit[i][0],&ptbit[i][0],64);//xor current IV with current plaintext(which is the output of the decryption)
                std::copy(&cta[i][0],&cta[i][64],&IVa[0]);//copy the pre-decryption cyphertext into the IV to be used next time
                if (mode ==PCBC){
                    XOR(IVa,&ptbit[i][0],IVa,64);
                }
            }
        }
    }

    //stream cypher modes
    if (mode==CFB||mode==OFB){
        for (int i=0;i<64*encryption_block_num/encrypt_size;i++){
            //encryption
            DES(&IVa,&keya,&workinga,&ptbit[0],i,1);//encrypt current IV storing the result in the working array (Ek(IV))
            XOR(&workinga[0][0],&cta[0][(i*encrypt_size)],&ptbit[0][(i*encrypt_size)],encrypt_size);// XOR result of above with c(i-1) storing the result in the plaintext array
            //IV preparation
            leftshift(IVa,64,8);
            
            for (int k=0;k<encrypt_size;k++){
                if (mode ==CFB){
                    IVa[63-k]=cta[0][(i*encrypt_size)+((encrypt_size-1)-k)];//append the most recent 8 bits of the cypher text array to the end of the IV ready for the next stage of encryption
                }
                else{
                    IVa[63-k]=workinga[0][encrypt_size-1-k];
                }
            }

        }
    }

    //outputting results
    char output[file_contents.size()/2];//2 hex characters = one (char) so the output file is half the size of the input
    for (int i =0;i<file_contents.size()+padding;i++){//iterate through the results array (ptbit) and conver each 8 bit section into a char 
        char tempc = (char)binarytodecimal(&ptbit[0][i*8],8);
        output[i]=tempc;
    }
    arrayout(output,file_contents.size()/2,500000);

    return 0;
}
