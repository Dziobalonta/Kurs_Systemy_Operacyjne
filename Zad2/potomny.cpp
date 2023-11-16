#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[])
{
    std::cout << "Proces potomny nr."<< argv[1] << ": " << std::endl;
    std::cout << "UID: " << getuid() << std::endl;
    std::cout << "GID: " << getgid() << std::endl;
    std::cout << "PID: " << getpid() << std::endl;
    std::cout << "PPID: " << getppid() << std::endl;
    std::cout << "PGID: " << getpgrp() << std::endl;
    return 0;
}