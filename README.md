# ttds_example
An "operating system" writen in C, which can port to any C compiler which longjmp is supported.
使用longjmp实现的一个“操作系统”，也许叫协程更合适一点。
# 特性
* 方便移植，任意支持setjmp/longjmp的C编译器都应该支持(目前已经MSVC-64位不支持，似乎是因为64bit模式下结栈的完整性有检查，所以无法支持.)
* 支持信号量/条件变量
* 支持互拆量
* 不关中断。所以，可以认为中断响应很快
* 无优先级，排队执行. 以协程为目的，个人认为不需要优先级
* 单文件实现，还是需要定义函数body的，通过定义DEF_MYPT_C_FUNCTION_BODY后包含
* 线程池API  这是受实现原理所限制，目前想到最好的方案是线程池.
* 
# 命名空间
这里是指标识符的前缀。
程序里反复修改了多次命名空间，最终决定使用输入方便的ttds作为“命名空间”，即所以标识符前都加ttds，因此，并无实际含义。

```c++

#define DEF_MYPT_C_FUNCTION_BODY
#include "ttds_cotask.h"


int stack_used(){
    ttds_cothread_t *ptr = ttds_cothread_self();
    return ( 100 * ( ptr->stack_high - ptr->stack_check ) )/(ptr->stack_high-ptr->stack_low);
}

int entry_a( void *p )
{
    printf("thread a started.\n");
    while (1)
    {
        printf("thread A run. stack:%d%%\n", stack_used() );
        ttds_cothread_delay(1000+ rand()%200 );
    }
}


int entry_b( void *p )
{
    while (1)
    {
        printf("thread B run. stack used:%d. stack:%d%%\n", ttds_cothread_get_stack_used(NULL), stack_used() );
        ttds_cothread_delay(1000+ rand()%200 );
    }
}

int idle_thread( void *p )
{
    while (1)
    {
        ttds_cothread_yeild();
    }
}


int main(){
    static ttds_cothread_t pool[6];

    ttds_threadpool_init( 4096 );
    ttds_threadpool_add_thread( pool, 6, 4096);

    ttds_threadpool_run( idle_thread, 0, 1024, "TaskIdle" );
    ttds_threadpool_run( entry_a, 0, 2000, "TaskA" );
    ttds_threadpool_run( entry_b, 0, 2000, "TaskB" );

    ttds_threadpool_startup();
    
    while (1){
        printf("main thread running\n");
        ttds_cothread_delay( 2000 );
    }
}

```
