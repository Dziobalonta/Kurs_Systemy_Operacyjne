#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#define NELE 20 //rozmiar elementu bufora (bajty)
#define NBUF 5 //liczba elementow bufora

int pamdziel_otworz(const char *nazwa, int flagi, mode_t tryb);

void pamdziel_ustaw(int deskryptor, off_t dlugosc);

void pamdziel_zamknij(int deskryptor);

void pamdziel_usun(const char *nazwa);

void *pamiec_map(void *adres, size_t dlugosc, int prot, int flagi, int deskryptor, off_t przesuniecie);

void pamiec_usun_map(void *adres, size_t dlugosc);
