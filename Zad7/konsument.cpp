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
    
    SegmentPD *mem_kons = (SegmentPD *)pamiec_map(NULL, sizeof(SegmentPD), PROT_WRITE, MAP_SHARED, memory_descryptor, 0);   //pointer do segmentu pamieci
    mem_kons->wstaw = 0;    //inicjalizacja wstaw
    mem_kons->wyjmij = 0;   //inicjalizacja wyjmij

    sem_t *SEM_PROD = sem_otworz(argv[3], 0);     //semafor producent
    sem_t *SEM_KONS = sem_otworz(argv[4], 0);     //semafor konsument

    int value_prod; //zmienna przechowujaca wartosc semafora PROD
    int value_kons; //zmienna przechowujaca wartosc semafora KONS
    sem_wartosc(SEM_PROD, &value_prod);
    sem_wartosc(SEM_KONS, &value_kons);

    sleep(2);

    cout << "KONSUMENT -\tNazwa: " << argv[3] << " adres: " << (void *)SEM_PROD << " wartosc: " <<  value_prod << endl;
    cout << "KONSUMENT -\tNazwa: " << argv[4] << " adres: " << (void *)SEM_KONS << " wartosc: " <<  value_kons << endl;
    cout << "KONSUMENT -\tDeskryptor PD: " << memory_descryptor << "\n\n";

    int file;      //deskryptor pliku wejsciowego
    if((file = open(argv[5], O_CREAT | O_TRUNC | O_WRONLY, 0644)) == -1){
        perror("OPEN ERROR!");
        _exit(EXIT_FAILURE);
    }

    char towar[size];   //towar tablica znakowa przechowujaca zczytane znaki
    memset(towar, 0, NELE);    //inicjalizacja tablicy towar

    while(true)
    {
        sem_opusc(SEM_KONS);
        sleep(rand() % 4);

        strcpy(towar, mem_kons->bufor[mem_kons->wyjmij]);

        if(strcmp(towar, "\0") == 0){       //jesli towar == "\0", koniec (wiadomosc od producenta)
            cout << "\t\tKONSUMENT ZAKONCZYL CZYTANIE\n";
            sem_podnies(SEM_PROD);
            break;
        }

        sem_wartosc(SEM_KONS, &value_kons);
        cout << "KONSUMENT - Wartosc semafora: " << value_kons << " pozycja w buforze: "<< mem_kons->wyjmij << " przesylany tekst: " << towar << " ilosc znakow: " <<  strlen(towar) << "\n\n";
        mem_kons->wyjmij = (mem_kons->wyjmij + 1) % NBUF;   //przesuniecie pozycji wyjmowania o 1

        write(file, towar, sizeof(char)*strlen(towar));    //wpisanie ciagu znakow do pliku

        memset(towar, 0, size);     //wyczyszczenie tablicy towar

        sem_podnies(SEM_PROD);
    }

    // Po zakonczeniu dzialania zamykamy wszystkie otwarte wczesniej deskryptory
    if(close(file) == -1){
        perror("CLOSE ERROR!");
        exit(EXIT_FAILURE);
    }
    sem_zamknij(SEM_PROD);
    sem_zamknij(SEM_KONS);
    pamiec_usun_map(mem_kons, sizeof(SegmentPD));
    pamdziel_zamknij(memory_descryptor);

    return 0;
}