#include "iqueue.hpp"

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

IQueue::IQueue(string fileName, int startLine, int endLine) {
    q = queue<Instruction>();

    ifstream file;
    file.open(fileName);

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

        // parse tokens
        string PC = tokens[0];
        iType type = (iType) stoi(tokens[1]);
        // erase PC and type from tokens so only dependents if any are left in the vector
        tokens.erase(tokens.begin(), tokens.begin() + 2);

        Instruction instr = Instruction(PC, type, tokens);
        q.push(instr);

        lineCount++;
    }

    file.close();
}

Instruction IQueue::pop() {
    Instruction instr = q.front();
    q.pop();

    return instr;
}