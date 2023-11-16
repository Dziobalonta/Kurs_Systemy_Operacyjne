#include <iostream>
#include <csignal>
#include <sys/types.h>
#include <unistd.h>

using std::cout;

void Handle_CTRL_C(int sig)
{
    cout << "Nie mozna przerwac programu!\n";
}

int main()
{
    signal(SIGQUIT,SIG_IGN);                // Ctrl + "\"
    signal(SIGTSTP,SIG_DFL);                // Ctrl + Z
    signal(SIGINT,&Handle_CTRL_C);          // Ctrl + C
    cout << "Program czeka na sygnal\n";
    pause();
    sleep(5);
    kill(getpid(),SIGKILL);
    return 0;
}