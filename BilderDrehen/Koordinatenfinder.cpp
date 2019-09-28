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
    
    this->eckeObenLinks = this->berechneEckeObenLinks();
    
    const auto toc{std::chrono::high_resolution_clock::now()};
    const duration<double, std::milli> ms{toc-tic};
    cout << "Zeit: " << ms.count() << endl;
    cout << "Obere Kante: " << this->obereKante << endl;
    cout << "Linke Kante: " << this->linkeKante << endl;
    cout << "Untere Kante: " << this->untereKante << endl;
    cout << "Rechte Kante: " << this->rechteKante << endl;
    cout << "Referenzecke: (" << this->referenzEcke.first << ", " << this->referenzEcke.second << ")" << endl;
    
    this->eckeObenRechts = this->berechneEckeObenRechts();
    this->eckeUntenLinks = this->berechneEckeUntenLinks();
    this->eckeUntenRechts = this->berechneEckeUntenRechts();
}

const std::pair<int, int> Koordinatenfinder::berechneEckeObenLinks() {
    this->obereKante = this->berechneObereKante(1000, 40);
    this->linkeKante = this->berechneLinkeKante(100, 40);
    this->untereKante = this->berechneUntereKante(this->bild.height()-1, 40);
    this->rechteKante = this->berechneRechteKante(this->bild.width()-1, 40);
    
    
    
    
    
    // TODO Referenzecke benutzen
    
    
    
    
    
    
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

const int Koordinatenfinder::berechneLinkeKante(const int startwertX, const int radius) const {
    assert(this->obereKante != -1); // erst sollte die obere Kante berechnet worden sein
//    cout << "Berechne Linke Kante: Radius = " << radius << ", Startwert = " << startwertX << " (" << startwertX << "-" << startwertX+2*radius+1 << ")" << endl;
    for (int x = startwertX + radius + 1; x <= this->bild.width()/2; x += 2*radius+1) {
        for (int y = this->obereKante + radius; y <= this->bild.height()-2-radius; y += 2*radius+1) {
            if (!this->istKomplettWeiss(x, y, radius)) {
                if (radius == 0) {
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

const int Koordinatenfinder::berechneRechteKante(const int startwertX, const int radius) const {
    assert(this->untereKante != -1); // erst sollte die untere Kante berechnet worden sein
//    cout << "Berechne Rechte Kante: Radius = " << radius << ", Startwert = " << startwertX << " (" << startwertX << "-" << startwertX+2*radius+1 << ")" << endl;
    for (int x = startwertX - radius - 1; x >= this->linkeKante; x -= 2*radius+1) {
        for (int y = this->obereKante + radius; y <= this->untereKante; y += 2*radius+1) {
            if (!this->istKomplettWeiss(x, y, radius)) {
                if (radius == 0) {
                    return x;
                }
                else {
                    return this->berechneRechteKante(x + (3*radius + 1), (radius-1)/3);
                }
            }
        }
    }
    cout << "Linke Kante wurde nicht gefunden" << endl;
    return 0;
}

const int Koordinatenfinder::berechneObereKante(const int startwertY, const int radius) {
    for (int y = startwertY + radius + 1; y <= this->bild.height()-2-radius; y += 2*radius+1) {
//        cout << "Berechne Obere Kante: Radius = " << radius << ", y = [" << y-radius-1 << ", " << y+radius+1 << "]" << endl;
        for (int x = radius + 1; x <= this->bild.width()-2-radius; x += 2*radius+1) {
            if (!this->istKomplettWeiss(x, y, radius)) {
                if (radius == 0) {
                    cout << "Referenzecke gefunden! x = " << x << ", y = " << y << endl;
                    this->referenzEcke = pair<int, int>(x, y);
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

const int Koordinatenfinder::berechneUntereKante(const int startwertY, const int radius) const {
    assert(this->linkeKante != -1); // erst sollte die linke Kante berechnet worden sein
    for (int y = startwertY - radius - 1; y >= this->obereKante; y -= 2*radius+1) {
//        cout << "Berechne untere Kante: Radius = " << radius << ", y = [" << y-radius-1 << ", " << y+radius+1 << "]" << endl;
        for (int x = this->linkeKante+1; x <= this->bild.width()-2-radius; x += 2*radius+1) {
            if (!this->istKomplettWeiss(x, y, radius)) {
                if (radius == 0) {
                    return y;
                }
                else {
                    return this->berechneUntereKante(y + (3*radius + 1), (radius-1)/3);
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
