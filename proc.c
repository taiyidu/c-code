#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdint.h>

int main()
{
    printf("proc\n");
    char* mem_path="/proc/2864/mem";
    int fd =open(mem_path,O_RDWR);//read and write

    
    //read memory
    uintptr_t address=0xB0D19E14;
    int res;
    lseek(fd,address,SEEK_SET);//更改指针位置
    read(fd,&res,sizeof(int));
    printf("%lx read int %d\n",address,res);

    lseek(fd,address,SEEK_SET);//更改指针位置
    int buf[1]={999};
    write(fd,buf,sizeof(int));
    printf("%lx write int %d\n",address,res);

    
    close(fd);
    return 0;
}