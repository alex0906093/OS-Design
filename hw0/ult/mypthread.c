/* BY : APoorv || Alex */
//-----INCLUDES----
#include "mypthread.h"
#include "queue.h"
//------MACROS-----
#define C2LWR(c) ((c>0x40 && c<0x5b)? c | 0x60:c)
#define MAX(c,d) (c>d?c:d)
#define FMT_STR(c,s) CYAN "%d:" c "T-%d> " s "\n" RESET,__LINE__,CurThr->thr_id
#define STACK_SIZE 16384
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
#define PRIORITY(i) (i?QDT->thr_id>CurThr->thr_id:QDT->thr_id<=CurThr->thr_id)
#define THREAD mypthread_t
#define QDT    ((THREAD *)q->data)
//------STATES-----
#define T_DEAD    0
#define T_ACTIVE  1
#define T_BLOCKED 2
//------ERR_CD-----
#define NAH 1
#define YAY 0
//------DEBUGS-----
#define DEBUG //--Code Out @ End
#ifdef DEBUG
#   define D(x) x
#else
#   define D(x) 
#endif
//-----------------
Queue *Queue_m=0;ucontext_t *Main=0;THREAD *CurThr=0;QueueItem *q=0;
int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr, void *(*start_routine) (void *), void *ar) {
    static int id=0;static bool FirstCall=1;ucontext_t *ctx = (ucontext_t*)calloc(1,sizeof(ucontext_t));
    if (FirstCall) {
        Queue_m = create_queue();
        FirstCall=0;getcontext(ctx);Main = ctx;
        mypthread_t *tmp = CurThr = (THREAD*)malloc(sizeof(THREAD));
        *tmp = INIT_THREAD(T_ACTIVE);
        enqueue(Queue_m,tmp);
        D(printf(FMT_STR(GREEN,"Created Main Thread [T-%d]"),tmp->thr_id));
        ctx = (ucontext_t*)calloc(1,sizeof(ucontext_t));
    }
    getcontext(ctx);
    ctx->uc_stack.ss_size = attr&&attr->StackSize?attr->StackSize:STACK_SIZE;
    ctx->uc_stack.ss_sp   = (char*)malloc(ctx->uc_stack.ss_size);
    ctx->uc_link = 0;
    *thread = INIT_THREAD(T_ACTIVE);
    enqueue(Queue_m,thread);
    makecontext(thread->context,(void *)start_routine,1,ar);
    D(printf(FMT_STR(GREEN,"Created Thread [T-%d]"),thread->thr_id));
    return YAY;
}
void mypthread_exit(void *retval) {
    CurThr->state = T_DEAD;bool AllDead=1;
    if (CurThr->Waiter) {CurThr->Waiter->state = T_ACTIVE;}
    free(CurThr->context->uc_stack.ss_sp);free(CurThr->context);
    D(printf(FMT_STR(YELLOW,"Bye.")));
    //QUEUE_SCAN(q,Queue_m,i) {if (q->state!=T_DEAD) {AllDead=0;break;}}
    //if (AllDead) {mypthread_free();setcontext(Main);} else {
    mypthread_yield();
    //}
}
int mypthread_free(void) {
    THREAD *ptr=0;
    while (ptr = dequeue(Queue_m)) {free(ptr);}
}
int mypthread_yield(void) {
    bool Change=0;
    ITER_LT(j,2) {
        QUEUE_SCAN(q,Queue_m,i) {
            D(printf(FMT_STR(CYAN,"Scanning Thr [T-%d] @ Priority:%d"),QDT->thr_id,j));
            if (QDT->state!=T_ACTIVE||PRIORITY(j)) {continue;}
            D(if (QDT->thr_id==32) {
                int debb=1;
            });
            D(printf(FMT_STR(CYAN,"Switching Context -> [T-%d]"),QDT->thr_id));
            ucontext_t *c=CurThr->context;CurThr=QDT;
            swapcontext(c,CurThr->context);Change=1;break;
        }
        if (Change) {break;}
    }
    return Change?YAY:NAH;
}
int mypthread_join(mypthread_t thread, void **retval) {
    if (thread.thr_id==CurThr->thr_id) {D(printf(FMT_STR(RED,"Detected DEADLOCK when joining with T-%d"),thread.thr_id));return EDEADLK;}
    bool found=0;
    QUEUE_SCAN(q,Queue_m,i) {if (QDT->thr_id==thread.thr_id) {found=1;break;}}
    if (!found) {D(printf(FMT_STR(YELLOW,"Couldn't find thread with ID:%d"),thread.thr_id));return ESRCH;}
    if (QDT->state==T_DEAD) {D(printf(FMT_STR(YELLOW,"Thread %d is dead. :("),QDT->thr_id));return EINVAL;}
    if (QDT->Waiter) {D(printf(FMT_STR(RED,"TH:[%d] already waiting on %d"),QDT->Waiter->thr_id,QDT->thr_id));return EINVAL;}
    CurThr->state = T_BLOCKED;
    QDT->Waiter = CurThr;
    D(printf(FMT_STR(CYAN,"Joined @ [T-%d]"),QDT->thr_id));
    return mypthread_yield();
}
