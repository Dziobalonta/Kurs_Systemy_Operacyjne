#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <string.h>
#include <ctime>

#include "semafor.h"
#include "pamiec_dzielona.h"

using std::cout;
using std::endl;

//SegmentPD Segment pamieci dzielonej
typedef struct {
    char bufor[NBUF][NELE]; //wspolny bufor danych
    int wstaw, wyjmij; // pozycja wstawiania/wyjmowania z bufora 
} SegmentPD;


// argv[1] nazwa pamieci dzielonej
// argv[2] wielkosc chunkow
// argv[3] nazwa semafora producenta
// argv[4] nazwa semafora konsumenta
// argv[5] nazwa pliku wejsciowego

int main(int argc, char *argv[])
{
    int size = atoi(argv[2]);       //wielkosc tablicy towar
    int memory_descryptor = pamdziel_otworz(argv[1], O_RDWR, 0644); //deskryptor do pamieci dzielonej
    
    SegmentPD *mem_prod = (SegmentPD *)pamiec_map(NULL, sizeof(SegmentPD), PROT_WRITE, MAP_SHARED, memory_descryptor, 0);   //pointer do segmentu pamieci
    mem_prod->wstaw = 0;    //inicjalizacja wstaw
    mem_prod->wyjmij = 0;   //inicjalizacja wyjmij

    sem_t *SEM_PROD = sem_otworz(argv[3], 0);     //semafor producent
    sem_t *SEM_KONS = sem_otworz(argv[4], 0);     //semafor konsument

    int value_prod; //zmienna przechowujaca wartosc semafora PROD
    int value_kons; //zmienna przechowujaca wartosc semafora KONS
    sem_wartosc(SEM_PROD, &value_prod);
    sem_wartosc(SEM_KONS, &value_kons);

    sleep(1);

    cout << "PRODUCENT -\tNazwa: " << argv[3] << " adres: " << (void *)SEM_PROD << " wartosc: " <<  value_prod << endl;
    cout << "PRODUCENT -\tNazwa: " << argv[4] << " adres: " << (void *)SEM_KONS << " wartosc: " <<  value_kons << endl;
    cout << "PRODUCENT -\tDeskryptor PD: " << memory_descryptor << endl;

    int file;      //deskryptor pliku wejsciowego
    if((file = open(argv[5], O_RDONLY, 0644)) == -1)
    {
        perror("OPEN ERROR!");
        _exit(EXIT_FAILURE);
    }

    int bytes;  //ilosc bitow pobieranych przez read
    char towar[size];   //towar tablica znakowa przechowujaca zczytane znaki
    memset(towar, 0, NELE);    //ustawienie tablicy towar na zera

    while(true)
    {
        sem_opusc(SEM_PROD);
        sleep(rand() % 2);
        bytes = read(file, towar, size);   //czytanie do towar'u z pliku ciagu znakow
        if(bytes == -1){
           perror("READ ERROR!");
            break;
        }
        if(bytes == 0)          //read nic nie przeczytal (sknoczyly sie znaki)
        {         
            cout << "\t\tPRODUCENT ZAKONCZYL CZYTANIE\n";
            sem_podnies(SEM_KONS);
            strcpy(mem_prod->bufor[mem_prod->wstaw], "\0");     //przekazanie do tablicy w pamieci znaku "\0" ktory zakonczy konsumenta
            break;
        }
        sem_wartosc(SEM_PROD, &value_prod);
        cout << "PRODUCENT - Wartosc semafora: " << value_prod << " pozycja w buforze: "<< mem_prod->wstaw << " przesylany tekst: " << towar << "ilosc znakow: " <<  strlen(towar) << "\n\n";
        strcpy(mem_prod->bufor[mem_prod->wstaw], towar);    //do bufora w pamieci umieszczamy towar pobrany przez read
        
        mem_prod->wstaw = (mem_prod->wstaw + 1) % NBUF;     //przesuniecie pozycji wstawiania o 1
        memset(towar, 0, size);     //czyszczenie tablicy towar

        sem_podnies(SEM_KONS);
    }

    if(close(file) == -1){
        perror("CLOSE ERROR!\n");
        exit(EXIT_FAILURE);
    }
    sem_zamknij(SEM_PROD);
    sem_zamknij(SEM_KONS);
    pamiec_usun_map(mem_prod, sizeof(SegmentPD));
    pamdziel_zamknij(memory_descryptor);


    return 0;
}