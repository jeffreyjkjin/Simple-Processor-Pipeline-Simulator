#include "processor.hpp"

#include <iostream>

Processor::Processor(IQueue &iQ, int width) {
    // initalize attributes
    q = deque<Instruction>();
    
    for (unsigned i = 0; i < 5; i++) { sCount[i] = 0; }

    IntegerEXBusy = "";
    FloatEXBusy = "";  
    BranchEXBusy = ""; 
    LoadMEMBusy = "";
    StoreMEMBusy = "";

    // add first width number of instructions
    for (unsigned i = 0; i < width; i++) { 
        if (!iQ.isEmpty()) { 
            q.push_back(iQ.front()); 
            iQ.pop();
        }
    }

}

bool Processor::insertIF(Instruction instr, int width) {
    if (sCount[0] == width) { return false; }
    q.push_back(instr);
    sCount[0]++;

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