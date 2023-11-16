#include <iostream>
#include <fstream>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

using std::cout;
using std::ofstream;
using std::ifstream;
using std::endl;

int main(int argc,char* argv[])
{    
    
    ///////////-Wlasne sprawy-///////////
    sem_t* semafor;
    semafor = sem_open(argv[1], O_RDWR);
    if(semafor == SEM_FAILED)
    {
        perror("Blad otwierania semafora!");
        exit(EXIT_FAILURE);
    }

    int sem_wartosc = 0;
    sem_getvalue(semafor, &sem_wartosc);
    cout << "\tProces o PID: " << getpid() << endl;
    cout << "\tWartosc semafora PRZED SK:" << sem_wartosc << endl;

    sem_wait(semafor);                                            //opuszczenie (zajecie) semafora                   

    srand(time(NULL));
    sleep(rand() % 2);

    ///////////-Sekcja krytyczna-///////////

    ifstream czytaj(argv[2]);
    int liczba = 0;
    czytaj >> liczba;
    czytaj.close();

    sleep(rand() % 3);

    sem_getvalue(semafor, &sem_wartosc);
    cout << "\tWartosc semafora W SK:" << sem_wartosc << endl;
    cout << "\tOdczytana wartość: " << liczba << endl;
    ofstream zapisz(argv[2],std::ios::trunc);
    zapisz << liczba+1;
    zapisz.close();

    cout << "\tInkrementacja wykonana, zwalniam semafor\n";
    sem_post(semafor);                                            //podniesienie (zwolnienie) semafora

    ///////////-Reszta-///////////

    sem_getvalue(semafor, &sem_wartosc);
    cout << "\tWartosc semafora PO SK:" << sem_wartosc << endl;
    sem_close(semafor);
    return 0;
}