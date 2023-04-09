#include "eventlist.hpp"

EventList::EventList(Processor &p) {
    // schedule IFs for the first width-th number of instructions
    unsigned i = 0;
    for (auto instr: p) { 
        q.push(Event(IF, instr, i));

        i++;
    }
}

void EventList::pop() { q.pop(); }

Event EventList::front() const { return q.front(); }

void EventList::insertIF(Instruction &instr, unsigned pipeline) { q.push(Event(IF, instr, pipeline)); }

void EventList::processIF(unordered_map<string, unsigned> &instrs, Processor &p) {
    Event e = q.front();
    Instruction instr = e.instr;
    
    if (p.pipelines[e.pipeline][ID]) {
        q.push(Event(IF, instr, e.pipeline));
        return;
    }
    
    // schedule ID event for current instruction
    q.push(Event(ID, instr, e.pipeline));

    instrs[instr.PC] = 0;

    p.pipelines[e.pipeline][IF] = false;
    p.pipelines[e.pipeline][ID] = true;
}

void EventList::processID(unsigned clockCycle, unordered_map<string, unsigned> &instrs, Processor &p) {
    Event e = q.front();
    Instruction instr = e.instr;

    if (p.pipelines[e.pipeline][EX]) {
        // reschedule current instruction if EX stage is full
        q.push(Event(ID, instr, e.pipeline));
        return;
    }

    if (instr.type == Integer || instr.type == Float) {
        // checks if corresponding execution unit is available or not
        if (instr.type == Integer && (p.IntegerBusy == "" || p.IntegerBusy == instr.PC)) { 
            p.IntegerBusy = instr.PC; 
        }
        else if (instr.type == Float && (p.FloatBusy == "" || p.FloatBusy == instr.PC)) { 
            p.FloatBusy = instr.PC; 
        }
        else {
            // reschedule event if execution unit not available
            q.push(Event(ID, instr, e.pipeline));
            return;
        }

        // check if dependencies are satisfied
        for (auto curr: instr.dependents) {
            if (instrs.find(curr) != instrs.end() && instrs[curr] >= clockCycle) {
                // reschedule event if dependency not satisified
                q.push(Event(ID, instr, e.pipeline));
                return;
            }
        }
    }

    q.push(Event(EX, instr, e.pipeline));

    p.pipelines[e.pipeline][ID] = false;
    p.pipelines[e.pipeline][EX] = true;
}

void EventList::processEX(unsigned clockCycle, unordered_map<string, unsigned> &instrs, Processor &p) {
    Event e = q.front();
    Instruction instr = q.front().instr;

    if (p.pipelines[e.pipeline][MEM]) {
        // reschedule current instruction if MEM stage is full
        q.push(Event(EX, instr, e.pipeline));
        return;
    }

    if (instr.type == Integer || instr.type == Float || instr.type == Branch) {
        // update status of execution units
        if (instr.type == Integer) { p.IntegerBusy = ""; }
        else if (instr.type == Float) { p.FloatBusy = ""; }
        else if (instr.type == Branch) { p.BranchBusy = ""; }
        
        instrs[instr.PC] = clockCycle;
    }
    else {
        // check if read/write ports are available or not
        if (instr.type == Load && (p.LoadBusy == "" || p.LoadBusy == instr.PC)) {
             p.LoadBusy = instr.PC; 
        }
        else if (instr.type == Store && (p.StoreBusy == "" || p.StoreBusy == instr.PC)) { 
            p.StoreBusy = instr.PC; 
        }
        else {
            // reschedule event if read/write ports not available
            q.push(Event(EX, instr, e.pipeline));
            return;
        }

        // check if dependencies are satisfied
        for (auto curr: instr.dependents) {
            if (instrs.find(curr) != instrs.end() && instrs[curr] >= clockCycle) {
                // reschedule event if dependency not satisified
                q.push(Event(EX, instr, e.pipeline));
                return;
            }
        }
    }

    q.push(Event(MEM, instr, e.pipeline));

    p.pipelines[e.pipeline][EX] = false;    
    p.pipelines[e.pipeline][MEM] = true;
}

void EventList::processMEM(unsigned clockCycle, unordered_map<string, unsigned> &instrs, Processor &p) {
    Event e = q.front();
    Instruction instr = q.front().instr;

    if (p.pipelines[e.pipeline][WB]) {
        // reschedule current instruction if MEM stage is full
        q.push(Event(EX, instr, e.pipeline));
        return;
    }

    if (instr.type == Load || instr.type == Store) {
        // update status of execution units
        if (instr.type == Load) { p.LoadBusy = ""; }
        else if (instr.type == Store) { p.StoreBusy = ""; }

        instrs[instr.PC] = clockCycle;
    }

    q.push(Event(WB, instr, e.pipeline));

    p.pipelines[e.pipeline][MEM] = false;
    p.pipelines[e.pipeline][WB] = true;
}

void EventList::processWB(Processor &p) {
    Event e = q.front();
    Instruction instr = q.front().instr;
    p.remove(instr);

    p.pipelines[e.pipeline][WB] = false;
}