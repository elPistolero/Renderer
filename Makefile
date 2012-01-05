CXX = g++

CXXFLAGS =	-DGL_GLEXT_PROTOTYPES -O2 -g -Wall -fmessage-length=0

OBJS =		Renderer.o SceneGraph.o SceneNode.o ShaderHelper.o

LIBS =		-lSDL -lGL 

TARGET =	Renderer

$(TARGET):	$(OBJS)
	$(CXX) $(OBJS) $(LIBS) -o $(TARGET)
	
Renderer.o: Renderer.cpp
	$(CXX) $(CXXFLAGS) -c Renderer.cpp

SceneGraph.o: SceneGraph/SceneGraph.cpp
	$(CXX) $(CXXFLAGS) -c SceneGraph/SceneGraph.cpp
	
SceneNode.o: SceneGraph/SceneNode.cpp
	$(CXX) $(CXXFLAGS) -c SceneGraph/SceneNode.cpp
	
ShaderHelper.o: ShaderHelper/ShaderHelper.cpp
	$(CXX) $(CXXFLAGS) -c ShaderHelper/ShaderHelper.cpp
	
all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
