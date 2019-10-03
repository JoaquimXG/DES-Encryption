#include "Functions.h"
#include "Permutations.h"

void DES(unsigned (*pta)[64],unsigned (*subkeya)[16][48],unsigned (*ct)[2][32],unsigned (*ctbit)[64],int i,int mode, int decrypt){

    //Initial Permutation (and split in half)
    unsigned *aiterator = ct[0][0];
    if (mode==0){
        for (int k=0;k<64;k++){//iterates through plaintext and moves each value to a new position in ct designated by the lookupIP array
            *aiterator=pta[i][lookupIP[k]];
            aiterator++;
        }
    }
    else if (mode ==1){
        for (int k=0;k<64;k++){//iterates through plaintext and moves each value to a new position in ct designated by the lookupIP array
            *aiterator=(*pta)[lookupIP[k]];
            aiterator++;
        }
    }
    


    for (int k=0;k<16;k++){
        unsigned result[32];

        if (decrypt ==true){//decryption mode
            feistal((*ct)[1],&result[0],(*subkeya)[15-k]);//performs feistal function on right side returning the result and leaving ct unchanged 
        }
        else{//encryption mode 
            feistal((*ct)[1],&result[0],(*subkeya)[k]);//performs feistal function on right side returning the result and leaving ct unchanged 

        }        

        XOR(&result[0],(*ct)[0],(*ct)[0],32);//XOR result with the left side
        

        leftshift((*ct)[0],64,32);//swaps left with right
        
    }
    leftshift((*ct)[0],64,32);//swapping sides again because the final swap inside loop is unintended
    //arrayout(ct[0][0],64,8);
    //final permutation
    unsigned temporarya[64];
    
    for (int k=0;k<64;k++){
            temporarya[k]=ct[0][0][lookupfp[k]]; //how the fuck does this work
    }
    for (int k=0;k<64;k++){ //really need to find a better solution than this 
        if (mode == 0){//standard operation
            ctbit[i][k]=temporarya[k];
        }
        if (mode==1){//storing the result at ct and not editing ctbit 
            (*ct)[0][k]=temporarya[k];
        }
        
    }
    

}

