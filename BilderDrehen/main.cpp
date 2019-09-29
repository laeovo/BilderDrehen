#include <iostream>

#include "Koordinatenfinder.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    for (int i = 9; i <= 14; ++i) {
        string dateiname = "Scan ";
        dateiname.append(to_string(i));
        dateiname.append(".jpeg");
        Koordinatenfinder finder{dateiname};
        finder.fertigStellen();
    }
    return 0;
}
