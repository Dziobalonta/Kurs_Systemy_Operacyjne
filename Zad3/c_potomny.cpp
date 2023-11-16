#include <iostream>
#include <csignal>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

using std::cout;
using std::endl;

void Handler_ignore(int sig)
{
    //ignoruje sygnaly
   // cout << "Lider grupy wlasnie zignorowal sygnal\n";
}

int main(int argc,char* argv[])
{
    int pid = fork();
    fork();
    switch (pid)
    {
        case -1:
            perror("Blad funkcji fork!");
            exit(1);
        case 0:
            execlp(argv[1],argv[1],NULL);
            cout << "execlp nie dziala!\n";
            break;
        
        default:
        {  
            break;
        }
    }
    for (int i = 1; i < NSIG; i++)
    {
        signal(i,&Handler_ignore);
    }
    int status;
    for (int j = 0; j < 2; j++)
    {
            int child = wait(&status);
        if (child == -1) 
        {
            perror("Błąd funkcji wait");
            exit(1);
        }
        
        cout << "Proces o PID: " << child << " zakonczony\n";
        cout << "Czy zakonczony przez sygnal: ";
        if (WIFSIGNALED(status) != 0)
        {
            cout << "Tak\n";
            cout << "Numer sygnalu: " << WTERMSIG(status) <<" - " << strsignal(status) << endl;
        }
        else cout << "Nie\n"; 
    } 
    return 0;
}