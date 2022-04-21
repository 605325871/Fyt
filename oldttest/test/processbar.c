# include <stdio.h>
# include <unistd.h>
# include<string.h>
int main()
{
    char bar[101];
    const char *lable="|/-\\";
    memset(bar,'\0',sizeof(bar));
    int i=0;
    while(i <= 100)
    {
        printf("[%-100s][%d%%][%c]\r",bar,i,lable[i%4]);
        fflush(stdout);
        bar[i++]='#';
        usleep(10000);
    }
    printf("\n");
    
}