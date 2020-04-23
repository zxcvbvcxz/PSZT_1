#include <cstdlib>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <ctime> 
#include <climits>
#include "wspolne.h"
#include "zadanie.h"
#include "osobnik.h"
#include "funkcje_ewolucyjne.h"
#include <chrono>
#include <string>
using namespace std;

int g_iteracje_bez_poprawy = 100;
int g_licz_osobnikow = MI;

int main(int argc, char** argv)
{
    srand(unsigned ( std::time(0)));
    int liczba_procesorow;
    char* pchNazwaPliku;
    int t;
    if (argc > 3)
    {
        pchNazwaPliku = argv[1];
        t = atoi(argv[2]); // odczytaj liczbe iteracji bez poprawy
        if (t < 30 || t > 500)
        {
            cout << "Podana liczba iteracji bez poprawy po ktorych zachodzi przerwanie, powinna byc z zakresu [30;500]" << endl;
            exit(0);
        } else
            g_iteracje_bez_poprawy = t;
        t = atoi(argv[3]); // odczytaj liczbe osobnikow
        if (t < 10 || t > 500)
        {
            cout << "Podana liczba osobnikow w populacji powinna byc z zakresu [10;500]" << endl;
            exit(0);
        } else
            g_licz_osobnikow = t;
    } else
        if (argc > 2)
    {
        pchNazwaPliku = argv[1];
        t = atoi(argv[2]); // odczytaj liczbe iteracji bez poprawy
        if (t < 30 || t > 500)
        {
            cout << "Podana liczba iteracji bez poprawy po ktorych zachodzi przerwanie, powinna byc z zakresu [30;500]" << endl;
            exit(0);
        } else
            g_iteracje_bez_poprawy = t;
    } else
        if (argc > 1)
    {
        pchNazwaPliku = argv[1];
    } else
    {
        cout << "Za mala liczba argumentow" << endl;
        return -1;
    }

    ifstream in;
    in.open(pchNazwaPliku);
    if (!in.is_open())
    {
        cout << "Blad otwarcia pliku wejsciowego!";
        return -1;
    }

    in >> liczba_procesorow;
    if (liczba_procesorow < 1)
    {
        cout << "Bledne dane w pliku wejsciowym\n";
        exit(0);
    }


    auto start_stoper = chrono::high_resolution_clock::now(); // zacznij liczyć czas wykonania algorytmu    

    vector<osobnik> populacja_P(g_licz_osobnikow), potomni_R(g_licz_osobnikow);
    wygeneruj_pierwsza_populacje(in, populacja_P, g_licz_osobnikow, liczba_procesorow);
    in.close();

    int liczba_zadan = populacja_P[0].Zadania().size() - (liczba_procesorow - 1);
    int temp = 1;
    int wynik = INT_MAX;
    int licznik = g_iteracje_bez_poprawy;
    int iteracja = 1;
    printf("iteracja:%-5d| Suma opoznien:%d\n", 0, populacja_P[0].GetPrzystosowanie());
    while (true)
    {
        if (licznik == 0)
        {
            break;
        }
        //###########################################################################
        reprodukuj_potomnych(populacja_P, potomni_R, g_licz_osobnikow, liczba_procesorow);
        wybierz_najlepszych(populacja_P, potomni_R); // wynik dzialania zwracany w tablicy populacja_P
        //###########################################################################
        if (populacja_P[0].GetPrzystosowanie() < wynik)
        {
            wynik = populacja_P[0].GetPrzystosowanie();
            licznik = g_iteracje_bez_poprawy + 1;
        }
        printf("iteracja:%-5d| Suma opoznien:%d\n", iteracja, populacja_P[0].GetPrzystosowanie());
        licznik--;
        iteracja++;
    }
    auto stop_stoper = chrono::high_resolution_clock::now(); // koniec mierzenia czasu wykonania
    auto czas_wykonania = chrono::duration_cast<chrono::seconds>(stop_stoper - start_stoper);


    ofstream outfile;
    string nazwaPlikuWy = (string) argv[1];
    nazwaPlikuWy.resize(nazwaPlikuWy.size() - 4);
    nazwaPlikuWy += "_WYNIK.txt";
    outfile.open(nazwaPlikuWy, ios::out | ios::trunc);
    if (!outfile.is_open())
    {
        cout << "Blad otwarcia pliku wyjsciowego!";
        exit(0);
    }
    cout << "Wynik zostal zapisany do pliku: " << nazwaPlikuWy << endl;

    // wypisanie rezultatow
    outfile << "Plik wejsciowy: " << pchNazwaPliku << endl;
    cout << "Liczebnosc populacji: " << g_licz_osobnikow << endl;
    outfile << "Liczebnosc populacji: " << g_licz_osobnikow << endl;
    cout << "Liczba iteracji bez poprawy: " << g_iteracje_bez_poprawy << endl;
    outfile << "Liczba iteracji bez poprawy: " << g_iteracje_bez_poprawy << endl;
    cout << "Liczba zadan w pliku we: " << liczba_zadan << endl;
    outfile << "Liczba zadan w pliku we: " << liczba_zadan << endl;
    cout << "Liczba procesorow: " << liczba_procesorow << endl;
    outfile << "Liczba procesorow: " << liczba_procesorow << endl;
    outfile << "Rezultat: " << endl;
    cout << "Iteracje:" << iteracja << endl;
    outfile << "Iteracje: " << iteracja << endl;
    cout << "Czas wykonania(sek): " << czas_wykonania.count() << endl;
    outfile << "Czas wykonania(sek): " << czas_wykonania.count() << endl;
    cout << "Suma opoznien znalezionego rozwiazania: " << populacja_P[0].GetPrzystosowanie() << endl;
    outfile << "Suma opoznien znalezionego rozwiazania: " << populacja_P[0].GetPrzystosowanie() << endl;
    cout << "Harmonogram:"<<endl;
    outfile << "Harmonogram:"<<endl;
    temp = 1;
    cout << "Procesor " << temp << ": ";
    outfile << "Procesor " << temp << ": ";
    for (TVecZadanieIt j = populacja_P[0].Zadania().begin(); j != populacja_P[0].Zadania().end(); j++)
    {
        if (j->GetNowy_proc())
        {
            temp++;
            cout << endl;
            outfile << endl;
            cout << "Procesor " << temp << ": ";
            outfile << "Procesor " << temp << ": ";
            continue;
        }
        cout << j->GetID() << " ";
        outfile << j->GetID() << " ";
    }
    outfile.close();

}

