GCC= gcc
CFLAGS = -W -Wall -std=c99 -pipe -O2 -Wall -W -D_REENTRANT
INCLUDE = -I.
LIBS = -lgsl -lgslcblas -lm -lglut -lGL -lGLU -la2ri
OBJECTS = opengl.o main.o

TARGET = geodesique

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(GCC) $(CFLAGS) $(INCLUDE) $(OBJECTS) -o $(TARGET) $(LIBS)

%.o : %.c
	$(GCC) $(CFLAGS)  $(INCLUDE) -c $< -o $@

clean : 
	rm -f *~ *.o $(TARGET)

