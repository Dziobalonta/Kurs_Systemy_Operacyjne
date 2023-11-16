#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <cstring>

using std::cout;
using std::endl;
using std::ifstream;

int main(int argc,char* argv[])
{
    ifstream wejscie(argv[2]);
    char linijka[20];

    int filedesP = open(argv[1],O_WRONLY);
    if (filedesP == -1)
    {
        perror("Blad z otwieraniem potoku:");
        exit(1);
    }

    while (!wejscie.eof())
    {
        wejscie.read(linijka,20);
        int bufor = write(filedesP,&linijka,sizeof(linijka));
        if(bufor == -1)
        {
            perror("Blad funkcji write()!");
            exit(1);
        }
        char buf_str[3];
        sprintf(buf_str,"%d",bufor);
        write(1, "Wyslano ", 8);
        write(1,buf_str,strlen(buf_str));
        write(1, " bajtow:\t\t", 9);
        write(1, linijka, strlen(linijka)); 
        write(1, "\n", 1);

        memset(linijka, 0 , sizeof(linijka));

    }
    cout << "Producent wpisal cos do potoku\n";
    wejscie.close();
    close(filedesP);
    return 0;
}