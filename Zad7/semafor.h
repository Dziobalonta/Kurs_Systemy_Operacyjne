#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

sem_t *sem_stworz(const char *nazwa, int flaga, mode_t tryb, unsigned int wartosc);

sem_t *sem_otworz(const char *nazwa, int flaga);

void sem_wartosc(sem_t *semafor, int *wartosc_sem);

void sem_opusc(sem_t *semafor);

void sem_podnies(sem_t *semafor);

void sem_zamknij(sem_t *semafor);

void sem_usun(const char *nazwa);