#include <iostream>

#include "Koordinatenfinder.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    Koordinatenfinder finder{"Test.jpeg"};
    finder.fertigStellen();
    return 0;
}
