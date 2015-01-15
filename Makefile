
GCC=/usr/bin/g++
FLAG=-g

OBJ=Logger.o

SRC=demo.cpp

demo.x : $(OBJ) $(SRC)
	$(GCC) $(FLAG) $(STD_LIB) $(OBJ) $(SRC) -o $@

%.o : %.cpp
	$(GCC) $(FLAG) -c $< -o $@

clean :
	rm -rf *.x *.o
