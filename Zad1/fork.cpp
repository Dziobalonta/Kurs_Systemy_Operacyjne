#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = 0;
    std::cout << "Proces Macierzysty: "<< std::endl;
    std::cout << "UID: " << getuid() << std::endl;
    std::cout << "GID: " << getgid() << std::endl;
    std::cout << "PID: " << getpid() << std::endl;
    std::cout << "PPID: " << getppid() << std::endl;
    std::cout << "PGID: " << getpgrp() << std::endl;
    for (int i = 0; i < 3; i++)
    {
        switch (fork())
        {
            case -1:
                perror("Blad funkcji fork!");
                exit(1);
            case 0:
                std::cout << "Proces potomny nr."<< i+1 << ": " << std::endl;
                std::cout << "UID: " << getuid() << std::endl;
                std::cout << "GID: " << getgid() << std::endl;
                std::cout << "PID: " << getpid() << std::endl;
                std::cout << "PPID: " << getppid() << std::endl;
                std::cout << "PGID: " << getpgrp() << std::endl;
                break;
            default:
                pid = wait(NULL);
                std::cout << "Proces o PID " <<  pid << " zakonczony!" << std::endl;
                break;
        }
    }
    return 0;
}

