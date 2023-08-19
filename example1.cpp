

#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>

#define DEF_MYPT_C_FUNCTION_BODY
#include "ttds_cotask.h"

ttds_cosem_t g_sem;

int stack_used(){
    ttds_cothread_t *ptr = ttds_cothread_self();
    return ( 100 * ( ptr->stack_high - ptr->stack_check ) )/(ptr->stack_high-ptr->stack_low);
}


int entry_a( void *p )
{
    int i;

    ttds_cothread_self()->name = "TaskA";

    printf("thread a started.\n");
    while (1)
    {
        printf("thread A run. stack:%d%%\n", stack_used() );
        ttds_cothread_delay(1000+ rand()%200 );
        ttds_cosem_give( &g_sem, 1 );
    }
}

int entry_c(void* p);
int entry_d(void* p);
int entry_b( void *p )
{
    ttds_cothread_self()->name = "TaskB";
    printf("thread b started. %p=\n", ttds_cothread_self() );
    ttds_threadpool_run( entry_c, 0, 2000, "TaskC");
    ttds_threadpool_run( entry_d, 0, 2000, "TaskD" );
    while (1)
    {
        printf("thread B run. stack used:%d. stack:%d%%\n", ttds_cothread_get_stack_used(NULL), stack_used() );
        ttds_cothread_delay(1000+ rand()%200 );
    }
}

int entry_c( void *p )
{
    int a = 0;
    ttds_cothread_self()->name = "TaskC";
    printf("thread c started.\n");
    while (1)
    {
        ttds_coerr_t ok = ttds_cosem_take( &g_sem, 300+rand()%100 );
        if (ok == ttds_coEOK){
            a = 1;
            printf("thread C run. stack used:%d take sem ok . stack:%d%%\n", ttds_cothread_get_stack_used(NULL), stack_used() );
        } else{
            a = 2;
            printf("thread C run. stack used:%d take sem TIMEOUT %d. stack:%d%%\n", ttds_cothread_get_stack_used(NULL), (int)ok, stack_used() );
        }
    }
}

volatile int g_thread_d_exit = 0;

int entry_d(void* p)
{
    int a = 0;
    g_thread_d_exit = 1;
    ttds_cothread_self()->name = "TaskD";
    //printf("thread d started.\n");
    for (a = 0; a<10; a++)
    {
        ttds_coerr_t ok = ttds_cosem_take(&g_sem, 300 + rand() % 100);
        if (ok == ttds_coEOK) {
            a = 1;
            //printf("thread D run. stack used:%d take sem ok . stack:%d%%\n", ttds_threadpool_stack_used(), stack_used());
        }
        else {
            a = 2;
            //printf("thread D run. stack used:%d take sem TIMEOUT %d. stack:%d%%\n", ttds_threadpool_stack_used(), (int)ok, stack_used());
        }
    }
    g_thread_d_exit++;
    return 0;
}

int idle_thread( void *p )
{
    ttds_cothread_self()->name = "TaskIdle";
    while (1)
    {
        int tick_ms;
        ttds_cothread_yeild();
        tick_ms = ttds_idle_get_sleep_time();
        if (tick_ms>0){
            /* power saving for tick_ms */
            //printf("    ----- sleep for %dms -----\n", tick_ms );
            std::this_thread::sleep_for( std::chrono::milliseconds(tick_ms) );
        }
    }
}


static void ticker_init();

int main(int argc, char **argv)
{
    ticker_init();

    static ttds_cothread_t pool[6];

    ttds_threadpool_init( 4096 );
    ttds_threadpool_add_thread( pool, 6, 4096);

    ttds_cosem_init( &g_sem, 0 );

    ttds_threadpool_run( idle_thread, 0, 1024, "TaskIdle" );
    ttds_threadpool_run( entry_a, 0, 2000, "TaskA" );
    ttds_threadpool_run( entry_b, 0, 2000, "TaskB" );

    ttds_threadpool_startup();
    ttds_threadpool_default()->enable_stack_check = 0;
    while (1){
        #if 1
        printf("main thread running\n");
        printf("\n\n*** STACK INFO ***\n");
        printf("%25s: %8d   /   %8d  %8d\n", "main", ttds_cothread_get_stack_used(&ttds_threadpool_default()->main), ttds_threadpool_default()->main.stack_size,  ttds_threadpool_default()->main.stack_high-ttds_threadpool_default()->main.stack_low );
        for (int i=0; i<sizeof(pool)/sizeof(pool[0]); i++){
            printf("%25s: %8d   /   %8d  %8d\n", pool[i].name, ttds_cothread_get_stack_used(pool+i), pool[i].stack_size,  pool[i].stack_high-pool[i].stack_low );
        }
        #endif
        ttds_cothread_delay( 2000 );
        if (g_thread_d_exit == 2){
            ttds_threadpool_run(entry_d, 0, 2000, "TaskD");
        }
    }

    return 0;
}

std::chrono::time_point<std::chrono::steady_clock> g_tp_last;

static void ticker_init()
{
    auto now = std::chrono::steady_clock::now();
    g_tp_last = now;
}

int32_t g_ms_offset = 0;
int32_t ttds_cothread_sys_tick_get_ms()
{
    auto now = std::chrono::steady_clock::now();
    auto diff_ms = std::chrono::duration_cast<std::chrono::milliseconds>( now - g_tp_last ).count();
    return (int32_t)diff_ms;
}
