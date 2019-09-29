#include <chrono>
#include <cmath>
#include <iostream>
#include <string>

#include "Binaersuche.hpp"

#include "Koordinatenfinder.hpp"

using namespace std::chrono;
using namespace cimg_library;
using namespace std;

Koordinatenfinder::Koordinatenfinder(const string& dateiname) : bild(dateiname.c_str()), dateiname(dateiname.substr(0,dateiname.size()-5)) {
//    cout << "Koordinatenfinder::Konstruktor. Dateiname ist " << this->dateiname << ".jpeg" << endl;
    assert(bild.width() == 2480);
    assert(bild.height() == 3508);
    
//    const auto tic{std::chrono::high_resolution_clock::now()};
    
    this->berechneEcken();
    
//    const auto toc{std::chrono::high_resolution_clock::now()};
//    const duration<double, std::milli> ms{toc-tic};
//    cout << "Zeit: " << ms.count() << endl;
    
//    cout << "Ecke oben links: (" << this->eckeObenLinks.first << ", " << this->eckeObenLinks.second << ")" << endl;
//    cout << "Ecke unten links: (" << this->eckeUntenLinks.first << ", " << this->eckeUntenLinks.second << ")" << endl;
//    cout << "Ecke unten rechts: (" << this->eckeUntenRechts.first << ", " << this->eckeUntenRechts.second << ")" << endl;
//    cout << "Ecke oben rechts: (" << this->eckeObenRechts.first << ", " << this->eckeObenRechts.second << ")" << endl;
}

void Koordinatenfinder::fertigStellen() {
//    cout << "Fertig stellen..." << endl;
    this->bildDrehen();
    this->bildZuschneiden();
    this->bildSichern();
}

const double Koordinatenfinder::getWinkel() const {
    const int y{-(this->eckeObenRechts.second - this->eckeObenLinks.second)};
    const int x{this->eckeObenRechts.first - this->eckeObenLinks.first};
    const double tangens{(y+0.)/(x+0.)};
    return atan(tangens) * 180 / M_PI;
}

const pair<int, int> Koordinatenfinder::getEckeObenLinks() const {
    return this->eckeObenLinks;
}

void Koordinatenfinder::bildDrehen() {
    const float winkel{(float)this->getWinkel()};
    cout << "Winkel = " << winkel << endl;
//    cout << "Drehen..." << endl;
    this->bild = this->bild.get_rotate(winkel, (float)(this->getEckeObenLinks().first), (float)(this->getEckeObenLinks().second), 1);
}

void Koordinatenfinder::bildZuschneiden() {
    this->berechneAbmessungen();
//    cout << "Zuschneiden..." << endl;
//    cout << "Breite = " << this->width << endl;
//    cout << "Höhe = " << this->height << endl;
    this->bild = this->bild.get_crop(this->getEckeObenLinks().first, this->getEckeObenLinks().second, 0, 0, this->getEckeObenLinks().first+this->width, this->getEckeObenLinks().second+this->height, 0, 2);
}

void Koordinatenfinder::bildSichern() {
//    cout << "Sichern..." << endl;
    string neuerDateiname{this->dateiname};
    neuerDateiname.append("_export.jpg");
    this->bild.save_jpeg(neuerDateiname.c_str(), 75);
}

void Koordinatenfinder::berechneEcken() {
    this->obereKante = this->berechneObereKante(1000, 40);
    this->linkeKante = this->berechneLinkeKante(100, 40);
    this->untereKante = this->berechneUntereKante(this->bild.height()-1, 40);
    this->rechteKante = this->berechneRechteKante(this->bild.width()-1, 40);
    
    if (this->referenzEcken[0].first < this->bild.width()/2) {
        this->eckeObenLinks = this->referenzEcken[0];
        this->eckeUntenLinks = this->referenzEcken[1];
        this->eckeUntenRechts = this->referenzEcken[2];
        this->eckeObenRechts = this->referenzEcken[3];
    }
    else {
        this->eckeObenLinks = this->referenzEcken[1];
        this->eckeUntenLinks = this->referenzEcken[2];
        this->eckeUntenRechts = this->referenzEcken[3];
        this->eckeObenRechts = this->referenzEcken[0];
    }
}

void Koordinatenfinder::berechneAbmessungen() {
//    cout << "Berechne Abmessungen..." << endl;
    const int y1{(this->eckeObenRechts.second - this->eckeObenLinks.second)};
    const int x1{this->eckeObenRechts.first - this->eckeObenLinks.first};
    this->width = sqrt(x1*x1 + y1*y1);
    const int y2{(this->eckeUntenLinks.second - this->eckeObenLinks.second)};
    const int x2{this->eckeUntenLinks.first - this->eckeObenLinks.first};
    this->height = sqrt(x2*x2 + y2*y2);
}

const int Koordinatenfinder::berechneLinkeKante(const int startwertX, const int radius) {
    assert(this->obereKante != -1); // erst sollte die obere Kante berechnet worden sein
//    cout << "Berechne Linke Kante: Radius = " << radius << ", Startwert = " << startwertX << " (" << startwertX << "-" << startwertX+2*radius+1 << ")" << endl;
    for (int x = startwertX + radius + 1; x <= this->bild.width()/2; x += 2*radius+1) {
        for (int y = this->obereKante + radius; y <= this->bild.height()-2-radius; y += 2*radius+1) {
            if (!this->istKomplettWeiss(x, y, radius)) {
                if (radius == 0) {
                    this->referenzEcken.push_back(pair<int, int>(x, y));
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

const int Koordinatenfinder::berechneRechteKante(const int startwertX, const int radius) {
    assert(this->untereKante != -1); // erst sollte die untere Kante berechnet worden sein
//    cout << "Berechne Rechte Kante: Radius = " << radius << ", Startwert = " << startwertX << " (" << startwertX << "-" << startwertX+2*radius+1 << ")" << endl;
    for (int x = startwertX - radius - 1; x >= this->linkeKante; x -= 2*radius+1) {
        for (int y = this->obereKante + radius; y <= this->untereKante; y += 2*radius+1) {
            if (!this->istKomplettWeiss(x, y, radius)) {
                if (radius == 0) {
                    this->referenzEcken.push_back(pair<int, int>(x, y));
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
                    this->referenzEcken.push_back(pair<int, int>(x, y));
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

const int Koordinatenfinder::berechneUntereKante(const int startwertY, const int radius) {
    assert(this->linkeKante != -1); // erst sollte die linke Kante berechnet worden sein
    for (int y = startwertY - radius - 1; y >= this->obereKante; y -= 2*radius+1) {
//        cout << "Berechne untere Kante: Radius = " << radius << ", y = [" << y-radius-1 << ", " << y+radius+1 << "]" << endl;
        for (int x = this->linkeKante+1; x <= this->bild.width()-2-radius; x += 2*radius+1) {
            if (!this->istKomplettWeiss(x, y, radius)) {
                if (radius == 0) {
                    this->referenzEcken.push_back(pair<int, int>(x, y));
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
