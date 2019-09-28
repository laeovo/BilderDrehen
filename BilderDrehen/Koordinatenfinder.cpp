#include <chrono>
#include <iostream>
#include <string>

#include "Binaersuche.hpp"

#include "Koordinatenfinder.hpp"

using namespace std::chrono;
using namespace cimg_library;
using namespace std;

Koordinatenfinder::Koordinatenfinder(const string& dateiname) : bild(dateiname.c_str()) {
    cout << "Koordinatenfinder::Konstruktor. Dateiname ist " << dateiname << endl;
    assert(bild.width() == 2480);
    assert(bild.height() == 3508);
    
    const auto tic{std::chrono::high_resolution_clock::now()};
    
    //this->eckeObenLinks = this->berechneEckeObenLinks();
    
    this->obereKante = this->berechneObereKante(500, 40);
//    this->linkeKante = this->berechneLinkeKante(0, 40);
    
    const auto toc{std::chrono::high_resolution_clock::now()};
    const duration<double, std::milli> ms{toc-tic};
    cout << "Zeit: " << ms.count() << endl;
    
//    cout << "Linke Kante: " << this->linkeKante << endl;
    cout << "Obere Kante: " << this->obereKante << endl;
    
    this->eckeObenRechts = this->berechneEckeObenRechts();
    this->eckeUntenLinks = this->berechneEckeUntenLinks();
    this->eckeUntenRechts = this->berechneEckeUntenRechts();
}

const std::pair<int, int> Koordinatenfinder::berechneEckeObenLinks() {
    this->obereKante = this->berechneObereKante(500, 40);
    
    
    
    
    
    
    
    
    const int radius = 13;
    const int max = bild.height()/2;
    int obereGrenze{100};
    const int x{this->bild.width() / 2};
    while (this->istKomplettWeiss(x, obereGrenze, radius)) {
        cout << "Probiere y = " << obereGrenze << " als obere Grenze" << endl;
        obereGrenze += 2 * radius + 1;
        if (obereGrenze > max) {
            cout << "Fehler: obere Grenze wurde nicht gefunden; berechneEckeObenLinks() scheiterte" << endl;
            return std::pair<int, int>(0,0);
        }
    }
    cout << "Ergebnis: die obere Grenze liegt ca. bei y = " << obereGrenze << endl;
    
    
    return std::pair<int, int>(0,0);
}

const std::pair<int, int> Koordinatenfinder::berechneEckeObenRechts() {
    
    return std::pair<int, int>(0,0);
}

const std::pair<int, int> Koordinatenfinder::berechneEckeUntenLinks() {
    
    return std::pair<int, int>(0,0);
}

const std::pair<int, int> Koordinatenfinder::berechneEckeUntenRechts() {
    const int x{this->eckeObenRechts.first + (this->eckeUntenLinks.first - this->eckeObenLinks.first)};
    const int y{this->eckeUntenLinks.second + (this->eckeObenRechts.second - this->eckeObenLinks.second)};
    return std::pair<int, int>(x, y);
}

const int Koordinatenfinder::berechneLinkeKante(const int startwert, const int radius) const {
    cout << "Berechne Linke Kante: Radius = " << radius << ", Startwert = " << startwert << " (" << startwert << "-" << startwert+2*radius+1 << ")" << endl;
    for (int x = startwert + radius + 1; x <= this->bild.width()/2; x += 2*radius+1) {
        for (int y = radius + 1; y <= this->bild.height()-2-radius; y += 2*radius+1) {
            if (!this->istKomplettWeiss(x, y, radius)) {
                if (radius == 0) {
                    cout << "Kante gefunden! x = " << x << ", y = " << y << endl;
                    return x;
                }
                else {
                    return this->berechneLinkeKante(x - (3*radius + 1), (radius-1)/3);
                }
            }
        }
    }
    cout << "Linke Kante wurde nicht gefunden" << endl;
    return 0;
}

const int Koordinatenfinder::berechneObereKante(const int startwert, const int radius) const {
    for (int y = startwert + radius + 1; y <= this->bild.height()-2-radius; y += 2*radius+1) {
        cout << "Berechne Obere Kante: Radius = " << radius << ", y = [" << y-radius-1 << ", " << y+radius+1 << "]" << endl;
        for (int x = radius + 1; x <= this->bild.width()-2-radius; x += 2*radius+1) {
            if (!this->istKomplettWeiss(x, y, radius)) {
                if (radius == 0) {
                    cout << "Kante gefunden! x = " << x << ", y = " << y << endl;
                    return y;
                }
                else {
                    return this->berechneObereKante(y - (3*radius + 1), (radius-1)/3);
                }
            }
        }
    }
    cout << "Obere Kante wurde nicht gefunden" << endl;
    return 0;
}

const bool Koordinatenfinder::istWeiss(const int x, const int y) const {
//    cout << "istWeiss(" << x << ", " << y << ")" << endl;
    if (x < 0 || x > this->bild.width() || y < 0 || y > this->bild.height()) {
//        cout << "  Pixel ist außerhalb des Bildes (-->return true)" << endl;
        return true;
    }
    const Farbe farbe{this->getPixel(x, y)};
//    cout << "  Farbe ist (" << farbe.r << ", " << farbe.g << ", " << farbe.b << ")" << endl;
    const int schwellwert{200}; // ab diesem Wert gilt ein Pixel als weiß
    return farbe.r > schwellwert && farbe.g > schwellwert && farbe.b > schwellwert;
}

const bool Koordinatenfinder::istKomplettWeiss(const int x, const int y, const int radius) const {
//    cout << "istKomplettWeiss (" << x << ", " << y << "), radius = " << radius << endl;
    if (radius == 0) {
        return istWeiss(x, y);
    }
    else {
        const int verfeinerung{3}; // gibt an, wie fein der Suchraum unterteilt wird.
                                   // je größer die Verfeinerung, desto näher sind die Punkte an den Rändern
        
        const int verschiebung{(radius * (verfeinerung-1) + 1) / verfeinerung};
        const int oben{y - verschiebung};
        const int unten{y + verschiebung};
        const int links{x - verschiebung};
        const int rechts{x + verschiebung};
        
        int weisseFelder{0};
        if (this->istKomplettWeiss(links, oben, radius/verfeinerung)) {
            ++weisseFelder;
        }
        if (this->istKomplettWeiss(rechts, oben, radius/verfeinerung)) {
            ++weisseFelder;
        }
        if (this->istKomplettWeiss(links, unten, radius/verfeinerung)) {
            ++weisseFelder;
        }
        if (this->istKomplettWeiss(rechts, unten, radius/verfeinerung)) {
            ++weisseFelder;
        }
        return weisseFelder >= 3;
    }
}

const Farbe Koordinatenfinder::getPixel(const int x, const int y) const {
    const int r{this->bild(x, y, 0, 0)};
    const int g{this->bild(x, y, 0, 1)};
    const int b{this->bild(x, y, 0, 2)};
    return Farbe(r, g, b);
}
