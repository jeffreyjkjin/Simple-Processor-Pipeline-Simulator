#include <instruction.hpp>

#include <queue>

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
        // DESC: Constructs a new event list and schedules IF events for the first width-th instructions.
        EventList();
        // DESC: Removes the first event in the queue.
        // POST: Returns the removed event.
        Event pop();
        void processIF();
        void processID();
        void processEX();
        void processMEM();
        void processWB();
};