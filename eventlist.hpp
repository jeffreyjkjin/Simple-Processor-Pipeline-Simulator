#include "instruction.hpp"

#include <unordered_map>
#include <queue>

#include "iqueue.hpp"

#pragma once

using namespace std;

// Stages of the processor.
typedef enum {
    IF = 0,
    ID = 1,
    EX = 2,
    MEM = 3,
    WB = 4
} Stage;

// Events in the event list.
class Event {
    public:
        Stage stage;       // Determines what the simulator will do next with the associated instruction.
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

        // Used to check for structural hazards.
        bool IntegerEXBusy; // Status of integer ALU unit.
        bool FloatEXBusy;   // Status of floating point unit.
        bool BranchEXBusy;  // Status of branch execution unit.
        bool LoadMEMBusy;   // Status of read port.
        bool StoreMEMBusy;  // Status of write port.

    public:
        //  DESC: Constructs a new event list and schedules IF events for the first width-th instructions.
        //   PRE: Assume processor has the first width number of instructions.
        // PARAM: processor - Queue of instructions.
        EventList(deque<Instruction> &processor);
        // DESC: Removes the first event in the eventlist.
        //  PRE: Assume eventlist is not empty.
        void pop();
        // DESC: Returns the first event in the eventlist.
        //  PRE: Assume eventlist is not empty.
        Event front(); 
        void processIF(IQueue &iQ, deque<Instruction> &processor, unordered_map<string, unsigned> &instrs);
        void processID(unordered_map<string, unsigned> &instrs);
        void processEX(unordered_map<string, unsigned> &instrs);
        void processMEM(unordered_map<string, unsigned> &instrs);
        void processWB(deque<Instruction> &processor);
};