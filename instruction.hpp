#include <vector>
#include <string>

#pragma once

using std::string;
using std::vector;

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
        string PC;                 // Hexadecimal value representing the instruction's address.
        iType type;                // The type of the instruction; decides which execution units will be used. 
        vector<string> dependents; // List of PC values of instructions that the current instruction is dependent on.
        unsigned number;           // Stores the line number of the associated instruction.

        //  DESC: Creates a new instruction with the provided parameters.
        // PARAM: PC - Program counter.
        //        type - Type of instruction.
        //        dependents - List of dependencies that the current instruction relies on.
        Instruction(const string PC, const iType type, const vector<string> dependents, const unsigned number): 
            PC(PC), type(type), dependents(dependents), number(number) {};
};