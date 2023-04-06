#include "instruction.hpp"

#include <unordered_map>
#include <queue>

#include "iqueue.hpp"
#include "processor.hpp"

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

    public:
        //  DESC: Constructs a new event list and schedules IF events for the first width-th instructions.
        //   PRE: Assume processor has the first width number of instructions.
        // PARAM: p - Queue of instructions.
        EventList(Processor &p);
        // DESC: Removes the first event in the eventlist.
        //  PRE: Assume eventlist is not empty.
        void pop();
        // DESC: Returns the first event in the eventlist.
        //  PRE: Assume eventlist is not empty.
        Event front() const;
        //  DESC: Schedules a new event for the provided instruction at the back of the queue in the IF stage.
        //   PRE: Assume there is room for the new instruction to enter the IF stage.
        // PARAM: instr - The instruction that will be added to the queue.
        void insertIF(Instruction &instr);
        void processIF(unordered_map<string, unsigned> &instrs, IQueue &iQ, Processor &p);
        void processID(unsigned clockCycle, unordered_map<string, unsigned> &instrs, Processor &p);
        void processEX(unsigned clockCycle, unordered_map<string, unsigned> &instrs, Processor &p);
        void processMEM(unsigned clockCycle, unordered_map<string, unsigned> &instrs, Processor &p);
        void processWB(Processor &p);
};