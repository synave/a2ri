GCC= gcc
CFLAGS = -W -Wall -std=c99 `pkg-config gsl --cflags` -pipe -O2 -Wall -W -D_REENTRANT
INCLUDE = -I.
LIBS = -lm -lglut -lGL -lGLU `pkg-config gsl --libs` -la2ri
OBJECTS = opengl.o main.o

TARGET = icp

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(GCC) $(CFLAGS) $(INCLUDE) $(OBJECTS) -o $(TARGET) $(LIBS)

%.o : %.c
	$(GCC) $(CFLAGS)  $(INCLUDE) -c $< -o $@

clean : 
	rm -f *~ *.o $(TARGET)

