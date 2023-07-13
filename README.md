# muduo-webserver
muduo网络库学习过程，实现HTTPServer
  1、利用socket编程实现的简单echo服务器；
  2、引用Epoll多路复用实现的echo服务器；
  3、引用进程池，实现的一款echo服务器；
  4、利用muduo网络库的思想，实现单进程的服务器；
  5、实现OneLoop pre Thread+ThreadPool模型的echo服务器；
  6、重写muduo网络库(未加入日志和定时器)，实现的HTTPServer；
  7、对HTTPServer性能进行优化，使用移动语义等特性；

使用：将Address.h中的地址改为本机服务器地址，并使用cmake/make进行编译衔接生成可执行文件webserver；运行./webserver 指定port
