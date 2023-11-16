#include <iostream>
#include <csignal>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

using std::cout;
using std::endl;

int main(int argc,char* argv[])
{
    int pid = fork();
    switch (pid)
    {
    case -1:
        perror("Blad funkcji fork!");
        exit(1);
    case 0:
        execlp(argv[1],argv[1],NULL);
        perror("Blad funkcji execlp!");
        break;
    default:
        kill(pid,0);
        if (errno == ESRCH) perror("Proces potonmy nie istnieje!");
        else
        {
            sleep(1);
            kill(pid,SIGUSR1);
            sleep(1);    
            kill(pid,SIGTERM);
            int status;
            cout << "Proces o PID: " << wait(&status) << " zakonczony\n";
            cout << "Czy zakonczony przez sygnal: ";
            if (WIFSIGNALED(status) != 0)
            {
                cout << "Tak\n";
                cout << "Numer sygnalu: " << WTERMSIG(status) <<" - " << strsignal(status) << endl;
            }
            else cout << "Nie\n";
        }
       

        break;
    }
    return 0;
}