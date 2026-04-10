CC = gcc
DEBUG = -D_DEBUG -g
INCLUDE = -I. -I../include/
CCFLAGS = -Wall -std=c99 -pipe -O2 $(DEBUG) -D_REENTRANT -fPIC
LIBS = -Wl,-E -ldl -rdynamic -shared -lm -lgsl -lgslcblas -lpthread
#CCFLAGS = -std=c99
#LIBS = -lm -lgsl -lgslcblas -lpthread


TARGET_O = debug.o matrix.o util.o point.o vector.o geometry.o vertex.o edge.o face.o hashtable.o space_partition.o skeleton.o model.o io.o boundingbox.o conversion.o subdivision.o topology.o shapematching.o quality.o overlap.o  icp.o triangulation.o graph.o geodesique.o epaississement.o 

TARGET_SO = liba2ri_dyn.so


all : $(TARGET_O) $(TARGET_SO)
	ar r liba2ri.a *.o
	mv *.so *.a ../lib/

%.so :
	$(CC) $(INCLUDE) $(TARGET_O) $(DEBUG) -o $@ $(LIBS)

%.o : %.c
	$(CC) $(CCFLAGS) $(INCLUDE) -c $< -o $@

clean :
	rm -f *~ *.o $(TARGET)
	rm -f ../lib/*.a ../lib/*.so

