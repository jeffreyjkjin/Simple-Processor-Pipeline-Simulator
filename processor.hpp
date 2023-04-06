#include <queue>

#include "iqueue.hpp"

#pragma once

// Queue of instructions that are going through the processor pipeline. 
class Processor {
    private:
        deque<Instruction> q; // Queue of instructions in the processor.

    public:
        unsigned sCount[5]; // Keeps track of number of instructions in each stage.

        // Used to check for structural hazards.
        string IntegerEXBusy; // PC of instruction using the integer ALU unit.
        string FloatEXBusy;   // PC of instruction using the floating point unit.
        string BranchEXBusy;  // PC of instruction using the branch execution unit.
        string LoadMEMBusy;   // PC of instruction using the read port.
        string StoreMEMBusy;  // PC of instruction using the write port.

        //  DESC: Creates a new processor and queues up the first width number of instructions.
        // PARAM: iQ - A queue with instructions that will be sent to the processor queue.
        //        width - The maximum number of instructions that can be in the IF stage.
        Processor(IQueue &iQ, int width);
        //  DESC: Sends an instruction to the processor if there is room in the IF stage.
        // PARAM: instr - The instruction that will be added.
        //        width - The maximum number of instructions that can be in the IF stage.
        //  POST: Returns if the instruction was added successfully.
        bool insertIF(Instruction instr, int width);
        //  DESC: Removes the provided instruction from the processor queue.
        //   PRE: Instruction must be in the processor.
        // PARAM: instr - The instruction that will be removed.
        void remove(Instruction instr);
        // DESC: Returns the current number of instructions in the processor queue.
        unsigned size() const;
        // DESC: Returns iterator for first instruction in processor queue.
        const deque<Instruction>::iterator begin();
        // DeSC: Returns iterator for last instruction in processor queue.
        const deque<Instruction>::iterator end();
};