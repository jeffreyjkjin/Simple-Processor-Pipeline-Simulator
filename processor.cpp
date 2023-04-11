#include "processor.hpp"

Processor::Processor(IQueue &iQ, int width) {
    // initalize attributes
    for (unsigned i = 0; i < 5; i++) { stageCount[i] = 0; }

    IntegerBusy = "";
    FloatBusy = "";  
    LoadBusy = "";
    StoreBusy = "";

    BranchBusy = ""; 

    // add first width-th number of instructions
    for (unsigned i = 0; i < width; i++) { 
        if (!iQ.isEmpty()) { 
            Instruction instr = iQ.front();
            q.push_back(instr); 
            iQ.pop();

            stageCount[IF]++;

            if (instr.type == Branch) {
                // stop fetching instructions if latest instruction is a branch
                BranchBusy = instr.PC;
                return;
            }
        }
    }
}

bool Processor::insertIF(Instruction instr, int width) {
    for (unsigned i = 0; i < width; i++) {
        // do not insert if IF stage full or branch is in stage IF, ID or EX
        if (stageCount[IF] < width && BranchBusy == "") {
            q.push_back(instr);
            stageCount[IF]++;
            
            if (instr.type == Branch) { BranchBusy = instr.PC; }
            
            return true;
        }
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