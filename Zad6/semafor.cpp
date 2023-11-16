#include "semafor.h"
#include <iostream>
#include <fcntl.h>
#include <semaphore.h>

using std::cout;
using std::endl;

semafor::semafor()
{
    semaphore = nullptr;
    nazwa = "/moj_semafor";
}

semafor::~semafor()
{
}

sem_t* semafor::sem_tworz(unsigned int krytyczne)
{
    semaphore = sem_open(nazwa.c_str(), O_CREAT | O_EXCL, 0666, krytyczne);
    if(semaphore == SEM_FAILED)
    {
        perror("Blad tworzenia semafora!");
        sem_zamknij();
        exit(EXIT_FAILURE);
    }
    return semaphore;
}

void semafor::sem_otworz(char* tytul)
{
    semaphore = sem_open(tytul, O_RDWR);
    if(semaphore == SEM_FAILED)
    {
        perror("Blad otwierania semafora!");
        exit(EXIT_FAILURE);
    }
}

int semafor::sem_wartosc()
{
    int value;
    if (sem_getvalue(semaphore, &value) == -1)
    {
        perror("Blad pobierania wartosci semafora!\n");
        exit(EXIT_FAILURE);
    }
    return value;
}

void semafor::sem_opusc()
{
    if (sem_wait(semaphore) == -1)
    {
        perror("Blad opuszczania semafora!\n");
        exit(EXIT_FAILURE);
    }
}

void semafor::sem_podnies()
{
    if (sem_post(semaphore) == -1)
    {
        perror("Blad podnoszenia semafora!\n");
        exit(EXIT_FAILURE);
    }
}

void semafor::sem_zamknij()
{
    if (sem_close(semaphore) == -1)
    {
        perror("Blad zamykania semafora!\n");
        exit(EXIT_FAILURE);
    }
}

void semafor::sem_usun()
{
    if (sem_unlink(nazwa.c_str()) == -1)
    {
        perror("Blad usuwania semafora!");
        exit(EXIT_FAILURE);
    }
}
