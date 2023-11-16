#include "semafor.h"

sem_t *sem_stworz(const char *nazwa, int flaga, mode_t tryb, unsigned int wartosc)
{
    sem_t *semafor;

    if((semafor = sem_open(nazwa, flaga, tryb, wartosc)) == SEM_FAILED)
    {
        perror("SEM_CREATE ERROR!");
        _exit(EXIT_FAILURE);
    }

    return semafor;
}

sem_t *sem_otworz(const char *nazwa, int flaga)
{
    sem_t *semafor;

    if((semafor = sem_open(nazwa, flaga)) == SEM_FAILED)
    {
        perror("SEM_OPEN ERROR!");
        exit(EXIT_FAILURE);
    }

    return semafor;
}

void sem_wartosc(sem_t *semafor, int *wartosc_sem)
{
    if(sem_getvalue(semafor, wartosc_sem) == -1)
    {
        perror("SEM_GETVALUE ERROR!");
        exit(EXIT_FAILURE);
    }
}

void sem_opusc(sem_t *semafor)
{
    if(sem_post(semafor) == -1)
    {
        perror("SEM_POST ERROR!");
        exit(EXIT_FAILURE);
    }
}

void sem_podnies(sem_t *semafor)
{
    if(sem_wait(semafor) == -1)
    {
        perror("SEM_WAIT ERROR!");
        exit(EXIT_FAILURE);
    }
}

void sem_zamknij(sem_t *semafor)
{
    if(sem_close(semafor) == -1)
    {
        perror("SEM_CLOSE ERROR!");
        exit(EXIT_FAILURE);
    }
}

void sem_usun(const char *nazwa)
{
    if(sem_unlink(nazwa) == -1)
    {
        perror("SEM_UNLINK ERROR!");
        exit(EXIT_FAILURE);
    }
}