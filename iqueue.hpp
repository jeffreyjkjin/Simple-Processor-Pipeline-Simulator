#include <fstream>
#include <queue>

#include "instruction.hpp"

#pragma once

using std::queue;

// Queue of instructions read from the provided file.
class IQueue {
    private:
        queue<Instruction> q; // Holds the program's instructions.

    public:
        //  DESC: Populates a queue with instructions from the file.
        //   PRE: Assumes file is valid.
        // PARAM: file - Trace file that will be read.
        //        lineStart - First line of the file that will be read.
        //        instrCount - Number of instructions to be read from the file.
        IQueue(const string fileName, const int startLine, const int instrCount);
        // DESC: Removes the first instruction at the front of the queue.
        //  PRE: Queue must not be empty.
        void pop();
        // DESC: Retrieves the first instruction in the queue.
        //  PRE: Queue must not be empty.
        Instruction front() const;
        // DESC: Checks whether the queue is empty or not.
        bool isEmpty() const;
};