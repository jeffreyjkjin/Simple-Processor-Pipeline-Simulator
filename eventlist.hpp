#include "instruction.hpp"

#include <unordered_map>
#include <queue>

#include "iqueue.hpp"
#include "processor.hpp"

#pragma once

using namespace std;

// Events in the event list.
class Event {
    public:
        Stage stage;       // The current stage that the associated instruction is in.
        Instruction instr; // The instruction associated with this event. 

        //  DESC: Creates a new event with the provided parameter.
        // PARAM: stage - The stage that the instruction is currently in.
        //        instr - The instruction associated with this event.
        Event(Stage stage, Instruction &instr) : stage(stage), instr(instr) {};
};

// Queue of events that occur in the simulation.
class EventList {
    private:
        queue<Event> q; // Stores the events.

    public:
        //  DESC: Constructs a new event list and schedules IF events for the first width-th 
        //        instructions fetched by the processor; stops fetching if a branch is fetched.
        //   PRE: Assume processor has the first width-th number of instructions.
        // PARAM: p - Queue of instructions.
        EventList(Processor &p);
        // DESC: Removes the first event in the eventlist.
        //  PRE: Assume eventlist is not empty.
        void pop();
        // DESC: Returns the first event in the eventlist.
        //  PRE: Assume eventlist is not empty.
        Event front() const;
        //  DESC: Schedules a new event for the provided instruction at the back of the queue in 
        //        the IF stage.
        //   PRE: Assume there is room for the new instruction to enter the IF stage.
        // PARAM: instr - The instruction that will be added to the queue.
        void insertIF(Instruction &instr);
        //  DESC: The current instruction is processed in the IF stage.
        // PARAM: instrs - Hashtable containing previous instructions that are in progress or have
        //                 been completed.
        //        p - Processor queue that contains the current instructions.
        //        width - Maximum number of instructions that can be in each stage.
        //  POST: The current instruction moves to the ID stage; remains in the current stage if 
        //        the next stage is full.
        void processIF(unordered_map<string, deque<pair<unsigned, bool>>> &instrs, Processor &p, int width);
        //  DESC: The current instruction is processed in the IF stage. Integer and float
        //        instructions utilize their corresponding execution units.
        // PARAM: instrs - Hashtable containing previous instructions that are in progress or have
        //                 been completed.
        //        p - Processor queue that contains the current instructions.
        //        width - Maximum number of instructions that can be in each stage.
        //  POST: The current instruction moves to the EX stage; remains in the current stage if 
        //        the next stage is full. Integer and float instructions only move onto the EX 
        //        stage if their corresponding execution units are available and dependencies are
        //        satified.
        void processID(unordered_map<string, deque<pair<unsigned, bool>>> &instrs, Processor &p, int width);
        //  DESC: The current instruction is processed in the ID stage. Integer, float and branches
        //        finish using their corresponding execution units. Load and store instructions
        //        occupy their read/write ports.
        // PARAM: instrs - Hashtable containing previous instructions that are in progress or have
        //                 been completed.
        //        p - Processor queue that contains the current instructions.
        //        width - Maximum number of instructions that can be in each stage.
        //  POST: The current instruction moves to the MEM stage; remains in the current stage if 
        //        the next stage is full. Integer, float and branch execution units are available
        //        after the corresponding instruction finishes this stage and their PC addresses 
        //        are stored in instrs so they can be used as dependencies for other instructions.
        //        Load and store instructions only move onto the MEM stage if their read/write 
        //        ports are available and dependencies are satisifed.
        void processEX(unordered_map<string, deque<pair<unsigned, bool>>> &instrs, Processor &p, int width);
        //  DESC: The current instruction is processed in the MEM stage. Load and store
        //        instructions finish using their read/write ports. 
        // PARAM: instrs - Hashtable containing previous instructions that are in progress or have
        //                 been completed.
        //        p - Processor queue that contains the current instructions.
        //        width - Maximum number of instructions that can be in each stage.
        //  POST: The current instruction moves to the WB stage; remains in the current stage if 
        //        the next stage is full. Load and store read/write ports are available ater the
        //        corresponding instruction finishes this stage and their PC addresses are stored
        //        in instrs so they can be used as dependencies for other instructions.
        void processMEM(unordered_map<string, deque<pair<unsigned, bool>>> &instrs, Processor &p, int width);
        //  DESC: The current instruction is processed in the WB stage.
        // PARAM: p - Processor queue that contains the current instructions.
        //  POST: The current instruction is removed from the processor.
        void processWB(Processor &p);
};