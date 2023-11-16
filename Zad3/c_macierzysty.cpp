#include <iostream>
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
        {
            cout << "\nPID: " << getpid() << " PPID: " << getppid() <<" PGID: " << getpgrp();
            setpgid(getpid(),getpid());
            cout <<" --> PGID: " << getpgrp() << endl;
            execlp(argv[1],argv[1],argv[2],NULL);
            break;
           
        }
        default:
        {   
            kill(getpgid(pid),0);
            if (errno == ESRCH) perror("Proces potonmy nie istnieje!");
            else
            {
                sleep(1);
                cout << getpid() << " wysyła do PGIDs: " << getpgid(pid) << endl;
                killpg(getpgid(pid),SIGQUIT);
                sleep(1); 
                killpg(getpgid(pid),SIGTSTP);
                sleep(1);
                killpg(getpgid(pid),SIGINT);
            }
            int status;
            int child = wait(&status);
            cout << "Proces o PID: " << child << " zakonczony\n";
            cout << "Czy zakonczony przez sygnal: ";
            if (WIFSIGNALED(status) != 0)
            {
                cout << "Tak\n";
                cout << "Numer sygnalu: " << WTERMSIG(status) <<" - " << strsignal(status) << endl;
            }
            else cout << "Nie\n";
            break;
        }
    }
    return 0;
}