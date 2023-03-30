#include "eventlist.hpp"

EventList::EventList() {
    // initialize variables
    q = queue<Event>();

    IntegerEXBusy = false;
    FloatEXBusy = false;
    BranchEXBusy = false;
    LoadMEMBusy = false;
    StoreMEMBusy = false;
}

Event EventList::pop() { 
    Event temp = q.front();
    q.pop();
    
    return temp;
}