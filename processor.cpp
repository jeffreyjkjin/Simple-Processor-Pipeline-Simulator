#include "processor.hpp"

Processor::Processor(IQueue &iQ, int width) {
    // initalize attributes
    for (unsigned i = 0; i < 5; i++) { stageCount[i] = 0; }

    for (unsigned i = 0; i < 5; i++) { iBusy[i] = false; }

    // should be the first instruction in iQ since it should be the first one to reach each stage
    for (unsigned i = 0; i < 5; i++) { nextInstr[i] = iQ.front().number; }

    // add first width-th number of instructions
    for (unsigned i = 0; i < width; i++) { 
        if (!iQ.isEmpty()) { 
            Instruction instr = iQ.front();
            q.push_back(instr); 
            iQ.pop();

            stageCount[IF]++;

            if (instr.type == Branch) {
                // stop fetching instructions if latest instruction is a branch
                iBusy[Branch - 1] = true;
                return;
            }
        }
    }
}

bool Processor::insertIF(Instruction instr, int width) {
    // do not insert if IF stage full or branch is in stage IF, ID or EX
    if (stageCount[IF] < width && !iBusy[Branch - 1]) {
        q.push_back(instr);
        stageCount[IF]++;
        
        if (instr.type == Branch) { iBusy[Branch - 1] = true; }
        
        return true;
    }

    return false;
}

void Processor::remove(Instruction instr) {
    for (auto it = q.begin(); it != q.end(); it++) {
        if ((*it).PC == instr.PC) { 
            q.erase(it); 
            return;
        }
    }
}

unsigned Processor::size() const { return q.size(); }

const deque<Instruction>::iterator Processor::begin() { return q.begin(); }

const deque<Instruction>::iterator Processor::end() { return q.end(); }