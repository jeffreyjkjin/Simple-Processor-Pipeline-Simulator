#include <queue>
#include <string>
#include <unordered_map>

#pragma once

using std::deque;
using std::pair;
using std::string;
using std::unordered_map;

// Keeps track of structural, control and data dependencies in the program.
class DTracker {
    public:
        unsigned stageCount[5] = {0}; // Keeps track of how many instructions are in each stage.
        unsigned nextInstr[5] = {0};  // Keeps track of next instruction to be processed in each stage.
        string iBusy[5];              // Keeps track of structural/control hazards for each instruction.

        // Used to track if an instruction has been "completed" and can be used as a dependency.
        // Pairs store each occurence of an instruction with the same PC.
        unordered_map<string, deque<pair<unsigned, bool>>> instrs;
};