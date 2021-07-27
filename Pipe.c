#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main()
{
    pid_t p1, p2;
    int fd[2];
    char child1[] = "Child process 1 is sending a message!";
    char child2[] = "Child process 2 is sending a message!";
    int len1 = strlen(child1);
    int len2 = strlen(child2);

    if (pipe(fd) == -1)
    {
        printf("error");
        return 1; //如果没有管道直接输出错误
    }

    if ((p1 = fork()) == -1)
    { //创建子进程1
        printf("failed to create child1\n");
    }
    else if (p1 == 0)
    {
        close(fd[0]); //关闭读端口
        sleep(1);
        write(fd[1], child1, strlen(child1) + 1);
        fprintf(stderr, "[%ld]:child1 write:{%.*s}\n", (long)getpid(), len1, child1); //向管道写数据
        exit(0);
    }

    if ((p2 = fork()) == -1)
    {
        printf("failed to create child2\n"); //创建子进程2
    }
    else if (p2 == 0)
    {
        close(fd[0]); //关闭读端口
        sleep(1);
        write(fd[1], child2, strlen(child2) + 1);
        fprintf(stderr, "[%ld]:child2 write:{%.*s}\n", (long)getpid(), len2, child2); //向管道写数据
        exit(0);
    }

    close(fd[1]);                                   //关闭管道写端
    len1 = read(fd[0], child1, strlen(child1) + 1); //读取数据
    len2 = read(fd[0], child2, strlen(child2) + 1); //读取数据
    fprintf(stderr, "[%ld]:father read child1:{%.*s}\n [%ld]:father read child2:{%.*s}\n", (long)getpid(), len1, child1, (long)getpid(), len2, child2);
    //打印出结果
    return 0;
}
