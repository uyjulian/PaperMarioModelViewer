
EXENAME=3DTest-cmd
CXXFLAGS=-g -Wall -DSFML_DYNAMIC -I. 

.PHONY: clean

all: $(EXENAME)

$(EXENAME): src/*
	g++ $(CXXFLAGS) src/main.cpp -o $@ $(LDFLAGS) -lsfml-graphics -lsfml-window -lsfml-system -framework OpenGL
	rm -rf $(EXENAME).dSYM
	
clean:
	rm -rf $(EXENAME) 
	