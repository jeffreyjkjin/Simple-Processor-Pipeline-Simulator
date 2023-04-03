#include "simulator.hpp"

#include <iostream>
#include <unordered_map>
#include <queue>

#include "eventlist.hpp"
#include "iqueue.hpp"

void Simulator::printStatistics() {
    cout << "Current Execution Time: " << clockCycle << endl;
    cout << "Histogram of Instructions" << endl;
    cout << "Integer: ";
    cout << (iHistogram[0] ? (float) iHistogram[0] / (float) totalInstructions : 0) << endl;
    cout << "Float Point: ";
    cout << (iHistogram[1] ? (float) iHistogram[1] / (float) totalInstructions : 0) << endl;
    cout << "Branch: ";
    cout << (iHistogram[2] ? (float) iHistogram[2] / (float) totalInstructions : 0) << endl;
    cout << "Load: ";
    cout << (iHistogram[3] ? (float) iHistogram[3] / (float) totalInstructions : 0) << endl;
    cout << "Store: ";
    cout << (iHistogram[4] ? (float) iHistogram[4] / (float) totalInstructions : 0) << endl;
}

Simulator::Simulator(string fileName, int startLine, int endLine, int width) {
    this->fileName = fileName;
    this->startLine = startLine;
    this->instrCount = endLine;
    this->width = width;

    totalInstructions = 0;
    clockCycle = 0;
}

void Simulator::start() {
    IQueue iQ = IQueue(fileName, startLine, instrCount);
    
    // Add the first width instructions into the processor queue
    deque<Instruction> processor = deque<Instruction>();
    for (int i = 0; i < width; i++) { processor.push_back(iQ.pop()); }

    EventList eList = EventList(processor);

    unordered_map<string, unsigned> instrs = unordered_map<string, unsigned>();

    while (!processor.empty()) {
        
        cout << "Current Cycle = "  << clockCycle << endl;
        int numInstrs = processor.size();
        for (int i = 0; i < numInstrs; i++) {
            Event curr = eList.front();
            switch (curr.stage) {
                case IF:
                    cout << "IF" << endl;
                    eList.processIF(iQ, processor, instrs);
                    break;
                case ID:
                    cout << "ID" << endl;
                    eList.processID(instrs);
                    break;
                case EX:
                    cout << "EX" << endl;
                    eList.processEX(instrs);
                    break;
                case MEM:
                    cout << "MEM" << endl;
                    eList.processMEM(instrs);
                    break;
                case WB:
                    cout << "WB" << endl;;
                    eList.processWB(processor);
                    totalInstructions++;
                    break;
            }
            eList.pop();
        }
        clockCycle++;
    }
}