#include <iostream>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    uid_t uid = getuid(); //identyfikator uzytkownika 
    gid_t gid = getgid(); //identyfikator grupy uzytkownika
    pid_t pid = getpid(); //identyfikator procesu
    pid_t ppid = getppid(); //identyfikator procesu macierzystego (przodka)
    pid_t pgid = getpgid(pid); //identyfikator grupy procesow (=PID lidera grupy)
    

    std::cout << "UID: " << uid << std::endl;
    std::cout << "GID: " << gid << std::endl;
    std::cout << "PID: " << pid << std::endl;
    std::cout << "PPID: " << ppid << std::endl;
    std::cout << "PGID: " << pgid << std::endl;
    return 0;
}
