# ttds_example
An "operating system" written in C, which can port to any C compiler which longjmp is supported.
使用longjmp实现的一个“操作系统”，也许叫协程更合适一点。

# 原理
使用setjmp/longjmp实现
* 主要思想
先使用setjmp设置锚点，锚点一条路径作为线程执行入口，另一条路径用来扩张栈空间。扩张方法是，使用递归扩张，扩张够了指定栈大小后，设置一个栈底锚点，栈底锚点用于扩张其他线程的栈.
这个过程中，栈是不可回收的，或者说不方便回收，那么做成线程永远不退出应该是一个好方法，于是便有了线程池的API。
* 栈溢出检查是重要的，而且也是必要的。初始化时，需要对栈空间写入固定值 0x55，然后在切换线程时去检查使用了多少。
# 假设
对栈有特殊操作，所以需要满足这些假设，程序的运行才是正确的。
* 把函数局部变量Var取地址并传入递归函数后，变量Var必定在栈上分配，而不是寄存器变量。
* 同一个函数递归调用栈帧大小相同。
* longjmp过程不修改无关的栈。
* 对栈帧以后的内存可以写操作(栈溢出检查需要)。
* setjmp保存了所有寄存器，longjmp恢复了所有寄存器。

# 特性
* 便于移植。本项目基于 setjmp/longjmp 指令，编译器普遍支持。
* 支持信号量。
* 支持互斥量。
* 无需关闭中断，不影响中断的响应。
* 仅单文件。

# 目前的问题：
 * MSVC-64 对于 setjmp/longjmp 指令的支持不佳。
 * linux-gcc longjmp causes uninitialized stack frame
### gcc longjmp causes uninitialized stack frame
在某些发行版上（ubuntu8.10以上，centOS貌似还没这个feature），gcc带优化的情况下，会自动带上FORTIFY_SOURCE编译，这样longjmp在运行时会crash，报错为“longjmp causes uninitialized stack frame”，修复办法为为关闭gcc的FORTIFY_SOURCE特性，增加编译参数“-U_FORTIFY_SOURCE”
或者加上“-D_FORTIFY_SOURCE=0”解决.

```
-D_FORTIFY_SOURCE=0
```


# 命名空间
这里是指标识符的前缀。
程序里反复修改了多次命名空间，最终决定使用输入方便的ttds作为“命名空间”，即所以标识符前都加ttds，因此，并无实际含义。

# how to use

### 修改主线程的栈大小
以msvc为例子，默认大小是1MB，要修改栈大小，可以使用参数"/stack"修改，如改为5MB的连接参数为：
* MSVC win32
```
/stack:0x500000
```
* gcc for pc
```
-Wl,--stack=0x500000
```

### 定义一次函数体，通过定义宏DEF_MYPT_C_FUNCTION_BODY来实现。
```
#define DEF_MYPT_C_FUNCTION_BODY
#include "ttds_cotask.h"
```

### 实现ttds_cothread_sys_tick_get_ms()
需要实现毫秒计时函数ttds_cothread_sys_tick_get_ms
```
int32_t ttds_cothread_sys_tick_get_ms(void);
```

### 线程池初始化
参数main_thread_stack_size是main线程的栈保留字节数。
```c
void ttds_threadpool_init(int main_thread_stack_size );
```
### 加入指定栈大小的线程对象
在使用ttds_threadpool_run()之前，需要先加入线程对象。
注意：线程对象栈大小不可更改。
```
void ttds_threadpool_add_thread(ttds_cothread_t *p_threads, int count,int stack_size );
```
### 启动线程池
注意，这函数会返回，返回后，运行环境是"main"线程.
```
void ttds_threadpool_startup(void);
```


### 使用标准C++环境的完整例子
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


static void ticker_init();
int main(){
    ticker_init();
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

#include <chrono>
std::chrono::time_point<std::chrono::steady_clock> g_tp_last;
static void ticker_init()
{
    auto now = std::chrono::steady_clock::now();
    g_tp_last = now;
}
int32_t ttds_cothread_sys_tick_get_ms()
{
    auto now = std::chrono::steady_clock::now();
    auto diff_ms = std::chrono::duration_cast<std::chrono::milliseconds>( now - g_tp_last ).count();
    return (int32_t)diff_ms;
}
```
