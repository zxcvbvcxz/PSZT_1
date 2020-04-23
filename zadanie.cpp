#include "zadanie.h"

zadanie::zadanie(int czas, int dead, int id)
{
    nowy_proc = false;
    id_zadania = id;
    czas_trwania = czas;
    deadline = dead;
    calkowity_czas_trwania = 0;
}

zadanie::zadanie()
{
    nowy_proc = true;
    id_zadania = 0;
    czas_trwania = 0;
    deadline = 0;
    calkowity_czas_trwania = 0;
}

zadanie::zadanie(const zadanie& a)
{
    id_zadania = a.id_zadania;
    nowy_proc = a.nowy_proc;
    czas_trwania = a.czas_trwania;
    deadline = a.deadline;
    calkowity_czas_trwania = a.calkowity_czas_trwania;
}

