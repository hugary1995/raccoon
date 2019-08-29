CXX = g++
CXXFLAGS = -Wall -g -Iinclude -I/usr/include/python2.7 -I/opt/moose/seacas/include -Ieigen
LDFLAGS = -L/opt/moose/seacas/lib
LDLIBS = -lpython2.7 -lexodus -lnetcdf

src = $(wildcard src/*.C)
obj = $(src:.C=.o)

FragCount: $(obj)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

.PHONY: clean

clean:
	rm -f src/*.o
