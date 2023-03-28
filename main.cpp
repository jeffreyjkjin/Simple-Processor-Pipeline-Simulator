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
    int endLine = stoi(argv[3]);
    int width = stoi(argv[4]);

    if (startLine <= 0 || endLine <= 0 || width <= 0 || endLine > startLine) {
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

    // TODO: check if file has valid lines between lineStart and lineEnd

    // run simulation
    Simulator s = Simulator(fileName, startLine, endLine);
    s.start();


    return 0;
}