CXX=g++
CXXFLAGS=-fPIC -shared

SOURCES=$(shell find ./sources -name "*.cpp")
all: libstudsys.so

libstudsys.so: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -lmysqlcppconn -o libstudsys.so

clean:
	$(RM) libstudsys.so
