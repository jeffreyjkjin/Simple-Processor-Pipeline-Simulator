#include <queue>

#include "dtracker.hpp"
#include "event.hpp"
#include "iqueue.hpp"

#pragma once

using std::queue;

// Queue of events that occur in the simulation.
class EventList {
    private:
        queue<Event> q; // Stores the events.

    public:
        //  DESC: Constructs a new event list and schedules IF events for the first width-th 
        //        instructions in the instruction queue; stops fetching if a branch is fetched.
        // PARAM: dT - Tracks dependencies for program instructions.
        //        iQ - Queue of instructions that haven't been fetched yet.
        //        width - The maximum number of instructions that can be in the IF stage.
        EventList(DTracker &dT, IQueue &iQ, const int width);
        // DESC: Removes the first event in the eventlist.
        //  PRE: Assume eventlist is not empty.
        void pop();
        // DESC: Returns the first event in the eventlist.
        //  PRE: Assume eventlist is not empty.
        Event front() const;
        // DESC: Returns the number of events in the eventlist.
        int size() const;
        //  DESC: Schedules at most, width-th instructions into the event list in the IF stage.
        // PARAM: dT - Tracks dependencies for program instructions.
        //        iQ - Queue of instructions that haven't been fetched yet.
        //        width - The maximum number of instructions that can be in the IF stage.
        void fetch(DTracker &dT, IQueue &iQ, const int width);
        //  DESC: The current instruction is processed in the IF stage.
        // PARAM: dT - Tracks dependencies for program instructions.
        //        width - Maximum number of instructions that can be in each stage.
        //  POST: The current instruction moves to the ID stage; remains in the current stage if 
        //        the next stage is full or they are not the next instruction in program order.
        void processIF(DTracker &dT, const int width);
        //  DESC: The current instruction is processed in the IF stage. Integer and float
        //        instructions utilize their corresponding structural dependencies.
        // PARAM: dT - Tracks dependencies for program instructions.
        //        width - Maximum number of instructions that can be in each stage.
        //  POST: The current instruction moves to the EX stage; remains in the current stage if 
        //        the next stage is full or they are not the next instruction in program order. 
        //        Integer and float instructions only move onto the EX stage if their corresponding
        //        structural and data dependencies are satisfied.
        void processID(DTracker &dT, const int width);
        //  DESC: The current instruction is processed in the ID stage. Integer, float and branches
        //        finish using their structural dependencies. Load and store instructions
        //        occupy their structural dependencies.
        // PARAM: dT - Tracks dependencies for program instructions.
        //        width - Maximum number of instructions that can be in each stage.
        //  POST: The current instruction moves to the MEM stage; remains in the current stage if 
        //        the next stage is full or they are not the next instruction in program order. 
        //        Integer, float and branch structural dependencies are available after the 
        //        corresponding instruction finishes this stage and their PC addresses are stored 
        //        in dT.instrs so they can be used as dependencies for other instructions. Load and
        //        store instructions only move onto the MEM stage if their structural and data 
        //        dependencies are satisifed.
        void processEX(DTracker &dT, const int width);
        //  DESC: The current instruction is processed in the MEM stage. Load and store
        //        instructions finish using their structural dependencies. 
        // PARAM: dT - Tracks dependencies for program instructions.
        //        width - Maximum number of instructions that can be in each stage.
        //  POST: The current instruction moves to the WB stage; remains in the current stage if 
        //        the next stage is full they are not the next instruction in program order. Load 
        //        and store structural dependencies are available ater the corresponding instruction 
        //        finishes this stage and their PC addresses are stored in dT.instrs so they can be
        //        used as dependencies for other instructions.
        void processMEM(DTracker &dT, const int width);
        //  DESC: The current instruction is processed in the WB stage.
        //  POST: The current instruction is retired and no longer exists in the event list.
        void processWB(DTracker &dT);
};