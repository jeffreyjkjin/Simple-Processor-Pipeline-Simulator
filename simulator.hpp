#include <fstream>

#pragma once

using namespace std;

class Simulator {
    private:
        string fileName; // Name of the trace file.
        int startLine;   // First line that will be read in the file.
        int endLine;     // Last line that will be read in the file.

        unsigned totalInstructions; // The total number of instructions executed.
        unsigned clockCycle;        // The current clock cycle of the processor.
        unsigned iHistogram[5];     // Contains the frequency of each instruction.

        // DESC: Outputs the current clock cycle, current number of executed instructions and
        //       histogram of instructions types.
        void printStatistics();        
    public:
        //  DESC: Constructs a new simulator with the provided parameters.
        // PARAM: file - Trace file that will be read.
        //        startLine - First line of the file that will be read.
        //        endLine - Last line of the file that will be read.
        Simulator(string fileName, int startLine, int endLine);
        // DESC: Starts the simulation. Keeps track of and prints out statistics periodically.
        void start();
};