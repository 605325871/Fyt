#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#define LS_NONE 0
#define LS_L 101
#define LS_R 102
#define LS_D 103
#define LS_I 104
 
#define LS_A 200
 
#define LS_AL (LS_A+LS_L)
#define LS_AI (LS_A+LS_I)

int analyzeParam(char* input){
    if(strlen(input)==2)
    {
        if(input[1]=='l') return LS_L;
        if(input[1]=='a') return LS_A;
        if(input[1]=='d') return LS_D;
        if(input[1]=='R') return LS_R;
        if(input[1]=='i') return LS_I;
    }
    else if(strlen(input)==3)
    {
        if(input[1]=='a'&& input[2]=='l') return LS_AL;
        if(input[1]=='a'&& input[2]=='i') return LS_AI;
    }
    return -1;
}


char* uid_to_name(uid_t uid)
{
    struct passwd* getpwuid(),* pw_ptr;
    static char numstr[10];
 
    if((pw_ptr = getpwuid(uid)) == NULL)
    {
        sprintf(numstr,"%d",uid);
 
        return numstr;
    }
    else
    {
        return pw_ptr->pw_name;
    }
}
 

char* gid_to_name(gid_t gid)
{
    struct group* getgrgid(),* grp_ptr;
    static char numstr[10];
 
    if(( grp_ptr = getgrgid(gid)) == NULL)
    {
        sprintf(numstr,"%d",gid);
        return numstr;
    }
    else
    {
        return grp_ptr->gr_name;
    }
}

 void show_file_info(char* filename, struct stat* info_p)
{
   
    char modestr[11];
    strcpy(modestr, "----------");
 
    if (S_ISDIR(info_p->st_mode))
        modestr[0] = 'd';
    if (S_ISCHR(info_p->st_mode))
        modestr[0] = 'c';
    if (S_ISBLK(info_p->st_mode))
        modestr[0] = 'b';
    if ((info_p->st_mode & S_IRUSR))
        modestr[1] = 'r';
     if ((info_p->st_mode& S_IWUSR))
        modestr[2] = 'w';
    if ((info_p->st_mode & S_IXUSR))
        modestr[3] = 'x';
    if ((info_p->st_mode & S_IRGRP))
        modestr[4] = 'r';
    if ((info_p->st_mode& S_IWGRP))
        modestr[5] = 'w';
    if ((info_p->st_mode& S_IXGRP))
        modestr[6] = 'x';
    if ((info_p->st_mode& S_IROTH))
        modestr[7] = 'r';
    if ((info_p->st_mode & S_IWOTH))
        modestr[8] = 'w';
    if ((info_p->st_mode& S_IXOTH))
        modestr[9] = 'x';
 
    printf("%s", modestr);
    printf(" %4d", (int) info_p->st_nlink);
    printf(" %-8s", uid_to_name(info_p->st_uid));
    printf(" %-8s", gid_to_name(info_p->st_gid));
    printf(" %8ld", (long) info_p->st_size);
    printf(" %.12s", 4 + ctime(&info_p->st_mtime));
    printf(" %s\n", filename);
}

void do_ls(char dirname[],int mode)
{
    DIR* dir_ptr;
    struct dirent* direntp;
 
    if ((dir_ptr = opendir(dirname)) == NULL)
    {
        fprintf(stderr, "ls2: cannot open %s \n", dirname);
    }
    else
    {
        
        
            char dirs[4096][100];
            int dir_count = 0;
            
            while ((direntp = readdir(dir_ptr)) != NULL)
            {
 
                if(mode < 200 && direntp->d_name[0]=='.')
                {
                    continue;
                }
 
                char complete_d_name[4000];  // 文件的完整路径
                if(dirname[2]!='s' && dirname[2]!='b'&&dirname[2]!='d')
                {
                strcpy (complete_d_name,dirname);
                strcat (complete_d_name,"/");
                strcat (complete_d_name,direntp->d_name);
                }
                struct stat info;
                if (stat(complete_d_name, &info) == -1)
                {
                    perror(complete_d_name);
                }
                else
                {
                    if(mode == LS_L||mode == LS_AL)
                    {
                        show_file_info(direntp->d_name, &info);
                    }
                    else if(mode == LS_A||mode == LS_NONE||mode == LS_I||mode == LS_AI)
                    {
                        if(mode == LS_I||mode == LS_AI)
                        {
                            printf("%ld ", direntp->d_ino);
                        }
 
                        printf("%-5s", direntp->d_name);
                    }
                    else if(mode == LS_R)
                    {

                        if(S_ISDIR(info.st_mode))
                        {
                            printf("%s\n", direntp->d_name);
 
                            strcpy (dirs[dir_count],complete_d_name);
                            dir_count++;
                        }
                        else
                        {
                            printf("%s\n", direntp->d_name);
                        }
                    }
 
                }
            }
 
            if(mode == LS_R)
            {
                int i=0;
                printf("\n");
                for(;i<dir_count;i++){
                    printf("%s:\n", dirs[i]);
                    do_ls(dirs[i],LS_R);
                }
            }
   printf("\n");
        closedir(dir_ptr);
    }
}

int main(int ac,char* av[])
{
 
    if(ac == 1)
    {
        do_ls(".",LS_NONE);
    }
    else
    {
        int mode = LS_NONE; // 默认为无参数ls
        int filepragm = 0; // 是否有输入文件参数
 
        while(ac>1)
        {
            ac--;
            av++;
 
            int calMode = analyzeParam(*av);
            if(calMode!=-1)
            {
                mode+=calMode;
            }
            else
            {
                filepragm = 1;
                do
                {
                    printf("%s:\n", *av);
                    do_ls(*av,mode);
                    printf("\n");
 
                    ac--;
                    av++;
                }while(ac>=1);
            }
        }
 
        if (!filepragm)
        {   
            do_ls(".",mode);
        }
        
    }     
}
