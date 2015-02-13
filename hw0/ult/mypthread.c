/* BY : APoorv || Alex */
//-----INCLUDES----
#include <ucontext.h>
#include <stdio.h>
#include "queue.h"
//------MACROS-----
#define C2LWR(c)   ((c>0x40 && c<0x5b)? c | 0x60:c)
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
#define ITER_LT(i,j) for (int i=0;i<j;i++)
//------STATES-----
#define T_RUN     1
#define T_BLOCKED 2
//------ERR_CD-----
#define FAIL 1
#define SUCC 0
//------DEBUGS-----
//#define DEBUG //--Code Out @ End
#ifdef DEBUG
#   define D(x) x
#else
#   define D(x) 
#endif
//-----------------
Queue *Queue = 0;ucontext_t *Main = 0;
int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr, void *(*start_routine) (void *), void *ar) {
<<<<<<< HEAD
    static int id=0;static char FirstCall='y';ucontext_t *ctx = (ucontext_t*)calloc(1,sizeof(ucontext_t));
    if (FirstCall) {Queue = create_queue();FirstCall=0;GetContext(ctx);Main = ctx;}
    ctx->uc_stack.ss_sp   = (char*)malloc(STACK_SIZE);
    ctx->uc_stack.ss_size = STACK_SIZE;
    ctx->uc_link = TaskScheduler;
    *thread = (mypthread_t){id++,T_BLOCKED,ctx}
    enqueue(Queue,thread);
    makecontext(&ctx,start_routine,ar);
}
void TaskScheduler() {
    ucontext_t t;getcontext(t);
    while (1) {
        thread=dequeue(Queue);
        if (thread->state==T_RUN) {break;}
    }
    swapcontext(&t,)
=======
    static char FirstCall='y';int id=0;
    if (FirstCall) {  }
    else {id = GetEmptySlot();}
    *thread = (mypthread_t){id,2,GetContext()}
    FirstCall=0;
>>>>>>> e7dc429d20deff2e1e9858ad718601732a80e0fc
}
void mypthread_exit(void *retval) {

}
int mypthread_yield(void) {
    
}
int mypthread_join(mypthread_t thread, void **retval) {
}
