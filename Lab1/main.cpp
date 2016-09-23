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
#include <iomanip>
using namespace std;

struct symbol {
    string name;
    int absAddress;
    
    symbol(string name, int absAddress) : name(name), absAddress(absAddress) {};
    symbol() {};
};

struct module {
    int moduleNum;
    int absAddress;
    vector<int> words;
    vector<string> uses;
    
    //module(int moduleNum, int absAddress) : moduleNum(moduleNum), absAddress(absAddress) {};
};

void passOne(ifstream& input, vector<symbol>* symbolTable, vector<module>* modules);
void passTwo(vector<symbol>* symbolTable, vector<module>* modules);
void parseWord(int word, vector<symbol>* symbolTable, vector<string> uses, int baseAddress);

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
    
    //print to stdout
    cout << "Symbol Table:" << endl;
    for (int i = 0; i < symbolTable->size(); i++) {
        symbol sym = symbolTable->at(i);
        cout << sym.name << ": " << sym.absAddress << endl;
    }
    cout << endl;
    
    // close input
    input.close();
    
    passTwo(symbolTable, modules);
    
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
        //create module struct, add some values to it
        module mod;
        mod.moduleNum = i;
        mod.absAddress = currAddress;
        
        //read and create symbols in this module
        int numDefs;
        input >> numDefs;
        for (int j = 0; j < numDefs; j++) {
            symbol sym;
            input >> sym.name;
            int relAddress;
            input >> relAddress;
            sym.absAddress = relAddress + mod.absAddress;
            symbolTable->push_back(sym);
        }
        
        //fill in rest of mod values
        int numUses;
        input >> numUses;
        for (int j = 0; j < numUses; j++) {
            string usedSymbol;
            input >> usedSymbol;
            
            mod.uses.push_back(usedSymbol);
        }
        
        int numWords;
        input >> numWords;
        for (int j = 0; j < numWords; j++) {
            int word;
            input >> word;
            
            mod.words.push_back(word);
        }
        
        currAddress += numWords;
        modules->push_back(mod);
    }
}

void passTwo(vector<symbol>* symbolTable, vector<module>* modules) {
    //pass 2:
    //for each module:
    //print base address
    //for each word in words[]:
    //resolve address and print to output
    cout << "Memory Map:" << endl;
    for (int i = 0; i < modules->size(); i ++) {
        module mod = modules->at(i);
        cout << "+" << mod.absAddress << endl;
        for (int j = 0; j < mod.words.size(); j ++) {
            cout << " " << j + mod.absAddress << ": ";
            //parse word
            parseWord(mod.words.at(j), symbolTable, mod.uses, mod.absAddress);
            cout << endl;
        }
    }
    
}

void parseWord(int word, vector<symbol>* symbolTable, vector<string> uses, int baseAddress) {
    //5-digit word:
    //digit 1: opcode
    //digits 2-4: address
    //digit 5: mode:
        //1 = immediate, 2 = absolute, 3 = relative, 4 = external
    int opcode = word / 10000;
    int address = (word % 10000) / 10;
    int mode = word % 10;
    
    if (mode == 1) {
        //ignore
    }
    else if (mode == 2) {
        //ignore
    }
    else if (mode == 3) {
        address += baseAddress;
        
    }
    else if (mode == 4) {
        string symbolUsed = uses.at(address);
        bool symbolFound = false;
        for (int i = 0; i < symbolTable->size(); i ++) {
            if (symbolTable->at(i).name == symbolUsed) {
                address = symbolTable->at(i).absAddress;
                symbolFound = true;
            }
        }
        if (!symbolFound) {
            cerr << "WARNING: symbol not found.  Using value 0";
        }
    }
    else {
        cerr << "WARNING: invalid address type" << endl;
    }
    
    cout << opcode;
    cout << setw(3) << setfill('0') << address;
    //cout << opcode << " " << address << " " << mode;
}















