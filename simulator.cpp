#include "simulator.hpp"

#include <iomanip>
#include <iostream>

#include "dtracker.hpp"
#include "eventlist.hpp"
#include "iqueue.hpp"

using std::chrono::high_resolution_clock;
using std::cout;
using std::endl;
using std::fixed;
using std::left;
using std::right;
using std::setprecision;
using std::setw;

void Simulator::print() const {
    // compute percentages for histogram
    double histogram[5];
    for (unsigned i = 0; i < 5; i++) {
        // check if instruction count is zero
        if (iCount[i]) { histogram[i] = ((double) iCount[i] * 100) / (double) totalInstructions; }
        else { histogram[i] = 0; }
    }

    cout << "-----Simple Processor Pipeline Simulator-----" << endl;
    cout << left << setw(21) << "Trace File";
    cout << right << setw(24) << fileName << endl;
    cout << left << setw(21) << "Starting Instruction";
    cout << right << setw(24) << startLine << endl;
    cout << left << setw(21) << "Instruction Count";
    cout << right << setw(24) << instrCount << endl;
    cout << left << setw(21) << "Pipeline Width";
    cout << right << setw(24) << width << endl;

    cout << "------------Simulation Statistics------------" << endl;
    cout << left << setw(30) << "Simulation Runtime (Seconds)";
    cout << right << setw(15) << runTime.count() / 1000 << endl;
    cout << left << setw(30) << "Total Execution Time (Cycles)";
    cout << right << setw(15) << clockCycle << endl;
    cout << left << setw(30) << "Total Instructions Executed";
    cout << right << setw(15) << totalInstructions << endl;

    cout << "------------Instruction Histogram------------" << endl;
    cout << left << setw(25) << "Instruction Type";
    cout << right << setw(20) << "Percentage (%)" << endl;
    cout << left << setw(31) << "(1) Integer";
    cout << left << setw(14) << histogram[0] << endl;
    cout << left << setw(31) << "(2) Float";
    cout << left << setw(14) << histogram[1] << endl;
    cout << left << setw(31) << "(3) Branch";
    cout << left << setw(14) << histogram[2] << endl;
    cout << left << setw(31) << "(4) Load";
    cout << left << setw(14) << histogram[3] << endl;
    cout << left << setw(31) << "(5) Store";
    cout << left << setw(14) << histogram[4] << endl;
}

Simulator::Simulator(const string fileName, const int startLine, const int instrCount, const int width):
    fileName(fileName), startLine(startLine), instrCount(instrCount), width(width) {
    // initalize attributes
    totalInstructions = 0;
    clockCycle = 0;
    for (unsigned i = 0; i < 5; i++) { iCount[i] = 0; }

    // set decimal placements for printing statistics
    cout << fixed << setprecision(4);
}

void Simulator::start() {
    auto start = high_resolution_clock::now();

    IQueue iQ = IQueue(fileName, startLine, instrCount);
    DTracker dT;
    EventList eList = EventList(dT, iQ, width);

    // event loop keeps running while there still instructions in processor or instruction queue
    while (eList.size() || !iQ.isEmpty()) {

        unsigned numInstrs = eList.size();
        for (unsigned i = 0; i < numInstrs; i++) {
            // processes every instruction in the processor
            Event curr = eList.front();

            switch (curr.stage) {
                case IF:
                    eList.processIF(dT, width);
                    break;
                case ID:
                    eList.processID(dT, width);
                    break;
                case EX:
                    eList.processEX(dT, width);
                    break;
                case MEM:
                    eList.processMEM(dT, width);
                    break;
                case WB:
                    eList.processWB(dT);

                    iCount[curr.instr.type - 1]++;
                    totalInstructions++;
                    break;
            }
            eList.pop();
        }

        eList.fetch(dT, iQ, width);

        clockCycle++;
    }

    auto end = high_resolution_clock::now();
    runTime = end - start;

    print();
}