CC = g++-11
CFLAGS = --sysroot=`xcrun --show-sdk-path`

ALL: solver.o inputUtil.o
	$(CC) $(CFLAGS) -o IPOsolver solver.o inputUtil.o

solver.o: solver.cpp
	$(CC) $(CFLAGS) -o solver.o -c solver.cpp

inputUtil.o: inputUtility.cpp inputUtility.hpp
	$(CC) $(CFLAGS) -o inputUtil.o -c inputUtility.cpp