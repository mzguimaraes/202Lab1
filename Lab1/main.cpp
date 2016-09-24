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

const int MAX_ADDRESS = 600;

struct symbol {
    string name;
    int absAddress;
    int moduleNum;
    bool used;
    
    symbol(string name, int absAddress, int modNum) : name(name), absAddress(absAddress), used(false), moduleNum(modNum) {};
    symbol() : used(false) {};
};

struct useListEntry {
    string symbolName;
    bool used;
    useListEntry() : used(false) {};
};

struct module {
    int moduleNum;
    int absAddress;
    vector<int> words;
    vector<useListEntry> uses;
};

void passOne(ifstream& input, vector<symbol>* symbolTable, vector<module*>* modules);
void passTwo(vector<symbol>* symbolTable, vector<module*>* modules);
void parseWord(const int word, vector<symbol>* symbolTable, module* mod);
void checkForErrors(vector<symbol>* symbolTable, vector<module*>* modules);

int main(int argc, const char * argv[]) {
    cout << endl;

    if (argc != 2) {
        cerr << "ERROR: please provide exactly one argument specifying the input filename" << endl;
        exit(EXIT_FAILURE);
    }
    ifstream input(argv[1]);
    if (!input.is_open()) {
        cerr << "File not found" << endl;
        exit(EXIT_FAILURE);
    }
    
    vector<symbol>* symbolTable = new vector<symbol>();
    vector<module*>* modules = new vector<module*>();

    passOne(input, symbolTable, modules);
    
    cout << "Symbol Table:" << endl;
    for (int i = 0; i < symbolTable->size(); i++) {
        symbol sym = symbolTable->at(i);
        cout << sym.name << ": " << sym.absAddress << endl;
    }
    cout << endl;
    
    input.close();
    
    passTwo(symbolTable, modules);
    
    cout << endl;
    
    checkForErrors(symbolTable, modules);
    
    //delete structs
    
    modules->clear();
    delete symbolTable;
    delete modules;
}

void passOne(ifstream& input, vector<symbol>* symbolTable, vector<module*>* modules) {
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
        module* mod = new module();
        mod->moduleNum = i;
        mod->absAddress = currAddress;
        
        //read and create symbols in this module
        int numDefs;
        input >> numDefs;
        for (int j = 0; j < numDefs; j++) {
            symbol sym;
            sym.moduleNum = mod->moduleNum;
            input >> sym.name;
            int relAddress;
            input >> relAddress;
            sym.absAddress = relAddress + mod->absAddress;
            bool alreadyDefined = false;
            for (int k = 0; k < symbolTable->size(); k ++) {
                if (symbolTable->at(k).name == sym.name) {
                    alreadyDefined = true;
                }
            }
            if (alreadyDefined) {
                cerr << "ERROR: multiple definitions of " << sym.name << ".  Using first definition. " << endl;
                
            }
            else {
                symbolTable->push_back(sym);
            }
        }
        
        //fill in rest of mod values
        int numUses;
        input >> numUses;
        for (int j = 0; j < numUses; j++) {
            string usedSymbol;
            input >> usedSymbol;
            
            useListEntry ule;
            ule.symbolName = usedSymbol;
            mod->uses.push_back(ule);
        }
        
        int numWords;
        input >> numWords;
        for (int j = 0; j < numWords; j++) {
            int word;
            input >> word;
            
            mod->words.push_back(word);
        }
        
        for (int k = 0; k < symbolTable->size(); k ++) {
            symbol sym = symbolTable->at(k);
            if (sym.moduleNum == mod->moduleNum) {
                if (sym.absAddress < mod->absAddress || sym.absAddress > mod->absAddress + numWords - 1) {
                    cerr << "WARNING: symbol " << sym.name << " address out of range.  Using module's base address. " << endl;
                    sym.absAddress = mod->absAddress;
                }
            }
        }
        
        currAddress += numWords;
        modules->push_back(mod);
    }
}

void passTwo(vector<symbol>* symbolTable, vector<module*>* modules) {
    //pass 2:
    //for each module:
    //print base address
    //for each word in words[]:
    //resolve address and print to output
    cout << "Memory Map:" << endl;
    for (int i = 0; i < modules->size(); i ++) {
        module* mod = modules->at(i);
        //cout << "+" << mod->absAddress << endl;
        for (int j = 0; j < mod->words.size(); j ++) {
            cout << " " << j + mod->absAddress << ": ";
            //parse word
            parseWord(mod->words.at(j), symbolTable, mod);
            cout << endl;
        }
    }
}

void parseWord(const int word, vector<symbol>* symbolTable, module* mod) {
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
        if (address > MAX_ADDRESS) {
            cerr << "WARNING: Absolute address exceeds machine size; zero used. ";
            address = 000;
        }
    }
    else if (mode == 3) {
        address += mod->absAddress;
        if (address > mod->absAddress + mod->words.size()) {
            cerr << "WARNING: Relative address exceeds module size; zero used. ";
            address = 000;
        }
        
    }
    else if (mode == 4) {
        if (address > mod->uses.size() - 1) {
            cerr << "WARNING: address " << address << " out of range of use list; treated as immediate. ";
        }
        else {
            useListEntry ule = mod->uses.at(address);
            string symbolUsed = ule.symbolName;
            bool symbolFound = false;
            
            for (int i = 0; i < symbolTable->size(); i ++) {
                if (symbolTable->at(i).name == symbolUsed) {
                    symbolFound = true;
                    symbolTable->at(i).used = true;
                    ule.used = true;
                    mod->uses.at(address) = ule;
                    address = symbolTable->at(i).absAddress;
                    break;
                }
            }
            
            if (!symbolFound) {
                cerr << "WARNING: symbol " << symbolUsed << " not defined.  Using value 0. ";
                address = 000;
            }
        }
    }
    else {
        cerr << "WARNING: invalid address type. " << endl;
    }
    
    cout << opcode;
    cout << setw(3) << setfill('0') << address;
    
}

void checkForErrors(vector<symbol>* symbolTable, vector<module*>* modules) {
    for (int i = 0; i < symbolTable->size(); i ++) {
        if (!symbolTable->at(i).used) {
            cerr << "WARNING: " << symbolTable->at(i).name << " was defined in module " << symbolTable->at(i).moduleNum << " but never used. " << endl;
        }
    }
    
    for (int i = 0; i < modules->size(); i ++) {
        module* mod = modules->at(i);
        for (int j = 0; j < mod->uses.size(); j ++) {
            if (!mod->uses.at(j).used) {
                cerr << "WARNING: In module " << mod->moduleNum << " " << mod->uses.at(j).symbolName << " is on use list but isn't used. " << endl;
            }
        }
    }
}















