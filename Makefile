CXX = g++

ODIR = Build/Objects

BIN = Build/bin

CXXFLAGS =	-DGL_GLEXT_PROTOTYPES -O2 -g -Wall -fmessage-length=0

_OBJS =		Renderer.o SceneGraph.o SceneNode.o SceneNodeVAO.o ShaderHelper.o

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

LIBS =		-lSDL -lGL -lGLEW

TARGET =	$(BIN)/Renderer

$(TARGET):	$(OBJS)
	$(CXX) $(OBJS) $(LIBS) -o $(TARGET)
	
$(ODIR)/Renderer.o: Renderer.cpp
	@mkdir -p $(ODIR)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -c Renderer.cpp -o $@

$(ODIR)/SceneGraph.o: SceneGraph/SceneGraph.cpp
	@mkdir -p $(ODIR)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -c SceneGraph/SceneGraph.cpp -o $@
	
$(ODIR)/SceneNode.o: SceneGraph/SceneNode.cpp
	@mkdir -p $(ODIR)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -c SceneGraph/SceneNode.cpp -o $@
	
$(ODIR)/SceneNodeVAO.o: SceneGraph/SceneNodeVAO.cpp
	@mkdir -p $(ODIR)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -c SceneGraph/SceneNodeVAO.cpp -o $@
	
$(ODIR)/ShaderHelper.o: ShaderHelper/ShaderHelper.cpp
	@mkdir -p $(ODIR)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -c ShaderHelper/ShaderHelper.cpp -o $@

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
