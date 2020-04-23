#include "osobnik.h"

osobnik & osobnik::operator = (const osobnik & a)
{
    poszeregowane_zadania = a.poszeregowane_zadania;
    wartosc_funkcji_przystosowania = a.wartosc_funkcji_przystosowania;
    return *this;
}

