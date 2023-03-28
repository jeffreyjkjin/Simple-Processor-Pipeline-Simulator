#include "iqueue.hpp"

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

IQueue::IQueue(string fileName, int startLine, int endLine) {
    ifstream file;
    file.open(fileName);
    
    q = new queue<Instruction>();

    int lineCount = 1;
    string line;

    // get to the startLine
    while (lineCount < startLine) { 
        getline(file, line);
        lineCount++;
    }

    // read lines from the file
    while (lineCount < startLine + endLine) {
        getline(file, line);

        // get tokens from line
        vector<string> tokens = vector<string>();
        stringstream stream = stringstream(line);
        while (stream.good()) {
            string token;
            getline(stream, token, ',');
            tokens.push_back(token);
        }

        // TODO: create new instruction with tokens from line and store into queue

        lineCount++;
    }

    file.close();
}