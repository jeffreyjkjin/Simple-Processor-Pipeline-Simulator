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

    cout << "Simulation Run Time (Seconds): " << runTime.count() / 1000 << endl;
    cout << "Execution Time (Cycles): " << clockCycle << endl;
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
    auto start = high_resolution_clock::now();

    IQueue iQ = IQueue(fileName, startLine, instrCount);
    
    Processor p = Processor(iQ, width);

    EventList eList = EventList(p);

    unordered_map<string, vector<tuple<unsigned, bool>>> instrs;

    // event loop keeps running while there still instructions in processor or instruction queue
    while (p.size() || !iQ.isEmpty()) {
    
        unsigned numInstrs = p.size();

        for (unsigned i = 0; i < numInstrs; i++) {
            // processes every instruction in the processor
            Event curr = eList.front();

            switch (curr.stage) {
                case IF:
                    eList.processIF(instrs, p, width);
                    break;
                case ID:
                    eList.processID(instrs, p, width);
                    break;
                case EX:
                    eList.processEX(instrs, p, width);
                    break;
                case MEM:
                    eList.processMEM(instrs, p, width);
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
                Instruction instr = iQ.front();

                if (p.insertIF(instr, width)) { 
                    // only send instruction if there is room in the IF stage
                    eList.insertIF(instr);
                    iQ.pop(); 
                }
            }
        }

        clockCycle++;
    }

    auto end = high_resolution_clock::now();
    runTime = end - start;

    printStatistics();
}