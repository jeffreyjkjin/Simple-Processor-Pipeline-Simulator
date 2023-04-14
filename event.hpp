#include "instruction.hpp"

#pragma once

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
        Stage stage;       // The current stage that the associated instruction is in.
        Instruction instr; // The instruction associated with this event. 

        //  DESC: Creates a new event with the provided parameter.
        // PARAM: stage - The stage that the instruction is currently in.
        //        instr - The instruction associated with this event.
        Event(Stage stage, Instruction instr) : stage(stage), instr(instr) {};
};
