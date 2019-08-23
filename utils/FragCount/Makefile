CXXFLAGS = -Wall -g -Iinclude -I/usr/include/python2.7 -I/home/gary/projects/seacas/packages/seacas/libraries/exodus/include -I/home/gary/projects/seacas/include -I/home/gary/projects/seacas/packages/seacas/libraries/exodus/sierra
CXX = g++

src = $(wildcard src/*.C)
obj = $(src:.C=.o)

FragCount: $(obj)
	$(CXX) -o $@ $^ -L/home/gary/projects/seacas/lib -lpython2.7 -lexodus -lnetcdf -lpthread

.PHONY: clean

clean:
	rm -f src/*.o
