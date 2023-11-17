1. posix 线程是一套标准，而不是实现。类似于 cmsis-rtos 这种标准

2. 线程的创建
    ```c
        /*
         * @param thread 是 pthread_t 类型的指针，此函数会回填
         * @param attr 是线程属性,为空跟着默认走
         * @param 线程 start_routine 是入口函数
         * @param *arg 是函数入口的参数
        */
        int pthread_create(pthread_t *thread, const pthread_attr_t  *attr,
                          void *(*start_routine) (void *), void *arg); // 详细介绍可以看 man 手册（man pthread_create）
    ```
    线程的调度取决于调度器策略

3. 线程的终止
    3 种方式:
        * 线程从启动例程返回，返回值就是线程的退出码
        * 线程可以被同一进程中的其他线程取消
        * 线程调用 pthread_exit() 函数
    
    pthread_join()  -----> 相当于进程的 wait() 原语 ，收尸

    栈的清理
        pthread_cleanup_push();
        pthread_cleanup_pop();