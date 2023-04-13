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

void EventList::processIF(unordered_map<string, vector<tuple<unsigned, bool>>> &instrs, Processor &p, int width) {
    Instruction instr = q.front().instr;
    
    if (p.stageCount[ID] == width || p.nextInstr[IF] != instr.number) {
        // reschedule current instruction if ID stage is full or not next program order instruction
        q.push(Event(IF, instr));
        return;
    }
    
    // schedule ID event for current instruction
    q.push(Event(ID, instr));

    instrs[instr.PC].push_back(make_tuple(instr.number, false));

    p.stageCount[IF]--;
    p.stageCount[ID]++;

    p.nextInstr[IF]++;
}

void EventList::processID(unordered_map<string, vector<tuple<unsigned, bool>>> &instrs, Processor &p, int width) {
    Instruction instr = q.front().instr;

    // check if dependencies are satisfied
    for (auto curr: instr.dependents) {
        if (instrs.find(curr) != instrs.end()) {
            for (auto search: instrs[curr]) {
                if (get<0>(search) < instr.number && !get<1>(search)) {
                    // reschedule event if dependency not satisified
                    q.push(Event(ID, instr));
                    return;
                }
            }
        }
    }

    if (p.stageCount[EX] == width || p.nextInstr[ID] != instr.number) {
        // reschedule current instruction if EX stage is full or not next program order instruction
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

    p.nextInstr[ID]++;
}

void EventList::processEX(unordered_map<string, vector<tuple<unsigned, bool>>> &instrs, Processor &p, int width) {
    Instruction instr = q.front().instr;

    if (p.stageCount[MEM] == width || p.nextInstr[EX] != instr.number) {
        // reschedule current instruction if MEM stage is full or not next program order instruction
        q.push(Event(EX, instr));
        return;
    }

    if (instr.type == Integer || instr.type == Float || instr.type == Branch) {
        // update status of execution units
        p.iBusy[instr.type - 1] = false;
        
        for (auto &search: instrs[instr.PC]) {
            if (get<0>(search) == instr.number) { 
                get<1>(search) = true; 
                break;
            }
        }
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

    p.nextInstr[EX]++;
}

void EventList::processMEM(unordered_map<string, vector<tuple<unsigned, bool>>> &instrs, Processor &p, int width) {
    Instruction instr = q.front().instr;

    if (p.stageCount[WB] == width || p.nextInstr[MEM] != instr.number) {
        // reschedule current instruction if WB stage is full or not next program order instruction
        q.push(Event(EX, instr));
        return;
    }

    if (instr.type == Load || instr.type == Store) {
        // update status of read/write ports
        p.iBusy[instr.type - 1] = false;

        for (auto &search: instrs[instr.PC]) {
            if (get<0>(search) == instr.number) { 
                get<1>(search) = true; 
                break;
            }
        }
    }

    q.push(Event(WB, instr));

    p.stageCount[MEM]--;
    p.stageCount[WB]++;

    p.nextInstr[MEM]++;
}

void EventList::processWB(Processor &p) {
    Instruction instr = q.front().instr;
    p.remove(instr);

    p.stageCount[WB]--;

    p.nextInstr[WB]++;
}