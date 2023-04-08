#include "processor.hpp"

#include <iostream>

Processor::Processor(IQueue &iQ, int width) {
    // initalize attributes
    q = deque<Instruction>();
    
    for (unsigned i = 0; i < 5; i++) { sCount[i] = 0; }

    IntegerBusy = "";
    FloatBusy = "";  
    LoadBusy = "";
    StoreBusy = "";

    BranchBusy = ""; 

    // add first width number of instructions
    for (unsigned i = 0; i < width; i++) { 
        if (!iQ.isEmpty()) { 
            Instruction curr = iQ.front();
            q.push_back(curr); 
            iQ.pop();

            if (curr.type == Branch) {
                // stop fetching instructions if latest instruction is a branch
                BranchBusy = curr.PC;
                return;
            }
        }
    }

}

bool Processor::insertIF(Instruction instr, int width) {
    // do not insert if IF stage full or branch is in stage IF, ID or EX
    if (sCount[0] == width || BranchBusy != "") { return false; }
    q.push_back(instr);
    sCount[0]++;

    if (instr.type == Branch) { BranchBusy = instr.PC; }

    return true;
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