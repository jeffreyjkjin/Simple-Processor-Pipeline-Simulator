#include "simulator.hpp"

#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <queue>

#include "eventlist.hpp"
#include "iqueue.hpp"
#include "processor.hpp"

void Simulator::printStatistics() const {
    // compute percentages for histogram
    double histogram[5];
    for (unsigned i = 0; i < 5; i++) {
        // check if instruction count is zero
        if (iCount[i]) { histogram[i] = ((double) iCount[i] * 100) / (double) totalInstructions; }
        else { histogram[i] = 0; }
    }

    cout << "Execution Time (Cycles): " << clockCycle << endl;
    cout << "Histogram of Instructions" << endl;
    cout << "Integer: " << histogram[0] << "%" << endl;
    cout << "Float: " << histogram[1] << "%" << endl;
    cout << "Branch: " << histogram[2] << "%" << endl;
    cout << "Load: " << histogram[3] << "%" << endl;
    cout << "Store: " << histogram[4] << "%" << endl;
}

Simulator::Simulator(string fileName, int startLine, int instrCount, int width) {
    // initalize attributes
    this->fileName = fileName;
    this->startLine = startLine;
    this->instrCount = instrCount;
    this->width = width;

    totalInstructions = 0;
    clockCycle = 0;
    for (unsigned i = 0; i < 5; i++) { iCount[i] = 0; }

    // set decimal placements for printing statistics
    cout << fixed << setprecision(4);
}

void Simulator::start() {
    IQueue iQ = IQueue(fileName, startLine, instrCount);
    
    Processor p = Processor(iQ, width);

    EventList eList = EventList(p);

    unordered_map<string, unsigned> instrs = unordered_map<string, unsigned>();

    // event loop keeps running while there still instructions in processor or instruction queue
    while (p.size() || !iQ.isEmpty()) {
    
        int numInstrs = p.size();
        for (int i = 0; i < numInstrs; i++) {
            // processes every instruction in the processor
            Event curr = eList.front();

            switch (curr.stage) {
                case IF:
                    eList.processIF(instrs, iQ, p);
                    break;
                case ID:
                    eList.processID(clockCycle, instrs, p);
                    break;
                case EX:
                    eList.processEX(clockCycle, instrs, p);
                    break;
                case MEM:
                    eList.processMEM(clockCycle, instrs, p);
                    break;
                case WB:
                    eList.processWB(p);

                    iCount[curr.instr.type - 1]++;
                    totalInstructions++;
                    break;
            }
            eList.pop();
        }

        // send next width-th number of instructions into processor
        for (unsigned i = 0; i < width; i++) {
            if (!iQ.isEmpty()) {
                Instruction curr = iQ.front();
                if (p.insertIF(curr, width)) { 
                    // only send instruction if there is room in the IF stage
                    eList.insertIF(curr);
                    iQ.pop(); 
                }
            }
        }

        clockCycle++;
    }

    printStatistics();
}