#pragma once
#include <semaphore.h>
#include <string>

using std::string;

class semafor
{
private:
    sem_t* semaphore;
    string nazwa;
public:
    semafor();
    ~semafor();

    sem_t* sem_tworz(unsigned int krytyczne);
    void sem_otworz(char* tytul);
    int sem_wartosc();
    void sem_opusc();
    void sem_podnies();
    void sem_zamknij();
    void sem_usun();


};
