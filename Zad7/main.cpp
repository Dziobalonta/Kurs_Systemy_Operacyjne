#include <stdlib.h>
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

#include "semafor.h"
#include "pamiec_dzielona.h"

#define ROZMIAR 16
#define PAM_DZIEL_NAZWA "/pamiec_dzielona"
#define SEM_PROD_NAZWA "/sem_prod" 
#define SEM_KONS_NAZWA "/sem_kons" 
#define KONS_SEMAFOR_POCZ_WAR 0

using std::cout;
using std::endl;

sem_t *prod_ptr = NULL;     //pointer do semafora producenta
sem_t *kons_ptr = NULL;    //pointer do semafora konsumenta
int memory_descryptor;    //deskryptor do pamieci


//SegmentPD Segment pamieci dzielonej
typedef struct {
    char bufor[NBUF][NELE];      //wspolny bufor danych
    int wstaw, wyjmij;          // pozycja wstawiania/wyjmowania z bufora 
} SegmentPD;

void sig_hndlr(int signal)
{
    if (signal == SIGINT)
    {
        cout << "Sygnal SIGINT przechwycony i obsluzony!\n";
        exit(signal);
    }
}

void cleaner()
{
    sem_zamknij(prod_ptr);
    sem_usun(SEM_PROD_NAZWA);

    sem_zamknij(kons_ptr);
    sem_usun(SEM_KONS_NAZWA);

    pamdziel_zamknij(memory_descryptor);
    pamdziel_usun(PAM_DZIEL_NAZWA);

    cout << "Semafory i pamiec dzielona zamkniete i usuniete pomyslnie!\n";
}

 
// argv[1]  nazwa producenta
// argv[2]  nazwa konsumenta
// argv[3]  nazwa pliku wejsciowego
// argv[4]  nazwa pliku wyjsciowego
    
int main(int argc, char *argv[])
{   
    
    // sem_zamknij(prod_ptr);
    // sem_zamknij(kons_ptr);
    // pamdziel_zamknij(memory_descryptor);

    
    // sem_usun(SEM_PROD_NAZWA);
    // sem_usun(SEM_KONS_NAZWA);
    // pamdziel_usun(PAM_DZIEL_NAZWA);
    
    if (atexit(cleaner) != 0)
    {
        cout << "Błąd przy rejestrowaniu funkcji sem_zamknij" << endl;
        return 1;
    }
    signal(SIGINT, sig_hndlr);

    memory_descryptor = pamdziel_otworz(PAM_DZIEL_NAZWA, O_CREAT | O_EXCL | O_TRUNC | O_RDWR, 0644);  //utworzenie pamieci dzielonej
    pamdziel_ustaw(memory_descryptor, sizeof(SegmentPD));

    sem_t *SEM_PROD = sem_stworz(SEM_PROD_NAZWA, O_CREAT | O_EXCL, 0644, NBUF);                     //semafor producenta
    sem_t *SEM_KONS = sem_stworz(SEM_KONS_NAZWA, O_CREAT | O_EXCL, 0644, KONS_SEMAFOR_POCZ_WAR);   //semafor konsumenta

    //ustawienie wartosci na semafory (bez tego cleaner nie zadziala)
    prod_ptr = SEM_PROD;
    kons_ptr = SEM_KONS;
    
    int value_prod; //wartosc semafora PROD
    int value_kons; //semafora KONS
    sem_wartosc(SEM_PROD, &value_prod);
    sem_wartosc(SEM_KONS, &value_kons);

    cout << "\nMAIN -\tNazwa semafora: " << SEM_PROD_NAZWA << " adres: "<< (void *)SEM_PROD << " wartosc startowa: " << value_prod << endl;
    cout << "MAIN -\tNazwa semafora: " << SEM_KONS_NAZWA << " adres: "<< (void *)SEM_KONS << " wartosc startowa: " << value_kons << endl;
    cout << "MAIN \tDeskryptor PD: " << memory_descryptor << "rozmiar w bajtach: " << sizeof(SegmentPD) << endl;
    sleep(1);

    char size[8];           //tablica przechowujaca wielkosc chunkow podawanych/pobieranych przez producenta/konsumenta
    sprintf(size, "%d", ROZMIAR);

    int id;
    for(int i = 0; i < 2; i++)
    {  
        id = fork();
        switch(id)
        {
            case -1:
                perror("FORK ERROR\n");
                exit(EXIT_FAILURE);

            case 0:
                
                if(i == 0)      //PRODUCENT
                {
                    execlp(argv[1], argv[1], PAM_DZIEL_NAZWA, size, SEM_PROD_NAZWA, SEM_KONS_NAZWA, argv[3], NULL);
                    perror("EXECL ERROR!");
                    _exit(EXIT_FAILURE);
                }
                
                else        //KONSUMENT
                {
                    execlp(argv[2], argv[2], PAM_DZIEL_NAZWA, size, SEM_PROD_NAZWA, SEM_KONS_NAZWA, argv[4], NULL);
                    perror("EXECL ERROR!");
                    _exit(EXIT_FAILURE);
                }

            default:
                break;
        }
    }

    //proces macierzysty czeka na zakoczenie procesow potomnych 
    for(int i = 0; i < 2; i++){
        if(wait(NULL) == -1){
            perror("WAIT ERROR\n");
            _exit(EXIT_FAILURE);
        }
    }

    return 0;
}