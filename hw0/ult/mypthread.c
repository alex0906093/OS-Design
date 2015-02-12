/* BY : APoorv || Alex */
//-----INCLUDES----
#include <stdio.h>
//------MACROS-----
#define C2LWR(c)   ((c>0x40 && c<0x5b)? c | 0x60:c)
#define THREAD_THROTTLE 100
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
//------ERRORS-----
#define NO_ID -1
#define INSUFFICIENT_RESOURCES -2
//------DEBUGS-----
//#define DEBUG //--Code Out @ End
#ifdef DEBUG
#   define D(x) x
#else
#   define D(x) 
#endif
//-----------------
mypthread_t ThreadContainer[THREAD_THROTTLE];
int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr, void *(*start_routine) (void *), void *ar) {
    static char FirstCall='y';int id=0;
    if (FirstCall) {}
    else {id = GetEmptySlot();}
    *thread = (mypthread_t){id,2,GetContext()}
    FirstCall=0;
}
int GetEmptySlot() {for (int i=0;i<THREAD_THROTTLE;i++) {if (!ThreadContainer[i]) {return i;}}}
void mypthread_exit(void *retval) {

}
int mypthread_yield(void) {
}
int mypthread_join(mypthread_t thread, void **retval) {
}
