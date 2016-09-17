//
//  main.cpp
//  Lab1
//
//  Created by Marcus Guimaraes on 9/17/16.
//  Copyright © 2016 Marcus Guimaraes. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct symbol {
    string name;
    int absAddress;
};

struct module {
    int moduleNum;
    int absAddress;
    int words[];
    symbol* uses[];
    
};

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
