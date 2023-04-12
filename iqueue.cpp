#include "iqueue.hpp"

#include <sstream>
#include <vector>

IQueue::IQueue(string fileName, int startLine, int instrCount) {
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
    while (lineCount < startLine + instrCount) {
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

        Instruction instr = Instruction(PC, type, tokens, lineCount);
        q.push(instr);

        lineCount++;
    }

    file.close();
}

void IQueue::pop() { q.pop(); }

Instruction IQueue::front() const { return q.front(); }

bool IQueue::isEmpty() const { return q.empty(); }