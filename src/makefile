GCC = g++
CFLAGS = -Wall -g -std=c++17

INCLUDES = -I/usr/local/include
LFLAGS = -L/usr/local/lib
LIBS = -lpthread -lssl -lcrypto
BOOST_LIBS = -D -lboost_system -lboost_context -lboost_coroutine -lboost_thread -lboost_json -lboost_log_setup -lboost_log

SRCS = BlankHttpConfig.cpp BlankHttpContext.cpp BlankHttpDefaultMiddleware.cpp BlankHttpHandleChain.cpp BlankHttpHandler.cpp BlankHttpLogger.cpp \
	   BlankHttpMiddleware.cpp BlankHttpRequestTarget.cpp BlankHttpResponse.cpp BlankHttpRouter.cpp BlankHttpRouteTable.cpp BlankHttpServer.cpp \
	   BlankHttpSession.cpp

OBJS = $(SRCS:.cpp=.o)

MAIN = libblankhttp.so

.PHONY: depend clean

all:	$(MAIN)
		@echo Blank HTTP library has been compiled.
		# $(RM) *.o

$(MAIN):$(OBJS)
		$(GCC) -o $(MAIN) -shared *.o 
.cpp.o:
		$(GCC) $(CFLAGS) $(INCLUDES) -c $< -o $@ -fPIC

clean:
		$(RM) *.o *~ $(MAIN)
install:
		mkdir -p /usr/local/lib
		mkdir -p /usr/local/include/blankhttp
		cp $(MAIN) /usr/local/lib
		cp *.h /usr/local/include/blankhttp/
		cp *.hpp /usr/local/include/blankhttp/

depend:	$(SRCS)
		makedepend $(INCLUDES) $^
