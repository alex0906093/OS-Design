
int mypthread_create(mypthread_t *thread, const mypthread_attr_t *attr, void *(*start_routine) (void *), void *ar);

void mypthread_exit(void *retval);

int mypthread_yield(void);

int mypthread_join(mypthread_t thread, void **retval);
