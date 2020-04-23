#ifndef FUNKCJE_EWOLUCYJNE_H
#define FUNKCJE_EWOLUCYJNE_H

#include "zadanie.h"
#include "osobnik.h"
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

void wygeneruj_pierwsza_populacje(ifstream &a, vector<osobnik>& populacja, int liczba_osobnikow, int liczba_procesorow);
void wylicz_opoznienia_i_funkcje_przystosowania(vector<osobnik>& populacja, int liczba_osobnikow);
void reprodukuj_potomnych(vector<osobnik>& rodzice, vector<osobnik>& potomni, int liczba_osobnikow, int ile_procesorow);
void wybierz_najlepszych(vector<osobnik>& populacja_P, vector<osobnik>& potomni_R);

#endif /* FUNKCJE_EWOLUCYJNE_H */

