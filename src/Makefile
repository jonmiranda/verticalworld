all:
	g++ -pedantic -Wno-deprecated *.cpp *.cc -DGL_GLEXT_PROTOTYPES -lglut -lGL -lGLU

osx:
	g++ -pedantic -Wno-deprecated *.cpp *.cc -framework GLUT -framework OpenGL

clean:
	rm -f *~ *.o a.out
