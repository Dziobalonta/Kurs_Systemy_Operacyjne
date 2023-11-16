#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fstream>

#define SEMAFOR_NAZWA "/moj_semafor"
#define PLIK_NAZWA "numer.txt"

using std::cout;
using std::ofstream;
using std::ifstream;
using std::string;
using std::endl;

sem_t* semafor;

void sem_zamknij()
{
    sem_close(semafor);
    sem_unlink(SEMAFOR_NAZWA);
    cout << "Semafor zamkniety i usuniety pomyslnie!\n";
}

void sig_hndlr(int signal)
{
    if (signal == SIGINT)
    {
        cout << "Sygnal SIGINT przechwycony i obsluzony!\n";
        exit(signal);
    }
}

//make run PROCESY=3 KRYTYCZNE=1
int main(int argc,char* argv[])
{
    if (atexit(sem_zamknij) != 0)
    {
        cout << "Błąd przy rejestrowaniu funkcji sem_zamknij" << endl;
        return 1;
    }
    signal(SIGINT, sig_hndlr);
    
    ofstream strumien(PLIK_NAZWA,std::ios::trunc);
    strumien << 2134;
    strumien.close();

    unsigned int n = atoi(argv[2]);
    unsigned int krytyczne = atoi(argv[3]);

    cout << "Wpisana wartosc do pliku: " << 2134 << ", oczekiwana wartosc: " << 2134+n << endl;
    semafor = sem_open(SEMAFOR_NAZWA, O_CREAT | O_EXCL, 0666, krytyczne);
    if(semafor == SEM_FAILED)
    {
        perror("Blad otwierania semafora!");
        sem_zamknij();
        exit(EXIT_FAILURE);
    }

    cout << "Stworzono semafor o adresie: " << semafor << ", z wartoscia poczatkowa: " << krytyczne << endl;

    for(int i=0;i < n; i++)
    {
        pid_t id = fork();
        switch (id)
        {
        case -1:
            perror("Blad funkcji fork!");
            sem_zamknij();
            exit(EXIT_FAILURE);
        case 0:
            execlp(argv[1], argv[1], SEMAFOR_NAZWA, PLIK_NAZWA, NULL);
            perror("Blad funkcji execlp!");
            sem_zamknij();
            exit(EXIT_FAILURE);
        default:
            wait(NULL);
            cout << "Proces potomny zakonczyl dzialanie\n";
            break;
        }
    }

    ifstream czytaj(PLIK_NAZWA);
    int wartosc;
    czytaj >> wartosc;
    czytaj.close();
    if(wartosc == 2134+n) cout << "Odczytana wartosc: " << wartosc << ", zgadza sie z oczekiwana!\n";
    else cout << "Wartosci NIE sa takie same! Cos poszlo nie tak\n";

    return 0;
}