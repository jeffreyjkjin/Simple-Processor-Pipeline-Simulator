#include "simulator.hpp"

#include <iostream>
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

    // while (!iQ.isEmpty()) {
    //     for (auto it; processor) {
    //         Event curr = eList.pop();

    //         switch (curr.stage) {
    //             case IF:
    //                 eList.processIF();
    //                 break;
    //             case ID:
    //                 eList.processID();
    //                 break;
    //             case EX:
    //                 eList.processEX();
    //                 break;
    //             case MEM:
    //                 eList.processMEM();
    //                 break;
    //             case WB:
    //                 eList.processWB();
    //                 break;
    //         }
    //     }
    //     clockCycle++;
    // }
}