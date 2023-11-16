#include <iostream>
#include <csignal>
#include <sys/types.h>
#include <unistd.h>

using std::cout;
using std::endl;

void Handle_CTRL_C(int sig)
{
    cout << "Nie mozna wykonac operacji Ctrl+C!\n";
}

void Handle_CTRL_BS(int sig)
{
    cout << "Nie mozna wykonac operacji Ctrl+BS!\n";
}

void Handle_CTRL_Z(int sig)
{
    cout << "Nie mozna wykonac operacji Ctrl+Z!\n";
}

int main()
{
    signal(SIGQUIT,&Handle_CTRL_BS);          // Ctrl + "\"
    signal(SIGTSTP,&Handle_CTRL_Z);          // Ctrl + Z
    signal(SIGINT,&Handle_CTRL_C);          // Ctrl + C
    cout << "Proces z zadania a) czeka na sygnal";
    cout << " - PID: " << getpid() << " PPID: " << getppid() << " PGID: "<< getpgrp() << endl;
    pause();
    pause();
    pause();
    sleep(5);
    kill(getpid(),SIGKILL);
    return 0;
}