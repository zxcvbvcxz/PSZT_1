#include "wspolne.h"
#include "funkcje_ewolucyjne.h"
#include <random>
using namespace std;

//default_random_engine gen;
random_device rd;
mt19937 mt(rd());

////////////////////////////////

/*funkcja pomocnicza dla wygeneruj_pierwsza_populacje(ifstream &a, osobnik* populacja, int liczba_osobnikow, int liczba_procesorow)
wykorzystywana przy sortowaniu osobnikow...
w kolejnosci od najlepszego - o najmniejszej wartosci f przystosowania
 */
bool cmp(const osobnik& a, const osobnik& b)
{
    return (a.GetPrzystosowanie() < b.GetPrzystosowanie());
}

/////////////////////////////////////////////////////////////////////////////

void wygeneruj_pierwsza_populacje(ifstream &a, vector<osobnik>& populacja, int liczba_osobnikow, int liczba_procesorow)
{
    int czas, deline;
    int id = 1;
    while (a >> czas and a >> deline)
    { //wczytywanie danych o procesach
        populacja[0].Zadania().push_back(zadanie(czas, deline, id));
        id++;
    }
    for (int i = 0; i < liczba_procesorow - 1; i++)
    { //dodawanie procesów rozdzielających na procesory
        populacja[0].Zadania().push_back(zadanie());
    }
    for (int i = 1; i < liczba_osobnikow; i++)
    { //klonowanie pierwszego elementu
        for (TVecZadanieIt j = populacja[i - 1].Zadania().begin(); j != populacja[i - 1].Zadania().end(); j++)
        {
            populacja[i].Zadania().push_back(zadanie(*j));
        }
    }
    for (int i = 0; i < liczba_osobnikow; i++)
    { //losowe przydzielanie procesów do procesorow dla każdego osobnika populacja_P
        random_shuffle(populacja[i].Zadania().begin(), populacja[i].Zadania().end());
    }
    wylicz_opoznienia_i_funkcje_przystosowania(populacja, liczba_osobnikow);
    sort(populacja.begin(), populacja.end(), cmp);

}

/////////////////////////////////////////////////////////////////////////////
//wyliczenie calkowity_czas i spoznienie dla kazdego procesu na kazdym procesorze

void wylicz_opoznienia_i_funkcje_przystosowania(vector<osobnik>& populacja, int liczba_osobnikow)
{
    for (int i = 0; i < liczba_osobnikow; i++)
    {
        for (TVecZadanieIt j = populacja[i].Zadania().begin(); j != populacja[i].Zadania().end(); j++)
        {
            if ((j)->GetNowy_proc() == true)
            {//jeśli rozdzielnik kontynuuj
                continue;
            }
            if (j == populacja[i].Zadania().begin())
            {

                j->SetCalkowity_czas(j->GetCzas_trwania());
                populacja[i].SetPrzystosowanie(populacja[i].GetPrzystosowanie() + (j->GetCalkowity_czas() > j->GetDeadline() ? j->GetCalkowity_czas() - j->GetDeadline() : 0));
                continue;
            }
            j->SetCalkowity_czas((j - 1)->GetCalkowity_czas() + j->GetCzas_trwania());
            populacja[i].SetPrzystosowanie(populacja[i].GetPrzystosowanie() + (j->GetCalkowity_czas() > j->GetDeadline() ? j->GetCalkowity_czas() - j->GetDeadline() : 0));
        }
    }
}

/////////////////////////////////////////////////////////////////////////////

void reprodukuj_potomnych(vector<osobnik>& rodzice, vector<osobnik>& potomni, int liczba_osobnikow, int ile_procesorow)
{
    // zapewnienie losowej generacji liczb z zakresu od 0 do size-1
    uniform_int_distribution<int> distribution(0, rodzice[0].Zadania().size() - 1);
    uniform_int_distribution<int> dist_proc(0, ile_procesorow - 2); // do losowania procesu rozdzielajacego
    uniform_int_distribution<int> jeden_z_dwoch(0, 1);
    for (int i = 0; i < liczba_osobnikow; i++)
    { //tworzenie zarodkow potomnych z populacji_P
        potomni[i].SetZadania(rodzice[i].Zadania());
    }
    int temp;
    if (ile_procesorow == 1)
    {
        temp = 0; //jak jest 1 procesor to nie robi zamiany dla procesu rozdzielającego (bo takiego nie ma) dla 10% osobnikow
    } else
    {
        temp = (int) (liczba_osobnikow / NAJGORSZE_PROCENT) >= 1 ? (int) (liczba_osobnikow / NAJGORSZE_PROCENT) : 1; //najgorsze 1/NAJGORSZE_PROCENT populacji d
    }
    TVecZadanieIt iter1;
    TVecZadanieIt iter2;
    for (int i = 0; i < liczba_osobnikow - temp; i++)
    { //zamiana 
        while (true)
        {
            iter1 = potomni[i].Zadania().begin() + distribution(mt);
            if (iter1->GetNowy_proc() == false)
            {
                break;
            }
        }
        while (true)
        {
            iter2 = potomni[i].Zadania().begin() + distribution(mt);
            if (iter2->GetNowy_proc() == false and iter2 != iter1)
            {
                break;
            }
        }
        swap(*iter1, *iter2);
    }
    int nr_rozdzielajacego, cnt;
    for (int i = liczba_osobnikow - temp; i < liczba_osobnikow; i++)
    { //zamiana 
        nr_rozdzielajacego = dist_proc(mt); //losuje ktore w kolejnosci zadanie rozdzielajace chcemy wybrac
        cnt = 0;
        //wyszukaj obiekt rozdzielajacy na procesory
        for (TVecZadanieIt j = potomni[i].Zadania().begin(); j != potomni[i].Zadania().end(); j++)
        {
            if (j->GetNowy_proc())
            {//jeśli rozdzielnik
                if (cnt == nr_rozdzielajacego)
                {
                    iter1 = j;
                    break;
                } else
                {
                    cnt++;
                    continue;
                }
            }
        }
                if (jeden_z_dwoch(mt))
                {
                    if (iter1 != potomni[i].Zadania().begin())
                    {
                        swap(*iter1, *(iter1 - 1));
                    }
                } else
                {
                    if ((iter1 + 1) != potomni[i].Zadania().end())
                    {
                        swap(*iter1, *(iter1 + 1));
                    }
                }
        while (true)
        {
            iter2 = potomni[i].Zadania().begin() + distribution(mt);
            if (iter2->GetNowy_proc() == false)
            {
                break;
            }
        }
        swap(*iter1, *iter2);
    }
    wylicz_opoznienia_i_funkcje_przystosowania(potomni, liczba_osobnikow);
    sort(potomni.begin(), potomni.end(), cmp);
}

/////////////////////////////////////////////////////////////////////////////
// Wybiera najlepszych osobnikow sposrod rodzicow i potomkow
// wynik umiesc w tablicy osobnikow- populacja_P

void wybierz_najlepszych(vector<osobnik>& populacja_P, vector<osobnik>& potomni_R)
{
    int i = 0, j = 0, k = 0;
    vector<osobnik> vTemp(g_licz_osobnikow);
    for (int a = 0; a < g_licz_osobnikow; a++)
    {
        vTemp[a] = populacja_P[a];
    }
    for (int licznik = 0; licznik < g_licz_osobnikow; licznik++)
    {
        if (vTemp[i].GetPrzystosowanie() <= potomni_R[j].GetPrzystosowanie() )
            populacja_P[k++] = vTemp[i++];
        else
            populacja_P[k++] = potomni_R[j++];
    }
    printf("rodzice:%-3d potomkowie:%-3d ", i, j);
    for (int a = 0; a < g_licz_osobnikow; a++)
    { //czyszczenie wszystkich potomkow po wybraniu nowej populacji
        potomni_R[a].Zadania().clear();
        potomni_R[a].SetPrzystosowanie(0);
    }
}



