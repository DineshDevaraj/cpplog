
#
# Author   : D.Dinesh
# Website  : www.techybook.com
# Email Id : dinesh@techybook.com
#
# Created  : 14 Jan 2015 - Wed
# Updated  : 15 Jan 2015 - Thu
# 
# Licence  : Refer the license file
#

FLAG=-g
GPP=/usr/bin/g++

OBJ=Logger.o

SRC=demo.cpp

demo.x : $(OBJ) $(SRC)
	$(GPP) $(FLAG) $(STD_LIB) $(OBJ) $(SRC) -o $@

%.o : %.cpp
	$(GPP) $(FLAG) -c $< -o $@

clean :
	rm -rf *.x *.o
