# ttds_example
An "operating system" writen in C, which can port to any C compiler which longjmp is supported.

```c++
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
