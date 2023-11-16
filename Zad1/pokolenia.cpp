#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int mthrpid = getpid(), count=0;
    std::cout << "(PID,PPID,GPID)\n";
    std::cout << "( " << getpid() << " , " << getppid() << " , " << getpgrp() << " ) - Proces 0\n";
    for (int i = 0; i < 3; i++)
    {
        int id = fork();
        switch (id)
        {
            case -1:
                perror("Blad funkcji fork!");
                exit(1);
            case 0:
                std::cout << "( " << getpid() << " , " << getppid() << " , " << getpgrp() << " )";
                std::cout << " - Proces " << i+1 << "\n";
                sleep(1);
                break;
            default:
                sleep(1);
                break;
        } 
    }
    return 0;
}