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
#include <vector>
using namespace std;

struct symbol {
    string name;
    int absAddress;
    
    symbol(string name, int absAddress) : name(name), absAddress(absAddress) {};
};

struct module {
    int moduleNum;
    int absAddress;
    vector<int> words;
    symbol* uses[];
    
    //module(int moduleNum, int absAddress) : moduleNum(moduleNum), absAddress(absAddress) {};
};

void passOne(ifstream& input, vector<symbol>* symbolTable, vector<module>* modules);

int main(int argc, const char * argv[]) {
    //get input filename, open
    if (argc != 2) {
        cerr << "ERROR: please provide exactly one argument specifying the input filename" << endl;
        exit(EXIT_FAILURE);
    }
    ifstream input(argv[1]);
    if (!input.is_open()) {
        cerr << "File not found" << endl;
        exit(EXIT_FAILURE);
    }
    
    //test if file is opening
//    string line;
//    while (getline(input, line)) {
//        cout << line << endl;
//    }
    vector<symbol>* symbolTable = new vector<symbol>();
    vector<module>* modules = new vector<module>();

    passOne(input, symbolTable, modules);
    
    //open output file, close input
    input.close();
    //pass 2:
        //for each module:
            //print base address
            //for each word in words[]:
                //resolve address and print to output
    //close output
    //delete structs
}

void passOne(ifstream& input, vector<symbol>* symbolTable, vector<module>* modules) {
    //pass 1:
        //for each module:
            //create module, add to modules
            //get base address
            //create symbols, add to symbol table
    
    int totalNumMods;
    input >> totalNumMods;
    
    int currAddress = 0;
    
    for (int i = 0; i < totalNumMods; i++) {
        
    }
}
















