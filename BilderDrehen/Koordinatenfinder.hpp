#ifndef Koordinatenfinder_hpp
#define Koordinatenfinder_hpp

#include <stdio.h>

#define cimg_display 0
#define cimg_use_jpeg

#include "CImg.h"

#include "Farbe.hpp"

class Koordinatenfinder {
public:
    Koordinatenfinder(const std::string& dateiname);
private:
    const cimg_library::CImg<unsigned char> bild;
    
    const std::pair<int, int> berechneEckeObenLinks();
    const std::pair<int, int> berechneEckeObenRechts();
    const std::pair<int, int> berechneEckeUntenLinks();
    const std::pair<int, int> berechneEckeUntenRechts();
    
    const bool istWeiss(const int x, const int y) const;
    const bool istKomplettWeiss(const int x, const int y, const int radius) const;
    
    const Farbe getPixel(const int x, const int y) const;
    
    std::pair<int, int> eckeObenLinks;
    std::pair<int, int> eckeObenRechts;
    std::pair<int, int> eckeUntenLinks;
    std::pair<int, int> eckeUntenRechts;
};

#endif /* Koordinatenfinder_hpp */
