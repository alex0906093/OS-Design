/* BY : APoorv || Alex */
//-----INCLUDES----
#include <ucontext.h>
#include <stdio.h>
#include <stdbool.h>
#include "queue.h"
//------MACROS-----
#define C2LWR(c) ((c>0x40 && c<0x5b)? c | 0x60:c)
#define MAX(c,d) (c>d?c:d)
#define FMT_STR(c,s) CYAN __LINE__ ":" c "T-%d> " s "\n" RESET,CurThr->thr_id
#define STACK_SIZE 16384
#define THREAD mypthread_t
#define THREAD_LOCKS 300
//------COLORS-----
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
#define UNDERLINED_PRE  "\x1b[04m"
#define UNDERLINED_POST "\x1b[24m"
//------AUTOMATE---
#define ITER_LT(i,j) for (int i=0;i<j;i++)
#define INIT_THREAD(x) (mypthread_t){id++,x,ctx,0}
//------STATES-----
#define T_DEAD    0
#define T_ACTIVE  1
#define T_BLOCKED 2
//------ERR_CD-----
#define NAH 1
#define YAY 0
//------DEBUGS-----
//#define DEBUG //--Code Out @ End
#ifdef DEBUG
#   define D(x) x
#else
#   define D(x) 
#endif
//-----------------
Queue *Queue=0;ucontext_t *Main=0;THREAD *CurThr=0;
int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr, void *(*start_routine) (void *), void *ar) {
    static int id=0;static char FirstCall='y';ucontext_t *ctx = (ucontext_t*)calloc(1,sizeof(ucontext_t));
    if (FirstCall) {
        Queue = create_queue();
        FirstCall=0;GetContext(ctx);Main = ctx;
        mypthread_t *tmp = (mypthread_t*)malloc(sizeof(mypthread_t))
        *tmp = INIT_THREAD(T_ACTIVE);
        enqueue(Queue,tmp);
        ctx = (ucontext_t*)calloc(1,sizeof(ucontext_t));
    }
    ctx->uc_stack.ss_sp   = (char*)malloc(STACK_SIZE);
    ctx->uc_stack.ss_size = attr&&attr->StackSize?attr->StackSize:STACK_SIZE;
    ctx->uc_link = Main;
    *thread = INIT_THREAD(T_ACTIVE);
    enqueue(Queue,thread);
    makecontext(&ctx,start_routine,ar);
    return YAY;
}
void mypthread_exit(void *retval) {
    CurThr->state = T_DEAD;bool AllDead=0;
    if (CurThr->Waiter) {CurThr->Waiter->state = T_ACTIVE;}
    free(CurThr->context->uc_stack.ss_sp);
    QUEUE_SCAN(q,Queue,i) {if (q->state!=T_DEAD) {AllDead=1;}}
    if (AllDead) {mypthread_free();setcontext(Main);} else {mypthread_yield();}
}
int mypthread_free(void) {
    THREAD *ptr=0;
    while (ptr = dequeue(Queue)) {free(ptr->context);free(ptr);}
}
int mypthread_yield(void) {
    bool Change=0;
    QUEUE_SCAN(q,Queue,i) {
        if (q->state!=T_ACTIVE) {continue}
        swapcontext(CurThr->context,q->context);CurThr=q;Change=1;
    }
    return Change?YAY:NAH;
}
int mypthread_join(mypthread_t thread, void **retval) {
    if (thread.thr_id==CurThr->thr_id) {D(printf(FMT_STR(RED,"Detected DEADLOCK when joining with T-%d"),thread.thr_id));return EDEADLK;}
    bool found=0;
    QUEUE_SCAN(q,Queue,i) {if (q->thr_id==thread.thr_id) {found=1;break;}}
    if (!found) {D(printf(FMT_STR(YELLOW,"Couldn't find thread with ID:%d"),thread.thr_id));return ESRCH;}
    if (q->state==T_DEAD) {D(printf(FMT_STR(YELLOW,"Thread %d is dead. :("),q->thr_id));return EINVAL;}
    if (q->Waiter) {D(printf(FMT_STR(RED,"TH:[%d] already waiting on %d"),q->Waiter->thr_id,q->thr_id));return EINVAL;}
    CurThr->state = T_BLOCKED;
    q->Waiter = CurThr;
    mypthread_yield();
}
