CC = g++
CFLAG = -Wall -c
OBJS = main.o httpserver.o httpresponse.o httpcontent.o httprequest.o\
	   tcpserver.o acceptor.o\
	   channel.o eventloop.o epoller.o tcpconnection.o\
	   eventloopthreadpool.o eventloopthread.o thread.o buffer.o\
	   $(TFLAG)
TFLAG = -lpthread

webserver : $(OBJS) 
	$(CC) $^ -o $@

main.o : main.cc eventloop.h address.h httpserver.h httprequest.h httpresponse.h\
		 httpresponsefile.h
	$(CC) $(CFLAG) $< -o $@
	
httpserver.o : httpserver.cc httpserver.h buffer.h
	$(CC) $(CFLAG) $< -o $@
	
httpresponse.o : httpresponse.cc httpresponse.h buffer.h
	$(CC) $(CFLAG) $< -o $@

httpcontent.o : httpcontent.cc httpcontent.h httprequest.h httpparsestate.h
	$(CC) $(CFLAG) $< -o $@

httprequest.o : httprequest.cc httprequest.h httpparsestate.h
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
					 channel.h buffer.h httpcontent.h
	$(CC) $(CFLAG) $< -o $@
	
buffer.o : buffer.cc buffer.h
	$(CC) $(CFLAG) $< -o $@

.PHONY : clean
clean:
	rm *.o
