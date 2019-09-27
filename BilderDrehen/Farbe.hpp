#ifndef Farbe_hpp
#define Farbe_hpp

#include <stdio.h>

struct Farbe {
    Farbe(const int r, const int g, const int b);
    
    const int r;
    const int g;
    const int b;
};

#endif /* Farbe_hpp */
