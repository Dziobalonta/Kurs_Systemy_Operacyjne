#include <iostream>
#include <fstream>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include "semafor.h"

using std::cout;
using std::ofstream;
using std::ifstream;
using std::endl;

int main(int argc,char* argv[])
{    
    
    ///////////-Wlasne sprawy-///////////
    semafor MySemaphore;
    MySemaphore.sem_otworz(argv[1]);
    cout << "\tProces o PID: " << getpid() << endl;
    cout << "\tWartosc semafora PRZED SK:" << MySemaphore.sem_wartosc() << endl;

    MySemaphore.sem_opusc();                                        //opuszczenie (zajecie) semafora                   

    srand(time(NULL));
    sleep(rand() % 2);

    ///////////-Sekcja krytyczna-///////////

    ifstream czytaj(argv[2]);
    int liczba = 0;
    czytaj >> liczba;
    czytaj.close();

    sleep(rand() % 3);

    cout << "\tWartosc semafora W SK:" << MySemaphore.sem_wartosc() << endl;
    cout << "\tOdczytana wartość: " << liczba << endl;
    ofstream zapisz(argv[2],std::ios::trunc);
    zapisz << liczba+1;
    zapisz.close();

    cout << "\tInkrementacja wykonana, zwalniam semafor\n";
    MySemaphore.sem_podnies();                                        //podniesienie (zwolnienie) semafora

    ///////////-Reszta-///////////

    cout << "\tWartosc semafora PO SK:" << MySemaphore.sem_wartosc() << endl;
    MySemaphore.sem_zamknij();
    return 0;
}