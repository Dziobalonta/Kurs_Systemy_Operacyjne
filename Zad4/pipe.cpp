#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

using std::ifstream;
using std::ofstream;
using std::cout;

int main(int argc, char* argv[])
{
    ofstream destynacja(argv[2]);
    int filedes[2];
    srand(time(NULL));
    // filedes[0] - read
    // filedes[1] - write
    if(pipe(filedes) == -1)
    {
        cout << "Blad otwierania potoku!\n";
        return 1;
    }
    int id = fork();
    switch (id)
    {
    case -1:
        perror("Blad funkcji fork!");
        exit(1);

    case 0:
        close(filedes[1]);
        char odebrane[10];
        int roz_buf;
        
        sleep(15);
        while ((roz_buf = read(filedes[0], &odebrane ,sizeof(odebrane))) > 0)
        {
            char buf[10];
            sprintf(buf,"%d",roz_buf);
            write(1, "Odebrano ", 9);
            write(1,buf,strlen(buf));
            write(1, " bajtow:\t\t", 9);
            write(1, odebrane, roz_buf); 
            write(1, "\n", 1);
            
            int i=0;
            while(odebrane[i] != '\0' && i < sizeof(odebrane)) i++;
            if(odebrane[i] == '\0') roz_buf = i;
            destynacja.write(odebrane,roz_buf);
            sleep(rand()%2);
        }
        cout << "Konsument odczytal z potoku\n\n";
        close(filedes[0]);
        break;
        
    default:
        char linijka[20];
        ifstream zrodlo(argv[1]);
        close(filedes[0]);
        while(!zrodlo.eof())
        {
            zrodlo.read(linijka,20);
            
            int wysl_buf=0;
            wysl_buf = write(filedes[1], &linijka, sizeof(linijka));
            char buf2[10];
            sprintf(buf2,"%d",wysl_buf);
            write(1, "Wyslano ", 8);
            write(1,buf2,strlen(buf2));
            write(1, " bajtow:\t\t", 9);
            write(1, linijka, wysl_buf); 
            write(1, "\n", 1);

            memset(linijka, 0 , sizeof(linijka));
            sleep(rand()%3);
        }
        cout << "Producent wpisal cos do potoku\n\n";
        close(filedes[1]);
        zrodlo.close();
        break;
    }
    int status;
    waitpid(id,&status,0);
    destynacja.close();


    return 0;
}