CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		Renderer.o

LIBS =		-lSDL -lGL

TARGET =	Renderer

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
