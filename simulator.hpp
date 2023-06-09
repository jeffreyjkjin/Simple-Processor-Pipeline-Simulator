#include <chrono>
#include <fstream>
#include <string>

#pragma once

using std::milli;
using std::string;
using std::chrono::duration;

class Simulator {
    private:
        string fileName; // Name of the trace file.
        int startLine;   // First line that will be read in the file.
        int instrCount;  // Number of instructions that will be read from the file.
        int width;       // Number of pipelines in the processor.

        unsigned totalInstructions;      // The total number of instructions executed.
        unsigned clockCycle;             // The current clock cycle of the processor.
        unsigned iCount[5];              // Counts the number of instructions of each type.
        duration<double, milli> runTime; // The total run time of the simulation.

        // DESC: Outputs statistics about the simulation.
        void print() const;        
    public:
        //  DESC: Constructs a new simulator with the provided parameters.
        // PARAM: file - Trace file that will be read.
        //        startLine - First line of the file that will be read.
        //        instrCount - Last line of the file that will be read.
        //        width - Number of pipelines for the processor.
        Simulator(const string fileName, const int startLine, const int instrCount, const int width);
        // DESC: Starts the simulation. Keeps track of and prints out statistics periodically.
        //  PRE: Assumes all simulation parameters are valid.
        void start();
};