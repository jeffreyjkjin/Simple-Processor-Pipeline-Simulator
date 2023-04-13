#include <stdexcept>
#include <string>

#include "simulator.hpp"

using namespace std;
 
int main(int argc, char *argv[]) {
    if (argc < 5) {
        // if the user does not provide enough parameters
        throw invalid_argument("Insuffient number of parameters provided!");
    }

    // grab parameters
    string fileName = argv[1];
    int startLine = stoi(argv[2]);
    int instrCount = stoi(argv[3]);
    int width = stoi(argv[4]);

    if (startLine <= 0 || instrCount <= 0 || width <= 0) {
        // validate parameters
        throw invalid_argument("Invalid parameter provided!");
    }

    // run simulation
    Simulator s = Simulator(fileName, startLine, instrCount, width);
    s.start();

    return 0;
}