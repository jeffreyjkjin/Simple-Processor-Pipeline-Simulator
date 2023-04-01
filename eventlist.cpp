#include "eventlist.hpp"

#include <iostream>
using namespace std;

EventList::EventList(deque<Instruction> processor) {
    // initialize variables
    q = queue<Event>();

    IntegerEXBusy = false;
    FloatEXBusy = false;
    BranchEXBusy = false;
    LoadMEMBusy = false;
    StoreMEMBusy = false;

    // schedule IF for the first width number of instructions
    // for (Instruction instr : processor) { q.push(Event(IF, instr)); }
    for (deque<Instruction>::iterator instr = processor.begin(); instr != processor.end(); instr++) { q.push(Event(IF, *instr)); }
 }

Event EventList::pop() { 
    Event temp = q.front();
    q.pop();
    
    return temp;
}