#include "eventlist.hpp"

EventList::EventList(Processor &p) {
    // initialize variables
    q = queue<Event>();

    // schedule IFs for the first width number of instructions
    for (auto instr: p) { 
        q.push(Event(IF, instr)); 
        p.sCount[IF]++;

        if (instr.type == Branch) {
            // if branch is fetched, stop fetching instructions
            p.BranchBusy = instr.PC;
            break;
        }
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

    if (curr.type == Integer || curr.type == Float) {
        // checks if corresponding execution unit is available or not
        if (curr.type == Integer && (p.IntegerBusy == "" || p.IntegerBusy == curr.PC)) { 
            p.IntegerBusy = curr.PC; 
        }
        else if (curr.type == Float && (p.FloatBusy == "" || p.FloatBusy == curr.PC)) { 
            p.FloatBusy = curr.PC; 
        }
        else if (curr.type == Integer || curr.type == Float) {
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
        if (curr.type == Integer) { p.IntegerBusy = ""; }
        else if (curr.type == Float) { p.FloatBusy = ""; }
        else if (curr.type == Branch) { p.BranchBusy = ""; }
        
        instrs[curr.PC] = clockCycle;
    }
    else {
        // check if read/write ports are available or not
        if (curr.type == Load && (p.LoadBusy == "" || p.LoadBusy == curr.PC)) {
             p.LoadBusy = curr.PC; 
        }
        else if (curr.type == Store && (p.StoreBusy == "" || p.StoreBusy == curr.PC)) { 
            p.StoreBusy = curr.PC; 
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
        if (curr.type == Load) { p.LoadBusy = ""; }
        else if (curr.type == Store) { p.StoreBusy = ""; }

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