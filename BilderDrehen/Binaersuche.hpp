#ifndef Binaersuche_hpp
#define Binaersuche_hpp

#include <stdio.h>
#include <vector>

enum BinaerErgebnis{ZU_KLEIN, ZU_GROSS};

template<class T>
class Binaersuche {
public:
    Binaersuche(const T min, const T max, const T minStep);
    const T berechneNaechsteStelle(const BinaerErgebnis ergebnis);
    const T getAktuelleStelle() const;
    const bool istFertig() const;
private:
    const T min;
    const T max;
    const T minStep;
    
    std::vector<T> stellen;
    T aktuelleStelle;
    bool fertig = false;
    
    void stelleHinzufuegen(const T neueStelle);
};

template<typename T>
Binaersuche<T>::Binaersuche(const T min, const T max, const T minStep) : min{min}, max{max}, minStep{minStep} {
    assert(min < max);
    this->stellen = {min, max};
    
    this->aktuelleStelle = (min + max) / 2;
    this->stelleHinzufuegen(aktuelleStelle);
}

template<typename T>
const T Binaersuche<T>::berechneNaechsteStelle(const BinaerErgebnis ergebnis) {
    if (!fertig) {
        size_t i = 0;
        while (this->stellen[i] != aktuelleStelle) {
            ++i;
        }
        if (ergebnis == ZU_KLEIN) {
            this->stelleHinzufuegen((this->stellen[i] + this->stellen[i+1]) / 2);
        }
        else {
            this->stelleHinzufuegen((this->stellen[i-1] + this->stellen[i]) / 2);
        }
    }
    return this->aktuelleStelle;
}

template<typename T>
const T Binaersuche<T>::getAktuelleStelle() const {
    return this->aktuelleStelle;
}

template<typename T>
const bool Binaersuche<T>::istFertig() const {
    return this->fertig;
}

template<typename T>
void Binaersuche<T>::stelleHinzufuegen(const T neueStelle) {
    assert(this->min < neueStelle);
    assert(neueStelle < this->max);
    
    size_t i = 0;
    while (this->stellen[i] < neueStelle) {
        ++i;
    }
    
    if (this->stellen[i] - this->stellen[i-1] < 2*minStep) {
        this->fertig = true;
        return;
    }
    
    this->stellen.push_back(neueStelle);
    std::sort(this->stellen.begin(), this->stellen.end());
    
    this->aktuelleStelle = neueStelle;
}

#endif /* Binaersuche_hpp */
