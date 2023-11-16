#include "pamiec_dzielona.h"


int pamdziel_otworz(const char *nazwa, int flagi, mode_t tryb)
{
    int deskryptor;

    if((deskryptor = shm_open(nazwa, flagi, tryb)) == -1){
        perror("SHM_OPEN ERROR!");
        _exit(EXIT_FAILURE);
    }

    return deskryptor;
}

void pamdziel_ustaw(int deskryptor, off_t dlugosc)
{
    if(ftruncate(deskryptor, dlugosc) == -1){
        perror("FTRUNCATE ERROR!");
        exit(EXIT_FAILURE);
    }
}

void pamdziel_zamknij(int deskryptor)
{
    if(close(deskryptor) == -1){
        perror("CLOSE ERROR!");
        exit(EXIT_FAILURE);
    }
}

void pamdziel_usun(const char *nazwa)
{
    if(shm_unlink(nazwa) == -1)
    {
        perror("SHM_UNLINK ERROR!");
        exit(EXIT_FAILURE);
    }
}

void *pamiec_map(void *adres, size_t dlugosc, int prot, int flagi, int deskryptor, off_t przesuniecie)
{
    void *zwroc;

    if((zwroc = mmap(adres, dlugosc, prot, flagi, deskryptor, przesuniecie)) == MAP_FAILED)
    {
        perror("MMAP ERROR!");
        exit(EXIT_FAILURE);
    }

    return zwroc;
}

void pamiec_usun_map(void *adres, size_t dlugosc)
{
    if(munmap(adres, dlugosc) == -1){
        perror("MUNMAP ERROR!");
        exit(EXIT_FAILURE);
    }
}

