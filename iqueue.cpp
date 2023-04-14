#include "iqueue.hpp"

#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using std::ifstream;
using std::invalid_argument;
using std::out_of_range;
using std::stringstream;
using std::getline;
using std::to_string;

IQueue::IQueue(const string fileName, const int startLine, const int instrCount) {
    ifstream file;
    file.open(fileName);

    if (!file.is_open()) {
        // if file does not exist
        throw invalid_argument("File '" + fileName + "' does not exist!");
    }

    int lineCount = 0;
    string line;

    // get to the startLine
    while (lineCount < startLine && getline(file, line)) { lineCount++; }

    if (lineCount != startLine) { 
        // if starting line is greater than the number of lines the file
        throw out_of_range("Line " + to_string(startLine) + " does not exist in '" + fileName + "'!");
    }

    // read lines from the file
    while (lineCount < startLine + instrCount) {
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

        if (!getline(file, line)) { break; }
        lineCount++;
    }

    file.close();
}

void IQueue::pop() { q.pop(); }

Instruction IQueue::front() const { return q.front(); }

bool IQueue::isEmpty() const { return q.empty(); }