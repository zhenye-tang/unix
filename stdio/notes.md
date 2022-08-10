文件IO 响应速度快
标准IO 的吞吐量大。存在缓存机制，需要从user态到内核态
标准IO与文件IO不可混用
fileno 将标准IO转换为文件IO的文件描述符
fdopen 将文件IO转换为标准IO

real:是 user + sys + 一点的时间,包含一些调度的时间
user:在user层面消耗的时间
sys: 在内核层面消耗的时间

怎么删除文件的第10行？

原子操作：不可分割的操作
原子操作：不可分割的最小单位
原子操作的作用：解决竞争和冲突

程序中的重定向：dup,dup2

linux 中文件大小与 win 不同，linux 中文件大小指的是文件中实际的字节数。但是实际存储在磁盘上的大小是所占 块个数 * 一个块的大小（512字节）
通过 bigfile.c 代码创建文件，发现创建 5M 大小的的文件，仅占了4K磁盘

win 中的大小就是实际所占磁盘的大小

文件属性

umask 限制产生过大权限的文件
chmod a+x(加上可执行权限)，chmod 能够改变文件权限信息

int chmod(const char *pathname, mode_t mode);
int fchmod(int fd, mode_t mode);

粘住位
 t位：把某一个命令的使用痕迹保留，下次装载的时候会比较快

文件系统：fat,ufs
 文件系统：文件或数据的存储和管理

硬链接、符号链接
符号链接：ln -s [src] [dst] 类似于 win 下的快捷方式
硬链接：目录项是同义词，建立硬链接有限制，不能给分区建立、不能给目录建立

int link(const char *oldpath, const char *newpath);
int unlink(const char *pathname); 从磁盘上删除一个文件，硬链接数为0时才真正删除。
能够产生一匿名文件。open 之后 马上 ulink，直到 close 时，才真正从磁盘删除了。
int remove(const char *pathname); 不能删除非空目录。比如 rm -rf

目录创建与销毁
mkdir
rmdir
int chdir(const char *path);//修改当前进程的工作路径
int fchdir(int fd);
char *getcwd(char *buf, size_t size);

分析目录/读取目录内容：
glob: 能够实现下面所有的操作

opendir():
closedir():
readdir():
rewinddir():
seekdir():
telldir():



