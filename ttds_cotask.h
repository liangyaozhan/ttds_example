/*
 * File: ttds_cotask.h
 * Author: Liang YaoZhan <262666882@qq.com>
 * Brief:  co-routine implemnted through setjmp
 *
 * Copyright (c) 2022 - 2023 Liang YaoZhan <262666882@qq.com>
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2023-08-19 Liang YaoZhan <262666882@qq.com> created
 *
 */

/*
 * File: ttds_queue.h
 * Author: Liang YaoZhan <262666882@qq.com>
 * Brief:  timer
 *
 * Copyright (c) 2022 - 2023 Liang YaoZhan <262666882@qq.com>
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2023-08-05 Liang YaoZhan <262666882@qq.com> created
 *
 */

#ifndef ttds_QUEUE_H
#define ttds_QUEUE_H

#ifndef ttds_assert 
#define ttds_assert(x)
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct ttds_queue_s ttds_queue_t;

    struct ttds_queue_s
    {
        ttds_queue_t *prev;
        ttds_queue_t *next;
    };

#define ttds_queue_init(q) \
    do                      \
    {                       \
        (q)->prev = q;      \
        (q)->next = q;      \
    } while (0)

#define ttds_queue_empty(h) \
    (h == (h)->prev)

#define ttds_queue_insert_head(h, x) \
    do                                \
    {                                 \
        (x)->next = (h)->next;        \
        (x)->next->prev = x;          \
        (x)->prev = h;                \
        (h)->next = x;                \
    } while (0)

#define ttds_queue_insert_after ttds_queue_insert_head

#define ttds_queue_insert_tail(h, x)                                                     \
    do                                                                                    \
    {                                                                                     \
        ttds_assert((x)->next == (x)->prev && (x)->next == (x) && "insert twice or not init"); \
        (x)->prev = (h)->prev;                                                            \
        (x)->prev->next = x;                                                              \
        (x)->next = h;                                                                    \
        (h)->prev = x;                                                                    \
    } while (0)

#define ttds_queue_head(h) \
    (h)->next

#define ttds_queue_last(h) \
    (h)->prev

#define ttds_queue_next(q) \
    (q)->next

#define ttds_queue_prev(q) \
    (q)->prev

#define ttds_queue_remove(x)              \
    do                                     \
    {                                      \
        ttds_queue_t *p_prev = (x)->prev; \
        ttds_queue_t *p_next = (x)->next; \
        p_next->prev = p_prev;             \
        p_prev->next = p_next;             \
        ttds_queue_init(x);               \
    } while (0)

/**
 * origin:      h ==A=== q  ====B=== h
 * after list 1:      h ==A=== h
 * after list 2:      n q==B=== n
 */
#define ttds_queue_split(h, q, n) \
    (n)->prev = (h)->prev;         \
    (n)->prev->next = n;           \
    (n)->next = q;                 \
    (h)->prev = (q)->prev;         \
    (h)->prev->next = h;           \
    (q)->prev = n;

#define ttds_queue_add(h, n)    \
    (h)->prev->next = (n)->next; \
    (n)->next->prev = (h)->prev; \
    (h)->prev = (n)->prev;       \
    (h)->prev->next = h;

#define ttds_queue_move(h, _new)         \
    do                                    \
    {                                     \
        ttds_queue_insert_tail(h, _new); \
        ttds_queue_remove(h);            \
    } while (0)

#ifndef ttds_container_of
#define ttds_container_of(p, t, m) \
    ((t *)((char *)p - (char *)(&(((t *)0)->m))))
#endif

#define ttds_queue_container(q, type, m) ttds_container_of((q), type, m)

#define ttds_queue_for_each_container(pos, head, t, member)   \
    for (pos = ttds_queue_container((head)->next, t, member); \
         &pos->member != (head);                               \
         pos = ttds_queue_container(pos->member.next, t, member))

#define ttds_queue_for_each_container_safe(pos, n, head, t, member) \
    for (pos = ttds_queue_container((head)->next, t, member),       \
        n = ttds_queue_container(pos->member.next, t, member);      \
         &pos->member != (head);                                     \
         pos = n, n = ttds_queue_container(n->member.next, t, member))

#ifdef __cplusplus
}
#endif

#endif
/*
 * File: ttds_timer.h
 * Author: Liang YaoZhan <262666882@qq.com>
 * Brief:  timer
 *
 * Copyright (c) 2022 - 2023 Liang YaoZhan <262666882@qq.com>
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2023-08-05 Liang YaoZhan <262666882@qq.com> created
 *
 */

#ifndef ttds_TIMER_H
#define ttds_TIMER_H


#ifdef __cplusplus
extern "C"
{
#endif

typedef struct ttds_timer_s ttds_timer_t;
struct ttds_tick_s;
typedef struct ttds_tick_s ttds_tick_t;

struct ttds_timer_s
{
/* private: */
    ttds_queue_t       node;
    unsigned int      tick_origin;
    int               tick;
    ttds_tick_t     *p_ticker;

/* public: */
    void (*timeout_callback)( ttds_timer_t *p_this, void *arg); /*!< timeout callback function */
    void             *arg;
};

void ttds_timer_init( ttds_timer_t *p_this, void (*cb)(ttds_timer_t *p_this, void*arg), void *arg);
void ttds_timer_destroy( ttds_timer_t *p_this );
void ttds_timer_restart(  ttds_timer_t *p_timer );

struct ttds_tick_s
{
    ttds_queue_t head;
};

void ttds_tick_init(  ttds_tick_t *p_this);
void ttds_tick_increase(  ttds_tick_t *p_this, int tick );
void ttds_tick_destroy(  ttds_tick_t *p_this);

void ttds_tick_add_timer(  ttds_tick_t *p_this, ttds_timer_t *p_timer, int tick);
void ttds_tick_remove_timer(  ttds_tick_t *p_this, ttds_timer_t *p_timer);
int  ttds_tick_next(ttds_tick_t *p_this);

#ifdef __cplusplus
}
#endif


#endif

#ifdef __cplusplus
extern "C"
{
#endif
/*
 * File: ttds_cocore.h
 * Author: Liang YaoZhan <262666882@qq.com>
 * Brief:  co-routine implemnted through setjmp
 *
 * Copyright (c) 2022 - 2023 Liang YaoZhan <262666882@qq.com>
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2023-08-05 Liang YaoZhan <262666882@qq.com> created
 *
 */

#ifndef ttds_coTASK_H
#define ttds_coTASK_H

#include <stdint.h>
#include <setjmp.h>


#define ttds_coCPLUSPLUS_BEGIN \
    extern "C"                \
    {
#define ttds_coCPLUSPLUS_END }

#ifdef __cplusplus
ttds_coCPLUSPLUS_BEGIN
#endif

/************************************************** ttds_cothread_t ***************************************/
/************************************************** ttds_cothread_t ***************************************/

#define ttds_assert(c) do{ ;}while(0)

#define ttds_coWAITING 0
#define ttds_coYIELDED 1
#define ttds_coEXITED 2
#define ttds_coPAUSE 3
#define ttds_coENDED 4

typedef char ***base_reg_t;

typedef enum _mypt_state
{
    ttds_coSTATE_INIT, /* */
    ttds_coSTATE_READY,
    ttds_coSTATE_PENDING,
    ttds_coSTATE_DEAD,
} ttds_costate_t;
typedef enum _mypt_err_s
{
    ttds_coEOK,
    ttds_coEEMPTY,
    ttds_coETERMINATED,
    ttds_coETIMEOUT,
} ttds_coerr_t;

struct _ttds_threadpool;
typedef struct _ttds_threadpool ttds_threadpool_t;
typedef struct _ttds_thread
{
/* private: */
    int stack_size;
    ttds_queue_t node_state;
    ttds_queue_t node_pending_resource;
    ttds_timer_t timer;
    ttds_coerr_t err;
    ttds_costate_t state;
    jmp_buf jb;

/*public:*/
    const char *name;
    char *stack_low;
    char *stack_check;
    char *stack_high;
    int exit_code;
    int (*entry)(void *arg);
    void *arg;
} ttds_cothread_t;
#define ttds_coOBJECT(obj) ttds_cothread_t obj

struct _ttds_sem
{
    int count;
    ttds_queue_t head_pendings;
};
typedef struct _ttds_sem ttds_cosem_t;
struct _ttds_mutex
{
    ttds_cothread_t *p_owner;
    ttds_queue_t head_pendings;
    uint16_t recursive;
    uint8_t is_recursive;
};
typedef struct _ttds_mutex ttds_comutex_t;

struct _ttds_condition_variable
{
    ttds_cosem_t sem;
};
typedef struct _ttds_condition_variable ttds_condition_variable_t;

struct _ttds_threadpool
{
/* private: */
    ttds_cothread_t *p_thread_current;
    ttds_queue_t head_state_pending;
    ttds_queue_t head_state_running;
    ttds_tick_t ticker;
    int32_t systick;
    jmp_buf jb_create_thread_req;
    jmp_buf jb_create_thread_res;
    ttds_cothread_t *p_thread_to_init_stack;
    char *p_stack_start;
    char *p_stack_current;
    uint8_t enable_stack_check;
    uint8_t is_stack_going_down;

/*public:*/
    ttds_cothread_t main;
};


#define ttds_cosem(sem) ttds_cosem_t sem

/* you should define this function before using ttds_coDELAY */
int32_t ttds_cothread_sys_tick_get_ms();

void ttds_cosem_init(ttds_cosem_t *p_this, int init_count);
ttds_coerr_t ttds_cosem_take(ttds_cosem_t *p_this, int delay_tick); /* delay_tick: -1 forevery */
void ttds_cosem_give(ttds_cosem_t *p_this, int count);
void ttds_cosem_give_all(ttds_cosem_t *p_this);
void ttds_cosem_destroy(ttds_cosem_t *p_this);

void ttds_comutex_init(ttds_comutex_t *p_this, int is_recursive);
ttds_coerr_t ttds_comutex_lock( ttds_comutex_t *p_this, int delay_tick ); /* delay_tick: -1 forevery */
void ttds_comutex_unlock( ttds_comutex_t *p_this );
void ttds_comutex_destroy( ttds_comutex_t *p_this );

void ttds_cothread_delay(int tick);
int ttds_cothread_get_stack_used(ttds_cothread_t *ptr );
void ttds_cothread_yeild();
ttds_cothread_t *ttds_cothread_self();

void ttds_threadpool_init(int main_thread_stack_size );
void ttds_threadpool_add_thread(ttds_cothread_t *p_threads, int count,int main_thread_stack_size );
int  ttds_threadpool_run( int (*func)(void *arg), void *arg, int thread_stack_size_need, const char *name );
ttds_threadpool_t *ttds_threadpool_default(void);
void ttds_threadpool_startup(void);
int  ttds_threadpool_stack_used(void);
void ttds_threadpool_timer_start(ttds_timer_t *p_timer, int ms);
void ttds_threadpool_timer_remove(ttds_timer_t *p_timer);
int ttds_idle_get_sleep_time(void);

#define ttds_condition_variable_init(pthis) ttds_cosem_init(&(pthis)->sem, 0)
#define ttds_condition_variable_destroy(pthis) ttds_cosem_destroy(&(pthis)->sem)
void ttds_condition_variable_wait( ttds_condition_variable_t *pthis, ttds_comutex_t *p_mutex);
ttds_coerr_t ttds_condition_variable_wait_for( ttds_condition_variable_t *p_this, ttds_comutex_t *p_mutex, int delay_ms);
#define ttds_condition_variable_notify_one(pthis) ttds_cosem_give(&(pthis)->sem, 1)
#define ttds_condition_variable_notify_all(pthis) ttds_cosem_give_all(&(pthis)->sem)

#ifdef __cplusplus
ttds_coCPLUSPLUS_END
#endif



#endif
#ifdef DEF_MYPT_C_FUNCTION_BODY
/*
 * File: ttds_cothread.c
 * Author: Liang YaoZhan <262666882@qq.com>
 * Brief:  co-routine implemnted through setjmp
 *
 * Copyright (c) 2022 - 2023 Liang YaoZhan <262666882@qq.com>
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2023-08-05 Liang YaoZhan <262666882@qq.com> created
 *
 */

#include <assert.h>
#include <string.h>


#ifndef NULL
#define NULL (void *)0
#endif

static void ttds_threadpool_run_ready_internal(void);
static ttds_cothread_t *ttds_threadpool_get_ready( ttds_threadpool_t *p_this );
static void ttds_threadpool_add(ttds_cothread_t *p_thread);
void ttds_cothread_resume( ttds_cothread_t *p_thread, int code);
void ttds_cothread_suspend(ttds_cothread_t *p_this);
void ttds_cothread_init(ttds_cothread_t *p_thread, int stack_size );
static void ttds_cothread_prepare_delay(ttds_cothread_t *p_this, int tick);
static void ttds_cothread_yeild_do(ttds_cothread_t *p_this);
static void ttds_cothread_make_ready(ttds_cothread_t *p_thread);
static void ttds_threadpool_make_ready(ttds_threadpool_t *p_this, ttds_cothread_t *p_thread);
static void __stack_clear( ttds_cothread_t *p_this, int index, char *parent );
static void cthread_stack_extend( ttds_cothread_t *p_this, int index, base_reg_t *parent);
void ttds_cothread_first_thread_init( ttds_cothread_t *p_thread, int stack_size );

static void ttds_cotimeout_cb__(ttds_timer_t *p_timer, void *arg){
    ttds_cothread_t *p_thread = (ttds_cothread_t*)arg;
    ttds_cothread_make_ready( p_thread );
    p_thread->err = ttds_coETIMEOUT;
}

void ttds_cothread_init(ttds_cothread_t *p_thread, int stack_size )
{
    memset(p_thread, 0x00, sizeof(*p_thread));
    ttds_cothread_t *ptr = (p_thread);
    ttds_queue_init(&ptr->node_state);
    ttds_queue_init(&ptr->node_pending_resource);
    ttds_timer_init(&p_thread->timer, ttds_cotimeout_cb__, p_thread );
    ptr->stack_size = stack_size;
    ptr->state = ttds_coSTATE_INIT;
}

static void ttds_cothread_make_ready(ttds_cothread_t *p_thread)
{
    ttds_threadpool_t *p_loop = ttds_threadpool_default();
    ttds_threadpool_make_ready(p_loop, p_thread);
}

void ttds_cothread_startup(ttds_cothread_t *p_thread )
{
    ttds_threadpool_t *p_loop = ttds_threadpool_default();

    if ( p_thread->state == ttds_coSTATE_INIT){
        int ret;
        ttds_threadpool_add(p_thread);
        ret = setjmp( p_loop->jb_create_thread_res );
        if (ret == 0){
            p_loop->p_thread_to_init_stack = p_thread;
            longjmp( p_loop->jb_create_thread_req, 1 );
        }
        ttds_cothread_make_ready(p_thread);
    }
}

void ttds_threadpool_startup(void)
{
    ttds_threadpool_t *p_this = ttds_threadpool_default();
    int ret;
    p_this->systick = ttds_cothread_sys_tick_get_ms();
    ret = setjmp(p_this->main.jb);
    if (ret == 0){
        ttds_threadpool_run_ready_internal();
        return ;
    }
    p_this->enable_stack_check = 0;
    p_this->main.stack_check = (char*)&p_this;
    __stack_clear( &p_this->main, -2, NULL );
}

static void ttds_threadpool_run_ready_internal(void)
{
    ttds_threadpool_t *p_this = ttds_threadpool_default();
    ttds_cothread_t *old = p_this->p_thread_current;

    if (p_this->enable_stack_check && old ){
        if (p_this->is_stack_going_down) {
            ttds_assert( (char *)&p_this >= old->stack_low && "stack overflow!" );
        } else {
            ttds_assert( (char *)&p_this <= old->stack_high && "stack overflow!" );
        }
        if (p_this->is_stack_going_down){
            if (old->stack_check > (char*)&p_this ){
                old->stack_check = (char*)&p_this;
            }
            char *p = old->stack_low;
            while ( 0x55 == *p && p< old->stack_high ){ p++; }
            if ( p < old->stack_check ){
                old->stack_check = p;
            }
            int diff = old->stack_check - old->stack_low;
            ttds_assert( diff > sizeof(char*)*16 && "stack overflow!" );
        } else {
            if (old->stack_check < (char *)&p_this){
                old->stack_check = (char*)&p_this;
            }
            char *p = old->stack_high;
            while ( 0x55 == *p && p > old->stack_low ){ p--; }
            if ( p > old->stack_check ){
                old->stack_check = p;
            }
            int diff = old->stack_high - old->stack_check;
            ttds_assert( diff > sizeof(char*)*16 && "stack overflow!" );
        }
    }

    p_this->p_thread_current = ttds_threadpool_get_ready(p_this);
    if (!p_this->p_thread_current){
        ttds_assert( 0 && "BUG, no thread to run");
        while (1){}
    }
    longjmp( p_this->p_thread_current->jb, 1 );
}

int  ttds_threadpool_stack_used(void)
{
    ttds_threadpool_t *p_this = ttds_threadpool_default();
    int size = p_this->p_stack_start - p_this->p_stack_current;
    if (size < 0){
        size = -size;
    }
    return size;
}

int ttds_cothread_get_stack_used(ttds_cothread_t *ptr )
{
    if (ptr == NULL){
        ptr = ttds_cothread_self();
    }
    if (ttds_threadpool_default()->is_stack_going_down){
        char *p = ptr->stack_low;
        while ( 0x55 == *p && p < ptr->stack_high ){ p++; }
        if ( p < ptr->stack_check ){
            ptr->stack_check = p;
        }
        int diff = ptr->stack_high - ptr->stack_check;
        ttds_assert(diff > 0 && "stack overflow!");
        return diff;
    } else {
        char *p = ptr->stack_high;
        while ( 0x55 == *p && p>ptr->stack_low){ p--; }
        if ( p > ptr->stack_check ){
            ptr->stack_check = p;
        }
        int diff = ptr->stack_check - ptr->stack_low;
        ttds_assert(diff > 0 && "stack overflow!");
        return diff >=0 ? diff: -diff;
    }
}

void ttds_cothread_first_thread_init( ttds_cothread_t *p_thread, int stack_size )
{
    ttds_threadpool_t *p_this = ttds_threadpool_default();
    int ret;

    ttds_cothread_init( p_thread, stack_size );
    p_thread->name = "main";
    ttds_threadpool_add(p_thread);
    ret = setjmp( p_this->jb_create_thread_res );
    if (ret == 0){
        p_this->p_stack_start = (char *)&p_thread;
        p_this->p_thread_current = p_thread;
        cthread_stack_extend( p_thread, -1, 0 );
    }
    if (p_this->is_stack_going_down){
        p_this->main.stack_high = (char*)&p_this;
    }
    ttds_cothread_make_ready(&p_this->main);
}


void ttds_threadpool_init(int main_thread_stack_size)
{
    ttds_threadpool_t *p_this = ttds_threadpool_default();

    memset(p_this, 0x00, sizeof(*p_this));
    ttds_queue_init(&p_this->head_state_pending);
    ttds_queue_init(&p_this->head_state_running);
    ttds_tick_init( &p_this->ticker);
    p_this->p_thread_current = NULL;
    ttds_cothread_first_thread_init( &p_this->main, main_thread_stack_size );
}

void ttds_threadpool_add_thread(ttds_cothread_t *p_threads, int count,int main_thread_stack_size )
{
    int i;
    for (i=0; i<count; i++){
        ttds_cothread_init( p_threads, main_thread_stack_size );
        ttds_cothread_startup( p_threads );
        ttds_cothread_suspend( p_threads );
        p_threads++;
    }
}

int ttds_threadpool_run( int (*func)(void *arg), void *arg, int thread_stack_size_need, const char *name )
{
    ttds_threadpool_t *p_this = ttds_threadpool_default();
    ttds_cothread_t *p_thread_selected = NULL;
    ttds_cothread_t *p_thread;
    ttds_cothread_t *p_thread_tmp;
    ttds_queue_for_each_container_safe(p_thread, p_thread_tmp, &p_this->head_state_pending, ttds_cothread_t, node_state ){
        if (p_thread->entry == NULL){
            if ( thread_stack_size_need == p_thread->stack_size ){
                p_thread_selected = p_thread;
                break;
            }
            if ( p_thread->stack_size > thread_stack_size_need ) {
                if ( p_thread_selected == NULL){
                    p_thread_selected = p_thread;
                } else {
                    if ( p_thread->stack_size < p_thread_selected->stack_size ){
                        p_thread_selected = p_thread;
                    }
                }
            }
        }
    }
    if (p_thread_selected){
        p_thread_selected->entry = func;
        p_thread_selected->arg = arg;
        p_thread_selected->name = name;
        ttds_cothread_resume( p_thread_selected, ttds_coEOK );
        return 1;
    }
    return 0;
}

static void __stack_clear( ttds_cothread_t *p_this, int index, char *parent )
{
    char *ptr = NULL;
    if ( index >= 0 && parent ){
        if ( (char*)&ptr < parent ){ /* going down */
            int diff = parent - (char*)&ptr;
            memset( p_this->stack_low, 0x55, (char*)&ptr - p_this->stack_low+1 - diff );
        } else {/* going up */
            int diff = (char*)&ptr - parent;
            memset( (char*)&ptr+diff, 0x55, p_this->stack_high- (char*)&ptr+1 );
        }
        return ;
    }
    if (parent)
        __stack_clear(p_this, index+1, parent );
    else
        __stack_clear(p_this, index+1, (char*)&ptr );
}

static void pooled_thread_entry( void )
{
    ttds_cothread_t *p_this = ttds_cothread_self();
    while (1){
        if (p_this->entry){
            p_this->stack_check = (char*)&p_this;
            __stack_clear( p_this, -2, NULL );
            p_this->exit_code = p_this->entry(p_this->arg);
            p_this->entry = NULL;
            p_this->arg = NULL;
        } else {
            ttds_cothread_suspend(  p_this );
            ttds_cothread_yeild_do( p_this );
        }
    }
}

ttds_threadpool_t *ttds_threadpool_default(void)
{
    static ttds_threadpool_t g_global_loop_obj;
    return &g_global_loop_obj;
}

static ttds_cothread_t *ttds_threadpool_get_ready( ttds_threadpool_t *p_this )
{
    ttds_cothread_t *ptr_thread;
    int32_t now = ttds_cothread_sys_tick_get_ms();
    int diff;

    diff = now - p_this->systick;
    if (diff){
        p_this->systick = now;
        ttds_tick_increase( &p_this->ticker, diff );
    }

    if ( !ttds_queue_empty(&p_this->head_state_running)){
        ttds_queue_t *ptr = p_this->head_state_running.next;
        ttds_queue_remove(ptr);
        ttds_queue_insert_tail(&p_this->head_state_running, ptr);
        ptr_thread = ttds_queue_container(ptr, ttds_cothread_t, node_state );
        return ptr_thread;
    }
    return NULL;
}

static void ttds_threadpool_add(ttds_cothread_t *p_thread)
{
    ttds_threadpool_t *p_this = ttds_threadpool_default();
    if (!p_thread)
    {
        return;
    }
    ttds_queue_insert_tail( &p_this->head_state_pending, &p_thread->node_state );
}
void ttds_threadpool_timer_start(ttds_timer_t *p_timer, int tick )
{
    ttds_threadpool_t *p_this  = ttds_threadpool_default();
    ttds_tick_add_timer( &p_this->ticker, p_timer, tick );
}

void ttds_threadpool_timer_remove(ttds_timer_t *p_timer)
{
    ttds_threadpool_t *p_this  = ttds_threadpool_default();
    ttds_tick_remove_timer( &p_this->ticker, p_timer );
}

void ttds_cothread_suspend(ttds_cothread_t *p_this)
{
    ttds_threadpool_t *p_loop = ttds_threadpool_default();
    ttds_queue_remove( &p_this->node_state );
    ttds_queue_insert_tail( &p_loop->head_state_pending, &p_this->node_state );
    p_this->state = ttds_coSTATE_PENDING;
    p_this->err = ttds_coEOK;
}

static void ttds_cothread_prepare_delay(ttds_cothread_t *p_this, int tick)
{
    ttds_threadpool_t *p_loop= ttds_threadpool_default();
    ttds_cothread_suspend(p_this);
    if ( tick != -1){
        p_this->timer.timeout_callback = ttds_cotimeout_cb__;
        p_this->timer.arg = p_this;
        ttds_tick_add_timer( &p_loop->ticker, &p_this->timer, tick );
    }
    ttds_cothread_yeild_do(p_this);
}

static void ttds_cothread_yeild_do(ttds_cothread_t *p_this)
{
    int ret = setjmp(p_this->jb);
    if (ret == 0){
        ttds_threadpool_run_ready_internal();
    } else {
        ttds_cothread_t *p = ttds_cothread_self();
        ttds_assert( p == p_this );
    }
}

void ttds_cothread_yeild(void)
{
    ttds_cothread_t *p_this = ttds_cothread_self();
    ttds_cothread_yeild_do( p_this );
}

ttds_cothread_t *ttds_cothread_self()
{
    ttds_threadpool_t *p_loop= ttds_threadpool_default();
    return p_loop->p_thread_current;
}
void ttds_cothread_delay(int tick)
{
    ttds_cothread_t *p_this = ttds_cothread_self();
    ttds_cothread_prepare_delay( p_this, tick );
}

static void ttds_threadpool_make_ready(ttds_threadpool_t *p_this, ttds_cothread_t *p_thread)
{
    if (!p_this){
        return ;
    }
    ttds_queue_remove( &p_thread->node_state );
    ttds_queue_remove( &p_thread->node_pending_resource );
    ttds_queue_insert_tail( &p_this->head_state_running, &p_thread->node_state );
    p_thread->state = ttds_coSTATE_READY;
}
void ttds_cosem_init( ttds_cosem_t *p_this, int init_count )
{
    ttds_queue_init( &p_this->head_pendings );
    p_this->count = init_count;
}

ttds_coerr_t ttds_cosem_take( ttds_cosem_t *p_this, int delay_tick ) /* delay_tick: -1 forevery */
{
    ttds_cothread_t *p_thread = ttds_cothread_self();
    if (p_this->count > 0){
        p_this->count--;
        p_thread->err = ttds_coEOK;
        return ttds_coEOK;
    }
    ttds_assert( ttds_queue_empty(&p_thread->node_pending_resource) );
    ttds_queue_insert_tail( &p_this->head_pendings, &p_thread->node_pending_resource );
    ttds_cothread_prepare_delay( p_thread, delay_tick );
    return p_thread->err;
}

void ttds_cothread_resume( ttds_cothread_t *p_thread, int code)
{
    ttds_threadpool_t *p_loop= ttds_threadpool_default();
    ttds_queue_remove( &p_thread->node_pending_resource );
    ttds_tick_remove_timer( &p_loop->ticker, &p_thread->timer );
    ttds_cothread_make_ready( p_thread );
    p_thread->err = (ttds_coerr_t)code;
}

void ttds_cosem_give( ttds_cosem_t *p_this, int count )
{
    int i;

    for (i=0; i<count && !ttds_queue_empty(&p_this->head_pendings); i++){
        ttds_cothread_t *p_thread = ttds_queue_container( p_this->head_pendings.next, ttds_cothread_t, node_pending_resource );
        ttds_cothread_resume( p_thread, ttds_coEOK );
    }
    p_this->count += count - i;
}

void ttds_cosem_give_all( ttds_cosem_t *p_this )
{
    for (; !ttds_queue_empty(&p_this->head_pendings); ){
        ttds_cothread_t *p_thread = ttds_queue_container( p_this->head_pendings.next, ttds_cothread_t, node_pending_resource );
        ttds_cothread_resume( p_thread, ttds_coEOK );
    }
}

void ttds_cosem_destroy( ttds_cosem_t *p_this )
{
    while ( !ttds_queue_empty(&p_this->head_pendings) ){
        ttds_cothread_t *p_thread = ttds_queue_container( p_this->head_pendings.next, ttds_cothread_t, node_pending_resource );
        ttds_cothread_resume( p_thread, ttds_coETERMINATED );
    }
}

void ttds_comutex_init(ttds_comutex_t *p_this, int is_recursive)
{
    memset( p_this, 0x00, sizeof(*p_this) );
    p_this->is_recursive = is_recursive;
    ttds_queue_init( &p_this->head_pendings );
}

ttds_coerr_t ttds_comutex_lock( ttds_comutex_t *p_this, int delay_tick ) /* delay_tick: -1 forevery */
{
    ttds_cothread_t *p_thread = ttds_cothread_self();
    if (p_this->p_owner == NULL){
        p_this->p_owner = p_thread;
        p_this->recursive = 1;
        p_thread->err = ttds_coEOK;
        return ttds_coEOK;
    }
    if (p_this->p_owner == p_thread && p_this->is_recursive){
        p_this->recursive++;
        return ttds_coEOK;
    }
    ttds_queue_insert_tail( &p_this->head_pendings, &p_thread->node_pending_resource );
    ttds_cothread_prepare_delay( p_thread, delay_tick );
    ttds_assert( p_this->p_owner == p_thread );
    return p_thread->err;
}
void ttds_comutex_unlock( ttds_comutex_t *p_this )
{
    ttds_cothread_t *p_thread = ttds_cothread_self();
    if (p_thread == p_this->p_owner){
        if ( --p_this->recursive == 0){
            p_this->p_owner = NULL;
            if ( !ttds_queue_empty(&p_this->head_pendings)){
                ttds_cothread_t *p_waiting_thread = ttds_queue_container( p_this->head_pendings.next, ttds_cothread_t, node_pending_resource );
                ttds_cothread_resume( p_waiting_thread, ttds_coEOK );
                p_this->p_owner = p_waiting_thread;
                p_this->recursive = 1;
            }
        }
    }
}
void ttds_comutex_destroy( ttds_comutex_t *p_this )
{
    ttds_assert( p_this->p_owner == NULL && "destroy busy mutex!" );
    while ( !ttds_queue_empty(&p_this->head_pendings) ){
        ttds_cothread_t *p_thread = ttds_queue_container( p_this->head_pendings.next, ttds_cothread_t, node_pending_resource );
        ttds_cothread_resume( p_thread, ttds_coETERMINATED );
    }
}

void ttds_condition_variable_wait( ttds_condition_variable_t *p_this, ttds_comutex_t *p_mutex)
{
    ttds_condition_variable_wait_for( p_this, p_mutex, -1 );
}
ttds_coerr_t ttds_condition_variable_wait_for( ttds_condition_variable_t *p_this, ttds_comutex_t *p_mutex, int delay_ms)
{
    ttds_coerr_t err;
    ttds_comutex_unlock( p_mutex );
    err = ttds_cosem_take( &p_this->sem, delay_ms );
    ttds_comutex_lock( p_mutex, -1 );
    return err;
}

static void ttds_threadpool_at_stack_bottom( int index)
{
    if (index == 0){
        ttds_threadpool_t *p_this = ttds_threadpool_default();
        int ret = setjmp(p_this->jb_create_thread_req);
        if (ret){
            if ( p_this != ttds_threadpool_default() ){
                ttds_assert( 0 && "stack overflow!");
                while (1){}
            }
            cthread_stack_extend( p_this->p_thread_to_init_stack, -1, 0 );
            ttds_assert( 0 && "never return");
            while (1){
                ;/* never return here */
            }
        }
        p_this->p_stack_current = (char *)&p_this;
        longjmp( p_this->jb_create_thread_res, 1 );
    }
    ttds_threadpool_at_stack_bottom( index+1 );
}

static void cthread_stack_extend( ttds_cothread_t *p_this, int index, base_reg_t *parent)
{
    base_reg_t ptr[32];
    int stack_size = 0;

    if (index == 2){
        if (setjmp( p_this->jb )){
            pooled_thread_entry();
            return;
        }
    }
    else if (index > 0) {
        stack_size = parent - (base_reg_t*)&ptr;
        if (stack_size < 0) {/* going up */
            stack_size = -stack_size;
            p_this->stack_low = (char*)parent;
            p_this->stack_high= (char*)&ptr;
            p_this->stack_check = p_this->stack_low;
        } else {/* goging down */
            p_this->stack_low = (char*)&ptr;
            p_this->stack_high= (char*)parent;
            p_this->stack_check = p_this->stack_high;
        }
        stack_size *= sizeof(base_reg_t);
        if (stack_size >= p_this->stack_size ){
            if ( parent > ptr){
                ttds_threadpool_default()->is_stack_going_down = 1;
            }
            ttds_threadpool_at_stack_bottom( -1 );
        }
    }
    if (parent){
        cthread_stack_extend(p_this, index + 1, parent);
    } else {
        cthread_stack_extend(p_this, index + 1, ptr);
    }
}

int ttds_idle_get_sleep_time(void)
{
    ttds_threadpool_t *p_loop = ttds_threadpool_default();
    ttds_queue_t *prev = p_loop->head_state_running.prev;
    ttds_queue_t *next = p_loop->head_state_running.next;
    ttds_assert( !ttds_queue_empty(&p_loop->head_state_running) );
    if (prev == next ){
        /* only one thread now, it must be idle thread */
        return ttds_tick_next( &p_loop->ticker );
    }
    return 0;
}

/*
 * File: ttds_timer.c
 * Author: Liang YaoZhan <262666882@qq.com>
 * Brief:  timer
 *
 * Copyright (c) 2022 - 2023 Liang YaoZhan <262666882@qq.com>
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2023-08-05 Liang YaoZhan <262666882@qq.com> created
 *
 */

#include <stdlib.h>


void ttds_timer_init( ttds_timer_t *p_this, void (*cb)(ttds_timer_t *p_this, void*arg), void *arg)
{
    ttds_queue_init( &p_this->node );
    p_this->timeout_callback = cb;
    p_this->arg = arg;
    p_this->tick = 0;
    p_this->p_ticker = NULL;
}
void ttds_timer_destroy( ttds_timer_t *p_this )
{
    ttds_queue_remove( &p_this->node );
    p_this->timeout_callback = 0;
}
void ttds_timer_restart( ttds_timer_t *p_timer )
{
    ttds_tick_t *p_tick = p_timer->p_ticker;
    if (!p_tick)return ;
    ttds_tick_remove_timer( p_tick, p_timer );
    ttds_tick_add_timer(  p_tick, p_timer, p_timer->tick_origin );
}


void ttds_tick_init(  ttds_tick_t *p_tick)
{
    ttds_queue_init(&p_tick->head);
}

void ttds_tick_increase(  ttds_tick_t *p_tick, int tick )
{
    while (tick > 0 && (!ttds_queue_empty(&p_tick->head)) ){
        ttds_timer_t *p_timer;
        p_timer = ttds_queue_container(p_tick->head.next, ttds_timer_t, node);
        if (p_timer->tick < tick){
            tick -= p_timer->tick;
            p_timer->tick = 0;
        } else {
            p_timer->tick -= tick;
            tick = 0;
            if ( p_timer->tick > 0){
                return ;
            }
        }
        while ( !ttds_queue_empty(&p_tick->head)){
            ttds_timer_t *pt = ttds_queue_container(p_tick->head.next, ttds_timer_t, node);
            if (pt->tick == 0){
                ttds_queue_remove( &pt->node );
                if (pt->timeout_callback){
                    pt->timeout_callback(pt, pt->arg);
                }
            }else{
                break;
            }
        }
    }
}

void ttds_tick_destroy(  ttds_tick_t *p_this)
{
    while (!ttds_queue_empty(&p_this->head)){
        ttds_queue_remove( p_this->head.next );
    }
}

void ttds_tick_add_timer(  ttds_tick_t *p_tick, ttds_timer_t *p_timer, int tick)
{
    ttds_timer_t *iter;
    p_timer->tick_origin = tick;
    p_timer->p_ticker = p_tick;
    ttds_queue_for_each_container(iter, &p_tick->head, ttds_timer_t, node){
        if (tick > iter->tick){
            tick -= iter->tick;
        } else {
            break;
        }
    }
    p_timer->tick = tick;
    ttds_queue_insert_tail( &iter->node, &p_timer->node );
    if ( &iter->node != &p_tick->head){
        iter->tick -= tick;
    }
}

void ttds_tick_remove_timer(  ttds_tick_t *p_tick, ttds_timer_t *p_timer)
{
    if ( ttds_queue_empty(&p_timer->node )){
        return ;
    }
    if ( p_timer->node.next != &p_tick->head ){
        ttds_timer_t *p_timer_next = ttds_queue_container(p_timer->node.next, ttds_timer_t, node);
        p_timer_next->tick += p_timer->tick;
    }
    ttds_queue_remove( &p_timer->node );
}

int  ttds_tick_next(ttds_tick_t *p_this)
{
    if (ttds_queue_empty(&p_this->head)){
        return -1;
    }
    ttds_timer_t *pt = ttds_queue_container(p_this->head.next, ttds_timer_t, node);
    return pt->tick;
}

#endif /* DEF_MYPT_C_FUNCTION_BODY */

#ifdef __cplusplus
}
#endif
