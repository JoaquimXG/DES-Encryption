#include "../include/CryptOption.h"
#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "Starting Crypt \n";

  CryptOption opt = CryptOption(argc, argv);

  std::cout << opt.toString(opt);
}
