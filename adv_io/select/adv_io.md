### 高级IO

#### 1.非阻塞IO

#### 2. IO 多路转接
* 监视文件描述符，当布置的监视事件发生时，才会返回
* select:缺点 int 类型，监视的文件描述符有限制，监视的事件太少，除了读写其他都是错误事件，当监视到时，需要重新布置监视任务事件，影响效率。优点：可移植性好
* poll: 无上述缺点
* epool: linux 方言