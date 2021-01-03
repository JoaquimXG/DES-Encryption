#include "../include/functions.h"
#include "../include/permutations.h"

// TODO Split this up into smaller, more manageable functions
void DES(unsigned (*pta)[64], unsigned (*subkeya)[16][48],
         unsigned (*ct)[2][32], unsigned (*ctbit)[64], int i, int mode,
         int decrypt) {

  // Initial Permutation (and split in half)
  unsigned *aiterator = ct[0][0];
  if (mode == 0) {
    // iterates through plaintext and moves each value to a new position in ct
    // designated by the lookupIP array
    for (int k = 0; k < 64; k++) {
      *aiterator = pta[i][lookupIP[k]];
      aiterator++;
    }
  } else if (mode == 1) {
    // iterates through plaintext and moves each value to a new position in ct
    // designated by the lookupIP array
    for (int k = 0; k < 64; k++) {
      *aiterator = (*pta)[lookupIP[k]];
      aiterator++;
    }
  }

  for (int k = 0; k < 16; k++) {
    unsigned result[32];
    // decryption mode
    if (decrypt == true) {
      // performs feistal function on right side returning the result and
      // leaving ct unchanged
      feistal((*ct)[1], &result[0], (*subkeya)[15 - k]);
    }
    // encryption mode
    else {
      // performs feistal function on right side returning the result and
      // leaving ct unchanged
      feistal((*ct)[1], &result[0], (*subkeya)[k]);
    }
    // XOR result with the left side
    XOR(&result[0], (*ct)[0], (*ct)[0], 32);

    // swaps left with right
    leftShift((*ct)[0], 64, 32);
  }
  // swapping sides again because the final swap inside loop is unintended
  leftShift((*ct)[0], 64, 32);

  // arrayout(ct[0][0],64,8);
  // final permutation
  unsigned temporarya[64];

  for (int k = 0; k < 64; k++) {
    // how the fuck does this work
    temporarya[k] = ct[0][0][lookupfp[k]];
  }
  // really need to find a better solution than this
  for (int k = 0; k < 64; k++) {
    // standard operation
    if (mode == 0) {
      ctbit[i][k] = temporarya[k];
    }
    // storing the result at ct and not editing ctbit
    if (mode == 1) {
      (*ct)[0][k] = temporarya[k];
    }
  }
}
