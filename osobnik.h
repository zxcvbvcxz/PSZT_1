#ifndef OSOBNIK_H
#define OSOBNIK_H

#include "zadanie.h"
#include <vector>

using namespace std;

class osobnik
{
    TVecZadanie poszeregowane_zadania;
    int         wartosc_funkcji_przystosowania = 0;

public:
    osobnik &operator = (const osobnik & a);

    TVecZadanie&        Zadania()                   { return poszeregowane_zadania;  }
    void                SetZadania(TVecZadanie& v)  { poszeregowane_zadania = v; }
    int                 GetPrzystosowanie() const   { return wartosc_funkcji_przystosowania; }
    void                SetPrzystosowanie(int v)    { wartosc_funkcji_przystosowania = v; }
};

#endif /* OSOBNIK_H */

