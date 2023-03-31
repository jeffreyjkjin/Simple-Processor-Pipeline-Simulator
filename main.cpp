#include <fstream>
#include <iostream>
#include <string>

#include "simulator.hpp"

using namespace std;
 
int main(int argc, char *argv[]) {
    if (argc < 5) {
        // if the user does not provide enough parameters
        cout << "Insuffient number of parameters provided..." << endl;
        return -1;
    }

    // grab parameters
    string fileName = argv[1];
    int startLine = stoi(argv[2]);
    int instrCount = stoi(argv[3]);
    int width = stoi(argv[4]);

    if (startLine <= 0 || instrCount <= 0 || width <= 0) {
        // validate parameters
        cout << "Invalid parameter provided..." << endl;
        return -1;
    }

    // open file
    ifstream file;
    file.open(fileName);

    if (!file.is_open()) {
        // if file does not exist
        cout << "File '" << fileName << "' does not exist..." << endl; 
        return -1;
    }
    file.close();

    // run simulation
    Simulator s = Simulator(fileName, startLine, instrCount);
    s.start();

    return 0;
}