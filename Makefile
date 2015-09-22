CC = gcc
CXX = g++
#libswscale
CFLAGS+=`pkg-config --cflags glfw3 glu`

CXXFLAGS+=$(CFLAGS)

LDFLAGS+= `pkg-config --static --libs glfw3 glu`

INCLUDES+=

TARGET = gl4_test
SRCS = main
OBJS = $(addsuffix .o,$(SRCS))

#リンクの依存関係か今のところわからない　そのため、Whole-archiveが必要になる
$(TARGET):  $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)
#-Wl,--no-whole-archive
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -g -c $<

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)

.PHONY: deploy
deploy:
	./deploy $(TARGET)

