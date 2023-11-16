#include <iostream>
#include <csignal>

using std::cout;

void Handle_SIGUSR1(int sig)
{
    cout << "Brawo! Wlasnie przechwyciles sygnal SIGUSR1 programem potomnym!\n";
}

int main()
{
    signal(SIGUSR1,&Handle_SIGUSR1);
    cout << "Program potomny czeka na sygnal\n";
    pause();
    pause();
    return 0;
}