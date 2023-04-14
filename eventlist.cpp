#include "eventlist.hpp"

using std::make_pair;

EventList::EventList(DTracker &dT, IQueue &iQ, const int width) {
    // initalize attributes
    for (unsigned i = 0; i < 5; i++) { dT.stageCount[i] = 0; }

    for (unsigned i = 0; i < 5; i++) { dT.iBusy[i] = ""; }
    
    // schedule IFs for the first width-th number of instructions
    while (!iQ.isEmpty() && dT.stageCount[IF] < width) {
        Instruction instr = iQ.front();
        iQ.pop();

        q.push(Event(IF, instr));

        dT.stageCount[IF]++;

        if (instr.type == Branch) {
            // stop fetching instructions if latest instruction is a branch
            dT.iBusy[Branch - 1] = instr.PC;
            return;
        }
    }

    // should be the first instruction in iQ since it should be the first one to reach each stage
    for (unsigned i = 0; i < 5; i++) { dT.nextInstr[i] = q.front().instr.number; }
}

void EventList::pop() { q.pop(); }

Event EventList::front() const { return q.front(); }

int EventList::size() const { return q.size(); }

void EventList::fetch(DTracker &dT, IQueue &iQ, const int width) {
    // schedule IFs for at most the next width-th instructions
    while (!iQ.isEmpty() && dT.stageCount[IF] < width && dT.iBusy[Branch - 1] == "") {
        Instruction instr = iQ.front();
        iQ.pop();

        q.push(Event(IF, instr));

        dT.stageCount[IF]++;

        if (instr.type == Branch) {
            // stop fetching instructions if latest instruction is a branch
            dT.iBusy[Branch - 1] = instr.PC;
            return;
        }
    }
}

void EventList::processIF(DTracker &dT, int width) {
    Instruction instr = q.front().instr;
    
    if (dT.stageCount[ID] == width || dT.nextInstr[IF] != instr.number) {
        // reschedule current instruction if ID stage is full or not next program order instruction
        q.push(Event(IF, instr));
        return;
    }
    
    // schedule ID event for current instruction
    q.push(Event(ID, instr));

    dT.instrs[instr.PC].push_front(make_pair(instr.number, false));

    dT.stageCount[IF]--;
    dT.stageCount[ID]++;

    dT.nextInstr[IF]++;
}

void EventList::processID(DTracker &dT, const int width) {
    Instruction instr = q.front().instr;

    // check if data dependencies are satisfied
    for (auto curr: instr.dependents) {
        if (dT.instrs.find(curr) != dT.instrs.end()) {

            for (auto search: dT.instrs[curr]) {
                // find latest instruction with the same PC
                if (search.first < instr.number && !search.second) {
                    // reschedule event if dependency not satisfied
                    q.push(Event(ID, instr));
                    return;
                }
            }
        }
    }
    
    // assume data dependencies are always satisified even if this instruction is rescheduled
    instr.dependents.clear();

    if (dT.stageCount[EX] == width || dT.nextInstr[ID] != instr.number) {
        // reschedule current instruction if EX stage is full or not next program order instruction
        q.push(Event(ID, instr));
        return;
    }

    if (instr.type == Integer || instr.type == Float) {
        if (dT.iBusy[instr.type - 1] != "") {
            // reschedule event if structural dependency not avaliable
            q.push(Event(ID, instr));
            return;
        }

        // occupy corresponding structural dependency
        dT.iBusy[instr.type - 1] = instr.PC;
    }

    q.push(Event(EX, instr));

    dT.stageCount[ID]--;
    dT.stageCount[EX]++;

    dT.nextInstr[ID]++;
}

void EventList::processEX(DTracker &dT, const int width) {
    Instruction instr = q.front().instr;

    if (instr.type == Integer || instr.type == Float || instr.type == Branch) {
        // update structural/control hazards
        if (dT.iBusy[instr.type - 1] == instr.PC) {
            dT.iBusy[instr.type - 1] = "";
        }

        // update completion status of current instruction
        for (auto &search: dT.instrs[instr.PC]) {
            if (search.first == instr.number) { 
                search.second = true; 
                break;
            }
        }
    }

    if (dT.stageCount[MEM] == width || dT.nextInstr[EX] != instr.number) {
        // reschedule current instruction if MEM stage is full or not next program order instruction
        q.push(Event(EX, instr));
        return;
    }

    if (instr.type == Load || instr.type == Store) {
        if (dT.iBusy[instr.type - 1] != "") {
            // reschedule event if structural dependency not available
            q.push(Event(EX, instr));
            return;            
        }

        // occupy corresponding structural dependency
        dT.iBusy[instr.type-1] = instr.PC;
    }

    q.push(Event(MEM, instr));

    dT.stageCount[EX]--;    
    dT.stageCount[MEM]++;

    dT.nextInstr[EX]++;
}

void EventList::processMEM(DTracker &dT, const int width) {
    Instruction instr = q.front().instr;

    if (instr.type == Load || instr.type == Store) {
        // update structural hazards
        if (dT.iBusy[instr.type - 1] == instr.PC) {
            dT.iBusy[instr.type - 1] = "";
        }

        // update completion status of current instruction
        for (auto &search: dT.instrs[instr.PC]) {
            if (search.first == instr.number) { 
                search.second = true; 
                break;
            }
        }
    }

    if (dT.stageCount[WB] == width || dT.nextInstr[MEM] != instr.number) {
        // reschedule current instruction if WB stage is full or not next program order instruction
        q.push(Event(MEM, instr));
        return;
    }

    q.push(Event(WB, instr));

    dT.stageCount[MEM]--;
    dT.stageCount[WB]++;

    dT.nextInstr[MEM]++;
}

void EventList::processWB(DTracker &dT) {
    Instruction instr = q.front().instr;

    if (dT.nextInstr[WB] != instr.number) { 
        // reschedule current instruction if not next instruction in program order
        q.push(Event(WB, instr));
        return;        
    }

    dT.stageCount[WB]--;

    dT.nextInstr[WB]++;
}