CC = g++
CFLAG = -Wall -c
OBJS = main.o httpserver.o httpresponse.o httpcontent.o httprequest.o\
	   tcpserver.o acceptor.o\
	   channel.o eventloop.o epoller.o tcpconnection.o\
	   eventloopthreadpool.o eventloopthread.o thread.o buffer.o\
	   $(TFLAG)
TFLAG = -lpthread
NDEBUG = -DNDEBUG
BUILD = -O3 -DNDEBUG

webserver : $(OBJS) 
	$(CC) $^ -o $@ $(BUILD)

main.o : main.cc eventloop.h address.h httpserver.h httprequest.h httpresponse.h\
		 httpresponsefile.h
	$(CC) $(CFLAG) $< -o $@ $(BUILD)
	
httpserver.o : httpserver.cc httpserver.h buffer.h
	$(CC) $(CFLAG) $< -o $@ $(BUILD)
	
httpresponse.o : httpresponse.cc httpresponse.h buffer.h
	$(CC) $(CFLAG) $< -o $@ $(BUILD)

httpcontent.o : httpcontent.cc httpcontent.h httprequest.h httpparsestate.h
	$(CC) $(CFLAG) $< -o $@ $(BUILD)

httprequest.o : httprequest.cc httprequest.h httpparsestate.h
	$(CC) $(CFLAG) $< -o $@ $(BUILD)

tcpserver.o : tcpserver.cc tcpserver.h acceptor.h tcpconnection.h\
			  eventloopthreadpool.h
	$(CC) $(CFLAG) $< -o $@ $(BUILD)
	
acceptor.o : acceptor.cc acceptor.h address.h channel.h
	$(CC) $(CFLAG) $< -o $@ $(BUILD)
	
channel.o : channel.cc channel.h
	$(CC) $(CFLAG) $< -o $@ $(BUILD)

eventloop.o : eventloop.cc eventloop.h channel.h mutex.h
	$(CC) $(CFLAG) $< -o $@ $(BUILD)

eventloopthreadpool.o : eventloopthreadpool.cc eventloopthreadpool.h eventloopthread.h
	$(CC) $(CFLAG) $< -o $@ $(BUILD)
	
eventloopthread.o : eventloopthread.cc eventloopthreadpool.h mutex.h\
					condition.h eventloop.h
	$(CC) $(CFLAG) $< -o $@ $(BUILD)

thread.o : thread.cc thread.h latch.h
	$(CC) $(CFLAG) $< -o $@ $(BUILD)
	
epoller.o : epoller.cc epoller.h channel.h
	$(CC) $(CFLAG) $< -o $@ $(BUILD)
	
tcpconnection.o : tcpconnection.cc tcpconnection.h \
					 channel.h buffer.h httpcontent.h
	$(CC) $(CFLAG) $< -o $@ $(BUILD)
	
buffer.o : buffer.cc buffer.h
	$(CC) $(CFLAG) $< -o $@ $(BUILD)

.PHONY : clean

debug: $(OBJS) 
	$(CC) $^ -o webserver
	
release: webserver

clean:
	rm *.o
