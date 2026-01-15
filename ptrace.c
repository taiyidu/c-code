#include<stdio.h>
#include<sys/ptrace.h>
#include<unistd.h>
int main()
{
    printf("ptrace\n");
    int pid=2853;
    long address=0xA301550C;

    printf("please input pid=>");
    scanf("%d",&pid);
    printf("please input address=>");
    scanf("%lx",&address);//lx is fro reading a sixteen-digit adress

    long ret=ptrace(PTRACE_ATTACH,pid,NULL,NULL);//strat debug 附加进程
    printf("PTRACE_ATTACH:%d\n",ret);

   
    //write memory
    int data=0;
    printf("please input new data=>");
    scanf("%d",&data);
    ptrace(PTRACE_POKEDATA,pid,address,data);
    sleep(1);

     //read memory
    ret=ptrace(PTRACE_PEEKDATA,pid,address,NULL);//strat debug 读内存
    printf("PTRACE_PEEKDATA:%d\n",ret);

    if(ret!=0)
    {
        printf("PTRACE_PEEKDATA ERROR");//查看是否附加成功并读取值
        return 1;
    }
    sleep(1);


    ret=ptrace(PTRACE_DETACH,pid,NULL,NULL);//end debug  分离进程
    printf("PTRACE_DETACH:%d\n",ret);
    printf("\n");
    return 0;
}