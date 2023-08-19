# ttds_example
An "operating system" writen in C, which can port to any C compiler which longjmp is supported.

```c++

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
