#include <vector>

#pragma once

using namespace std;

// Type of program instructions.
typedef enum {
   Integer = 1,
   Float = 2,
   Branch = 3,
   Load = 4,
   Store = 5 
} iType;

// Contains the attributes of a single instruction.
class Instruction {
    public:
        string PC;                 // Base 10 (from hexadecimal) value representing the instruction address.
        iType type;                // The type of the instruction.
        vector<string> dependents; // List of PC values of instructions that the current instruction is dependent on.

        //  DESC: Creates a new instruction with the provided parameters.
        // PARAM: PC - Program counter.
        //        type - Type of instruction.
        //        dependents - List of dependencies that the current instruction relies on.
        Instruction(string PC, iType type, vector<string> dependents): PC(PC), type(type), dependents(dependents) {};
};