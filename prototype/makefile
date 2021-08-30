GCC = g++
CFLAGS = -Wall -g -std=c++17

INCLUDES = -I/usr/local/include
LFLAGS = -L/usr/local/lib
LIBS = -lpthread -lssl -lcrypto
BOOST_LIBS = -D -lboost_system -lboost_context -lboost_coroutine -lboost_thread -lboost_json -lboost_log_setup -lboost_log

SRCS = main.cpp BlankHttpClient.cpp BlankHttpConfig.cpp BlankHttpContext.cpp BlankHttpDefaultMiddleware.cpp BlankHttpHandleChain.cpp BlankHttpHandler.cpp \
	   BlankHttpLogger.cpp BlankHttpMiddleware.cpp BlankHttpRequestTarget.cpp BlankHttpRouter.cpp BlankHttpRouteTable.cpp BlankHttpServer.cpp \
	   BlankHttpSession.cpp HelloUserHandler.cpp HelloUserRepeatMiddleware.cpp HelloWorldHandler.cpp BingMainPageHandler.cpp

OBJS = $(SRCS:.cpp=.o)

MAIN = blank_server

.PHONY: depend clean

all:	$(MAIN)
		@echo Blank HTTP Server has been compiled.
		# $(RM) *.o

$(MAIN):$(OBJS)
		$(GCC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS) $(BOOST_LIBS)
.cpp.o:
		$(GCC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
		$(RM) *.o *~ $(MAIN)
depend:	$(SRCS)
		makedepend $(INCLUDES) $^
