OBJECTS = LuaInterface.o Abstract.o Config.o Cell.o Test.o \
		ObjectManger.o Transform.o Common.o Map.o Session.o CameraObject.o \
		Debug.o Type_Abstract.o Type_Object.o Anim.o ObjectArrayImpl.o \
		Type_Anim.o Object.o LuaUtils.o Type_Techno.o Techno.o Physics.o Pathfinder.o
OUTPUT=gamemk.exe

RESFILE = $(SOURCE_PATH)ver.rc
RESFILE_OUTPUT = ver.res

BUILD_PATH = ./Build/
BINARY_PARH = .
SOURCE_PATH = ./

CXXFLAGS=-Wall -std=c++11 -O2
LDFLAGS=-Wall -Wl,-Bstatic -static -lpython27 -llua52 \
		-lsfml-window -Wl,-Bstatic -lsfml-graphics -lsfml-system \
		 -lBulletDynamics -lBulletCollision -lLinearMath
# LDFLAGS=-Wall -ggdb3 -Wl,-Bstatic -static -lpython27 -llua52 \
# 		-lsfml-window -Wl,-Bstatic -lsfml-graphics -lsfml-system \
# 		 -lBulletDynamics -lBulletCollision -lLinearMath 
RESCMP = windres

_OBJECTS=$(addprefix $(BUILD_PATH),$(OBJECTS))
_RESFILE_OUTPUT=$(addprefix $(BUILD_PATH),$(RESFILE_OUTPUT))

LINK_OBJECTS = $(_OBJECTS) $(_RESFILE_OUTPUT)

main: ready objects res link final

run:
	@gamemk.exe

$(BUILD_PATH)%.o: $(SOURCE_PATH)%.cpp
	@echo Compiling $< ...
	@$(CXX) -c $<		$(CXXFLAGS) -o		$@

ready:

final:
	@echo Build finished.

objects: $(_OBJECTS)

res: $(RESFILE)
	@echo Compiling Resource file...
	@$(RESCMP) $? -O coff -o $(_RESFILE_OUTPUT)

link:
	@echo Linking...
	@$(CXX) -o $(BINARY_PARH)/$(OUTPUT) $(LINK_OBJECTS) $(LDFLAGS)