#ifndef Koordinatenfinder_hpp
#define Koordinatenfinder_hpp

#include <stdio.h>
#include <vector>

#define cimg_display 0
#define cimg_use_jpeg

#include "CImg.h"

#include "Farbe.hpp"

class Koordinatenfinder {
public:
    Koordinatenfinder(const std::string& dateiname);
    
    const double getWinkel() const;
    const std::pair<int, int> getEckeObenLinks() const;
    
    void fertigStellen();
    void bildDrehen();
    void bildZuschneiden();
    void bildSichern();
private:
    cimg_library::CImg<unsigned char> bild;
    
    const int berechneLinkeKante(const int startwertX, const int radius);
    const int berechneRechteKante(const int startwertX, const int radius);
    const int berechneObereKante(const int startwertY, const int radius);
    const int berechneUntereKante(const int startwertY, const int radius);
    
    void berechneEcken();
    void berechneAbmessungen();
    
    const bool istWeiss(const int x, const int y) const;
    const bool istKomplettWeiss(const int x, const int y, const int radius) const;
    
    const Farbe getPixel(const int x, const int y) const;
    
    int linkeKante = -1;
    int rechteKante = -1;
    int obereKante = -1;
    int untereKante = -1;
    
    int width = 0;
    int height = 0;
    
    std::pair<int, int> eckeObenLinks;
    std::pair<int, int> eckeObenRechts;
    std::pair<int, int> eckeUntenLinks;
    std::pair<int, int> eckeUntenRechts;
    
    std::vector<std::pair<int, int>> referenzEcken;
    
    const std::string dateiname;
};

#endif /* Koordinatenfinder_hpp */
