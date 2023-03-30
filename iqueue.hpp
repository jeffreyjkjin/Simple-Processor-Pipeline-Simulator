#include <fstream>
#include <queue>

#include "instruction.hpp"

#pragma once

using namespace std;

// Queue of instructions read from the provided file.
class IQueue {
    private:
        queue<Instruction> q; // Queue that holds the program's instructions.

    public:
        //  DESC: Populates a queue with instructions from the file.
        //   PRE: Assumes file is valid.
        // PARAM: file - Trace file that will be read.
        //        lineStart - First line of the file that will be read.
        //        lineEnd - Last line of the file that will be read.
        IQueue(string fileName, int startLine, int endLine);
        //  PRE: Queue must not be empty.
        // DESC: Removes the first instruction at the front of the queue.
        // POST: Returns the removed instruction.
        Instruction pop();
        // DESC: Checks whether the queue is empty or not.
        // POST: Returns boolean for if the queue is empty.
        bool isEmpty();
};