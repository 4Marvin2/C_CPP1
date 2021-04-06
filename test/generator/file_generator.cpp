#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

void generate_str(std::ostream& out, int size) {
    std::random_device rd;
    char symbol;
    int length;
    for (int i = 0; i != size; ++i) {
        if (rand()%5) {
            symbol = (char)('a' + rand()%26);
            out << symbol;
        } else {
            length = rand()%10 + 1;
            symbol = (char)('a' + rand()%26);
            for (int j = 0; j < length; j++) {
                out << symbol;
            }
        }
    }
}

int main(int argc, char **argv) {

    std::ofstream file(argv[1]);

    int size;

    std::stringstream(argv[2]) >> size;
    generate_str(file, size);
    return 0;
}