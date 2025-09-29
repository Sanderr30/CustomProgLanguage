#include <iostream>
#include <fstream>
#include <runtime/interpreter/interpreter.h>

int main(int argc, char** argv) {
    std::ifstream file(argv[1]);

    if (!file) {
        std::cerr << "unable to open the file\n";
    }

    if (Interpreter::Interpret(file, std::cout)) {
        std::cout << std::endl;
        return 0;
    } else {
        std::cerr << "Error: Script execution failed" << std::endl;
        return 1;
    }
}
