#include "eventlist.hpp"

EventList::EventList(Processor &p) {
    // schedule IFs for the first width-th number of instructions
    unsigned i = 0;
    for (auto instr: p) { 
        q.push(Event(IF, instr));

        i++;
    }
}

void EventList::pop() { q.pop(); }

Event EventList::front() const { return q.front(); }

void EventList::insertIF(Instruction &instr) { q.push(Event(IF, instr)); }

void EventList::processIF(unordered_map<string, unsigned> &instrs, Processor &p, int width) {
    Instruction instr = q.front().instr;
    
    if (p.stageCount[ID] == width) {
        q.push(Event(IF, instr));
        return;
    }
    
    // schedule ID event for current instruction
    q.push(Event(ID, instr));

    if (instrs.find(instr.PC) == instrs.end()) { instrs[instr.PC] = 0; }

    p.stageCount[IF]--;
    p.stageCount[ID]++;
}

void EventList::processID(unsigned clockCycle, unordered_map<string, unsigned> &instrs, Processor &p, int width) {
    Instruction instr = q.front().instr;

    // check if dependencies are satisfied
    for (auto curr: instr.dependents) {
        if (instrs.find(curr) != instrs.end() && instrs[curr] == 0) {
            // reschedule event if dependency not satisified
            q.push(Event(ID, instr));
            return;
        }
    }

    if (p.stageCount[EX] == width) {
        // reschedule current instruction if EX stage is full
        q.push(Event(ID, instr));
        return;
    }

    if (instr.type == Integer || instr.type == Float) {
        // checks if corresponding execution unit is available or not
        if (p.iBusy[instr.type - 1]) {
            // reschedule event if execution unit not available
            q.push(Event(ID, instr));
            return;
        }

        // occupy corresponding execution unit
        p.iBusy[instr.type - 1] = true;
    }

    q.push(Event(EX, instr));

    p.stageCount[ID]--;
    p.stageCount[EX]++;
}

void EventList::processEX(unsigned clockCycle, unordered_map<string, unsigned> &instrs, Processor &p, int width) {
    Instruction instr = q.front().instr;

    if (p.stageCount[MEM] == width) {
        // reschedule current instruction if MEM stage is full
        q.push(Event(EX, instr));
        return;
    }

    if (instr.type == Integer || instr.type == Float || instr.type == Branch) {
        // update status of execution units
        p.iBusy[instr.type - 1] = false;
        
        instrs[instr.PC] = clockCycle;
    }
    else {
        // check if read/write ports are available or not
        if (p.iBusy[instr.type - 1]) {
            // reschedule event if read/write ports not available
            q.push(Event(EX, instr));
            return;            
        }

        // occupy corresponding read/write ports
        p.iBusy[instr.type-1] = true;
    }

    q.push(Event(MEM, instr));

    p.stageCount[EX]--;    
    p.stageCount[MEM]++;
}

void EventList::processMEM(unsigned clockCycle, unordered_map<string, unsigned> &instrs, Processor &p, int width) {
    Instruction instr = q.front().instr;

    if (p.stageCount[WB] == width) {
        // reschedule current instruction if MEM stage is full
        q.push(Event(EX, instr));
        return;
    }

    if (instr.type == Load || instr.type == Store) {
        // update status of read/write ports
        p.iBusy[instr.type - 1] = false;

        instrs[instr.PC] = clockCycle;
    }

    q.push(Event(WB, instr));

    p.stageCount[MEM]--;
    p.stageCount[WB]++;
}

void EventList::processWB(Processor &p) {
    Instruction instr = q.front().instr;
    p.remove(instr);

    p.stageCount[WB]--;
}