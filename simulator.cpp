#include "simulator.hpp"

#include <iostream>

#include "iqueue.hpp"

void Simulator::printStatistics() {
    
}

Simulator::Simulator(string fileName, int startLine, int endLine) {
    this->fileName = fileName;
    this->startLine = startLine;
    this->endLine = endLine;

    totalInstructions = 0;
    clockCycle = 0;
}

void Simulator::start() {
    IQueue q = IQueue(fileName, startLine, endLine);
}