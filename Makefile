CXX = g++

ODIR = Build/Objects

BIN = Build/bin

CXXFLAGS =	-DGL_GLEXT_PROTOTYPES -g -Wall -fmessage-length=0

_OBJS =		SceneGraph.o SceneNode.o SceneCamera.o SceneNodeTriangleMesh.o GLSLProgram.o OBJImporter.o Renderer.o

OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

LIBS =		-lSDL -lGL -lGLEW

TARGET =	$(BIN)/Renderer

$(TARGET):	$(OBJS)
	$(CXX) $(OBJS) $(LIBS) -o $(TARGET)
	
$(ODIR)/SceneGraph.o: SceneGraph/SceneGraph.cpp
	@mkdir -p $(ODIR)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -c SceneGraph/SceneGraph.cpp -o $@
	
$(ODIR)/SceneNode.o: SceneGraph/SceneNode.cpp
	@mkdir -p $(ODIR)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -c SceneGraph/SceneNode.cpp -o $@
	
$(ODIR)/SceneCamera.o: SceneGraph/SceneCamera.cpp
	@mkdir -p $(ODIR)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -c SceneGraph/SceneCamera.cpp -o $@
	
$(ODIR)/SceneNodeTriangleMesh.o: SceneGraph/SceneNodeTriangleMesh.cpp
	@mkdir -p $(ODIR)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -c SceneGraph/SceneNodeTriangleMesh.cpp -o $@
	
$(ODIR)/GLSLProgram.o: GLSLProgram/GLSLProgram.cpp
	@mkdir -p $(ODIR)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -c GLSLProgram/GLSLProgram.cpp -o $@
	
$(ODIR)/OBJImporter.o: Importer/OBJImporter.cpp
	@mkdir -p $(ODIR)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -c Importer/OBJImporter.cpp -o $@

$(ODIR)/Renderer.o: Renderer.cpp
	@mkdir -p $(ODIR)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -c Renderer.cpp -o $@

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
