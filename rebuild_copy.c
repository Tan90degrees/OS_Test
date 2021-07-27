#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFERSIZE 4096            //BUFFER大小
int dir_or_file(char *address);    //判别文件还是文件夹
int dir_to_dir(char *address[]);   //文件夹到文件夹
int file_to_file(char *address[]); //文件到文件

int main(int argc, char *argv[])
{
    int F;
    if (argc != 3) //根据第一个输入参数判别该怎么处理
    {
        printf("you need to input right address!\n");
        return 0;
    }
    int flag = dir_or_file(argv[1]);
    if (flag == 1)
    {
        F = dir_to_dir(argv);
    }
    else if (flag == 2)
    {
        F = file_to_file(argv);
    }
    else
    {
        printf("Can't do this!\n");
        return 0;
    }
}

int dir_or_file(char *address) //判别文件还是文件夹
{
    struct stat s;
    if (stat(address, &s) == 0)
    {
        if (s.st_mode & S_IFDIR)
        {
            return 1;
        }
        else if (s.st_mode & S_IFREG)
        {
            return 2;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return -1;
    }
}

int dir_to_dir(char *address[]) //文件夹到文件夹
{
    DIR *dir1;
    DIR *dir2;
    struct dirent *currentdp;
    if ((dir1 = opendir(address[1])) == NULL)
    {
        printf("Can't open directory %s\n", address[1]);
        return -1;
    }
    if ((dir2 = opendir(address[2])) == NULL)
    {
        printf("Can't open directory %s\n", address[2]);
        return -1;
    }
    while ((currentdp = readdir(dir1)) != NULL)
    {
        char *sum1 = malloc(200 * sizeof(char));
        char *sum2 = malloc(200 * sizeof(char));
        char *st = currentdp->d_name;
        if (strcmp(st, ".") && strcmp(st, ".."))
        {
            strcpy(sum1, address[1]); //字符串处理，组合成文件路径形式
            strcat(sum1, st);
            strcpy(sum2, address[2]);
            strcat(sum2, st);
            char *myadd[3];
            myadd[0] = NULL;
            myadd[1] = sum1;
            myadd[2] = sum2;
            file_to_file(myadd); //调用文件到文件函数
        }
    }
}

int file_to_file(char *address[]) //文件到文件
{
    int f1, f2, r_data;
    DIR *dir;
    char buf[BUFFERSIZE];
    if ((f1 = open(address[1], O_RDONLY)) == -1)
    {
        printf("Can't open %s!\n", address[1]);
        return -1;
    }
    if ((f2 = open(address[2], O_WRONLY | O_CREAT | O_EXCL, 0777)) == -1) //判断后者地址是否为文件夹
    {
        printf("%s maybe a dir, so change the way!\n", address[2]);
        if ((dir = opendir(address[2])) == NULL)
        {
            printf("Can't open directory %s\n", address[2]);
            return -1;
        }
        else
        {
            char *myadd = malloc(200 * sizeof(char));
            char *madd = address[1];
            for (int d = strlen(address[1]); d >= 0; d--)
            {

                if (madd[d] == '/') //字符串处理，将文件夹路径变为文件路径
                {
                    int l = d;
                    for (int p = 0; p < strlen(address[1]) - l + 1; p++)
                    {
                        myadd[p] = madd[d + 1];
                        d++;
                    }
                    break;
                }
            }
            char *sum = malloc(200 * sizeof(char));
            strcpy(sum, address[2]); //字符串处理，组合成文件路径形式
            strcat(sum, myadd);
            if ((f2 = open(sum, O_WRONLY | O_CREAT | O_EXCL, 0777)) == -1)
            {
                printf("Can't open %s!\n", sum);
                return -1;
            }
            while ((r_data = read(f1, buf, BUFFERSIZE)) > 0)
            {
                if (write(f2, buf, r_data) != r_data)
                {
                    printf("Write error!\n");
                    return -1;
                }
                printf("success to copy %s to %s!\n", address[1], address[2]);
                return 0;
            }
        }
    }
    while ((r_data = read(f1, buf, BUFFERSIZE)) > 0) //复制文件
    {
        if (write(f2, buf, r_data) != r_data)
        {
            printf("Write error!\n");
            return -1;
        }
        printf("success to copy %s to %s!\n", address[1], address[2]);
        return 0;
    }
    printf("Can't open %s!\n", address[1]);
    return -1;
}
