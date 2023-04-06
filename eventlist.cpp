#include "eventlist.hpp"

EventList::EventList(Processor &p) {
    // initialize variables
    q = queue<Event>();

    // schedule IFs for the first width number of instructions
    for (auto instr: p) { 
        q.push(Event(IF, instr)); 
        p.sCount[IF]++;
    }
}

void EventList::pop() { q.pop(); }

Event EventList::front() const { return q.front(); }

void EventList::insertIF(Instruction &instr) { q.push(Event(IF, instr)); }

void EventList::processIF(unordered_map<string, unsigned> &instrs, IQueue &iQ, Processor &p) {
    // schedule ID event for current instruction
    Instruction curr = q.front().instr;
    q.push(Event(ID, curr));

    instrs[curr.PC] = 0;

    p.sCount[IF]--;
    p.sCount[ID]++;
}

void EventList::processID(unsigned clockCycle, unordered_map<string, unsigned> &instrs, Processor &p) {
    Instruction curr = q.front().instr;

    if (curr.type == Integer || curr.type == Float || curr.type == Branch) {
        // checks if corresponding execution unit is available or not
        if (curr.type == Integer && (p.IntegerEXBusy == "" || p.IntegerEXBusy == curr.PC)) { 
            p.IntegerEXBusy = curr.PC; 
        }
        else if (curr.type == Float && (p.FloatEXBusy == "" || p.FloatEXBusy == curr.PC)) { 
            p.FloatEXBusy = curr.PC; 
        }
        else if (curr.type == Branch && (p.BranchEXBusy == "" || p.BranchEXBusy == curr.PC)) {
             p.BranchEXBusy = curr.PC; 
        }
        else if (curr.type == Integer || curr.type == Float || curr.type == Branch) {
            // reschedule event if execution unit not available
            q.push(Event(ID, curr));
            return;
        }

        // check if dependencies are satisfied
        for (auto instr: curr.dependents) {
            if (instrs.find(instr) != instrs.end() && instrs[instr] >= clockCycle) {
                // reschedule event if dependency not satisified
                q.push(Event(ID, curr));
                return;
            }
        }
    }

    q.push(Event(EX, curr));

    p.sCount[ID]--;
    p.sCount[EX]++;
}

void EventList::processEX(unsigned clockCycle, unordered_map<string, unsigned> &instrs, Processor &p) {
    Instruction curr = q.front().instr;

    if (curr.type == Integer || curr.type == Float || curr.type == Branch) {
        // update status of execution units
        if (curr.type == Integer) { p.IntegerEXBusy = ""; }
        else if (curr.type == Float) { p.FloatEXBusy = ""; }
        else if (curr.type == Branch) { p.BranchEXBusy = ""; }
        
        instrs[curr.PC] = clockCycle;
    }
    else {
        // check if read/write ports are available or not
        if (curr.type == Load && (p.LoadMEMBusy == "" || p.LoadMEMBusy == curr.PC)) {
             p.LoadMEMBusy = curr.PC; 
        }
        else if (curr.type == Store && (p.StoreMEMBusy == "" || p.StoreMEMBusy == curr.PC)) { 
            p.StoreMEMBusy = curr.PC; 
        }
        else if (curr.type == Load || curr.type == Store) {
            // reschedule event if read/write ports not available
            q.push(Event(EX, curr));
            return;
        }

        // check if dependencies are satisfied
        for (auto instr: curr.dependents) {
            if (instrs.find(instr) != instrs.end() && instrs[instr] >= clockCycle) {
                // reschedule event if dependency not satisified
                q.push(Event(EX, curr));
                return;
            }
        }
    }

    q.push(Event(MEM, curr));

    p.sCount[EX]--;
    p.sCount[MEM]++;
}

void EventList::processMEM(unsigned clockCycle, unordered_map<string, unsigned> &instrs, Processor &p) {
    Instruction curr = q.front().instr;

    if (curr.type == Load || curr.type == Store) {
        // update status of execution units
        if (curr.type == Load) { p.LoadMEMBusy = ""; }
        else if (curr.type == Store) { p.StoreMEMBusy = ""; }

        instrs[curr.PC] = clockCycle;
    }

    q.push(Event(WB, curr));

    p.sCount[MEM]--;
    p.sCount[WB]++;
}

void EventList::processWB(Processor &p) {
    Instruction instr = q.front().instr;
    p.remove(instr);

    p.sCount[WB]--;
}