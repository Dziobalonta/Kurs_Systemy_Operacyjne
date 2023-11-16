#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

using std::cout;



void Usun()
{
    system("rm PotokNazwany");
    cout << "Usunieto potok: PotokNazwany\n";
}

void sigint_hndlr(int sig)
{
    cout << "Usuwam Potok i koncze program!\n";
    atexit(Usun);
    exit(0);
}

int main(int argc,char* argv[])
{
    if(atexit(Usun) == -1)
    {
        cout << "Blad wywolania funkcji Usun()\n";
        return 1;
    }
    signal(SIGINT,&sigint_hndlr);
    
    
    int status;
    if(mkfifo("PotokNazwany",0777) == -1) // R or W by anybody in the system
    {
        if (errno != EEXIST)
        {
            cout << "Nie mozna utworzyc pliku fifo!\n";
            return 1;
        }
    }
    pid_t id_prod = fork();
    switch (id_prod)
    {
    case -1:
        perror("Blad funkcji fork!");
        exit(1);
    case 0:
        execlp(argv[1],argv[1],"PotokNazwany","zrodlo.txt",NULL);
        perror("Blad funkcji execlp!");
        break;
    }
    
    pid_t id_kons = fork();
    switch (id_kons)
    {
    case -1:
        perror("Blad funkcji fork!");
        exit(1);
    case 0:
        execlp(argv[2],argv[2],"PotokNazwany","destynacja.txt",NULL);
        perror("Blad funkcji execlp!");
        break;
    }
    
    waitpid(id_prod, &status, 0);
    waitpid(id_kons, &status, 0);

    
    return 0;
}
