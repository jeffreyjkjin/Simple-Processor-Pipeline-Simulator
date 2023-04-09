#include "processor.hpp"

Processor::Processor(IQueue &iQ, int width) {
    // initalize attributes
    for (unsigned i = 0; i < width; i++) {
        // set up 2D array for keeping track of pipelines and stages
        vector<bool> temp;
        for (unsigned j = 0; j < 5; j++) {
            temp.push_back(false);
        }
        pipelines.push_back(temp);
    }

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

            pipelines[i][IF] = true;

            if (instr.type == Branch) {
                // stop fetching instructions if latest instruction is a branch
                BranchBusy = instr.PC;
                return;
            }
        }
    }
}

int Processor::insertIF(Instruction instr, int width) {
    for (unsigned i = 0; i < width; i++) {
        // do not insert if IF stage full or branch is in stage IF, ID or EX
        if (!pipelines[i][IF] && BranchBusy == "") {
            q.push_back(instr);
            pipelines[i][IF] = true;
            
            if (instr.type == Branch) { BranchBusy = instr.PC; }
            
            return i;
        }
    }

    return -1;
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