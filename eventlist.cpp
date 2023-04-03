#include "eventlist.hpp"

#include <iostream>
using namespace std;

EventList::EventList(deque<Instruction> &processor) {
    // initialize variables
    q = queue<Event>();

    IntegerEXBusy = false;
    FloatEXBusy = false;
    BranchEXBusy = false;
    LoadMEMBusy = false;
    StoreMEMBusy = false;

    // schedule IFs for the first width number of instructions
    for (auto instr: processor) { q.push(Event(IF, instr)); }
    // for (deque<Instruction>::iterator instr = processor.begin(); instr != processor.end(); instr++) { q.push(Event(IF, *instr)); }
 }

void EventList::pop() { q.pop(); }

Event EventList::front() { return q.front(); }

void EventList::processIF(IQueue &iQ, deque<Instruction> &processor, unordered_map<string, unsigned> &instrs) {
    // schedule ID event for current instruction
    Event curr = q.front();
    q.push(Event(ID, curr.instr));

    instrs[curr.instr.PC] = false;

    if (!iQ.isEmpty()) {
        // schedule next instruction
        Instruction instr = iQ.pop();
        q.push(Event(IF, instr));
        processor.push_back(instr);
    }
}

void EventList::processID(unordered_map<string, unsigned> &instrs) {
    Instruction instr = q.front().instr;

    if (instr.type == Integer || instr.type == Float || instr.type == Branch) {
        // checks if corresponding execution unit is available or not
        if (instr.type == Integer && !IntegerEXBusy) { IntegerEXBusy = true; }
        else if (instr.type == Float && !FloatEXBusy) { FloatEXBusy = true; }
        else if (instr.type == Branch && !BranchEXBusy) { BranchEXBusy = true; }
        else if (instr.type == Integer || instr.type == Float || instr.type == Branch) {
            // reschedule event if execution unit not available
            q.push(Event(ID, instr));
            return;
        }

        // check if dependencies are satisfied
        for (auto curr: instr.dependents) {
            if (instrs[curr] < 1) {
                // reschedule event if dependency not satisified
                q.push(Event(ID, instr));
                return;
            }
        }
    }

    q.push(Event(EX, instr));
}

void EventList::processEX(unordered_map<string, unsigned> &instrs) {
    Instruction instr = q.front().instr;

    if (instr.type == Integer || instr.type == Float || instr.type == Branch) {
        // update status of execution units
        if (instr.type == Integer) { IntegerEXBusy = false; }
        if (instr.type == Float) { FloatEXBusy = false; }
        if (instr.type == Branch) { BranchEXBusy = false; }
        
        instrs[instr.PC] = true;
    }
    else {
        // check if read/write ports are available or not
        if (instr.type == Load && !LoadMEMBusy) { LoadMEMBusy = true; }
        else if (instr.type == Store && !StoreMEMBusy) { StoreMEMBusy = true; }
        else if (instr.type == Load || instr.type == Store) {
            // reschedule event if read/write ports not available
            q.push(Event(EX, instr));
            return;
        }

        // check if dependencies are satisfied
        for (auto curr: instr.dependents) {
            if (instrs[curr] < 1) {
                // reschedule event if dependency not satisified
                q.push(Event(EX, instr));
                return;
            }
        }
    }

    q.push(Event(MEM, instr));
}

void EventList::processMEM(unordered_map<string, unsigned> &instrs) {
    Instruction instr = q.front().instr;

    if (instr.type == Load || instr.type == Store) {
        // update status of execution units
        if (instr.type == Load) { LoadMEMBusy = false; }
        if (instr.type == Store) { StoreMEMBusy = false; }

        instrs[instr.PC] = true;
    }

    q.push(Event(WB, instr));
}

void EventList::processWB(deque<Instruction> &processor) {
    Instruction instr = q.front().instr;
    for (auto curr = processor.begin(); curr != processor.end(); curr++) {
        if ((*curr).PC == instr.PC && (*curr).type == instr.type) { 
            processor.erase(curr); 
            return;
        }
    }
}