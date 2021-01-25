#ifndef DESENCRYPTION_INCLUDE_DEBUGGGER_H_
#define DESENCRYPTION_INCLUDE_DEBUGGGER_H_
#include <iostream>
#include "./CryptOption.h"

class Debugger {
  bool debug;
  public:
    Debugger(CryptOption opt): debug(opt.debug){};

    void print(std::string input){
      if (debug){
          std::cout << input;
      }
    }
};

#endif
