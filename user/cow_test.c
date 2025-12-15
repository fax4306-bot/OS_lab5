#include <stdio.h>
#include <ulib.h>
#include <unistd.h>

int some_global_variable = 42;

int main(void) {
    int *shared_mem = (int *)0x10000000;
    *shared_mem = 12345;
    cprintf("父进程：原始值 = %d\n", *shared_mem);

    some_global_variable=7;

    int pid = fork();

    if (pid == 0) {
        cprintf("子进程：读取共享值 = %d\n", *shared_mem);
        cprintf("子进程：正在写入新值 54321...\n");
        *shared_mem = 54321;
        cprintf("子进程：我的新值 = %d\n", *shared_mem);
        exit(0);
    } else if (pid > 0) {
        waitpid(pid, NULL);
        cprintf("父进程：子进程退出后，我的值仍然是 = %d\n", *shared_mem);
        if (*shared_mem == 12345) {
            cprintf("COW 测试通过！\n");
        } else {
            cprintf("COW 测试失败！\n");
        }
    } else {
        cprintf("fork 失败。\n");
    }
    return 0;
}
