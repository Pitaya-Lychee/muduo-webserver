CC = g++
CFLAG = -Wall -c
OBJS = main.o echo.o tcpserver.o acceptor.o\
	   channel.o eventloop.o epoller.o tcpconnection.o\
	   eventloopthreadpool.o eventloopthread.o thread.o $(TFLAG)
TFLAG = -lpthread

web_server : $(OBJS) 
	$(CC) $^ -o $@

main.o : main.cc eventloop.h address.h echo.h
	$(CC) $(CFLAG) $< -o $@

echo.o : echo.cc echo.h tcpserver.h
	$(CC) $(CFLAG) $< -o $@

tcpserver.o : tcpserver.cc tcpserver.h acceptor.h tcpconnection.h\
			  eventloopthreadpool.h
	$(CC) $(CFLAG) $< -o $@
	
acceptor.o : acceptor.cc acceptor.h address.h channel.h
	$(CC) $(CFLAG) $< -o $@
	
channel.o : channel.cc channel.h
	$(CC) $(CFLAG) $< -o $@

eventloop.o : eventloop.cc eventloop.h channel.h mutex.h
	$(CC) $(CFLAG) $< -o $@

eventloopthreadpool.o : eventloopthreadpool.cc eventloopthreadpool.h eventloopthread.h
	$(CC) $(CFLAG) $< -o $@
	
eventloopthread.o : eventloopthread.cc eventloopthreadpool.h mutex.h\
					condition.h eventloop.h
	$(CC) $(CFLAG) $< -o $@

thread.o : thread.cc thread.h latch.h
	$(CC) $(CFLAG) $< -o $@
	
epoller.o : epoller.cc epoller.h channel.h
	$(CC) $(CFLAG) $< -o $@
	
tcpconnection.o : tcpconnection.cc tcpconnection.h \
					 channel.h
	$(CC) $(CFLAG) $< -o $@

.PHONY : clean
clean:
	rm *.o
