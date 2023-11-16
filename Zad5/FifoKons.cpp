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
using std::ofstream;

int main(int argc,char* argv[])
{
    ofstream destynacja(argv[2]);
    char odebrane[10];

    int filedesK = open(argv[1],O_RDONLY);
    if (filedesK == -1)
    {
        perror("Blad z otwieraniem potoku:");
        exit(1);
    }
    
    int bufor;
    sleep(1);
    while((bufor = read(filedesK,&odebrane,sizeof(odebrane))) > 0)
    {
        if( bufor == -1)
        {
            perror("Blad funkcji read()!");
            exit(1);
        }
        odebrane[bufor] = '\0';

        char bufor_str[10];
        sprintf(bufor_str,"%d",bufor);
        write(1, "Odebrano ", 9);
        write(1,bufor_str,strlen(bufor_str));
        write(1, " bajtow:\t\t", 9);
        write(1, odebrane, bufor); 
        write(1, "\n", 1);

         int i=0;
         while(odebrane[i] != '\0' && i < bufor) i++;
         if(odebrane[i] == '\0') bufor = i;
        destynacja.write(odebrane,bufor);
    }
    cout << "Konsument odczytal z potoku\n";

    destynacja.close();
    close(filedesK);
    return 0;
}