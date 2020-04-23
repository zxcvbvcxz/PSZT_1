#ifndef ZADANIE_H
#define ZADANIE_H
#include <vector>

class zadanie
{
    bool nowy_proc = true;
    int id_zadania = 0;
    int czas_trwania = 0;
    int deadline = 0;
    int calkowity_czas_trwania = 0;

public:
    bool    GetNowy_proc()              { return nowy_proc; }
    int     GetCzas_trwania()           { return czas_trwania; }
    int     GetCalkowity_czas()         { return calkowity_czas_trwania; }
    void    SetCalkowity_czas(int v)    { calkowity_czas_trwania = v; }
    int     GetDeadline()               { return deadline; }
    int     GetID()                     { return id_zadania; }

    zadanie();
    zadanie(int czas, int dead, int id);
    zadanie(const zadanie& a);
};

typedef std::vector<zadanie>    TVecZadanie;
typedef TVecZadanie::iterator   TVecZadanieIt;
#endif /* ZADANIE_H */

