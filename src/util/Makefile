.PHONY: clean cleanall

APP = libutil.a
OBJS = cmdline.o file.o thread.o string.o time.o cfg.o regex.o lexicalcast.o out.o

$(APP): $(OBJS)
	$(MAKELIB) $(APP) $(OBJS)
	cp $(APP) ../

cfg.o: cfg.cpp cfg.hpp base.hpp base/base.hpp base/memory.hpp base/in.hpp \
 base/out.hpp base/baseclass.hpp base/lexicalcast.hpp string.hpp file.hpp
	$(CXX) $(INCLUDE) $(CXXFLAGS) -c cfg.cpp
cmdline.o: cmdline.cpp cmdline.hpp base.hpp base/base.hpp base/memory.hpp \
 base/in.hpp base/out.hpp base/baseclass.hpp base/lexicalcast.hpp \
 string.hpp collection.hpp regex.hpp
	$(CXX) $(INCLUDE) $(CXXFLAGS) -c cmdline.cpp
file.o: file.cpp file.hpp base.hpp base/base.hpp base/memory.hpp \
 base/in.hpp base/out.hpp base/baseclass.hpp base/lexicalcast.hpp
	$(CXX) $(INCLUDE) $(CXXFLAGS) -c file.cpp
regex.o: regex.cpp regex.hpp base.hpp base/base.hpp base/memory.hpp \
base/in.hpp base/out.hpp base/baseclass.hpp base/lexicalcast.hpp \
 regex/deelx.h
	$(CXX) $(INCLUDE) $(CXXFLAGS) -c regex.cpp
string.o: string.cpp string.hpp base.hpp base/base.hpp base/memory.hpp \
 base/in.hpp base/out.hpp base/baseclass.hpp base/lexicalcast.hpp
	$(CXX) $(INCLUDE) $(CXXFLAGS) -c string.cpp
thread.o: thread.cpp thread.hpp base.hpp base/base.hpp base/memory.hpp \
 base/in.hpp base/out.hpp base/baseclass.hpp base/lexicalcast.hpp
	$(CXX) $(INCLUDE) $(CXXFLAGS) -c thread.cpp
time.o: time.cpp time.hpp base.hpp base/base.hpp base/memory.hpp \
 base/in.hpp base/out.hpp base/baseclass.hpp base/lexicalcast.hpp
	$(CXX) $(INCLUDE) $(CXXFLAGS) -c time.cpp
lexicalcast.o: base/lexicalcast.cpp base/base.hpp base/memory.hpp \
 base/in.hpp base/out.hpp base/baseclass.hpp base/lexicalcast.hpp
	$(CXX) $(INCLUDE) $(CXXFLAGS) -c base/lexicalcast.cpp
out.o: base/out.cpp base/base.hpp base/memory.hpp base/in.hpp \
 base/out.hpp base/baseclass.hpp base/lexicalcast.hpp base/../string.hpp \
 base/../base.hpp base/../base/base.hpp base/../time.hpp
	$(CXX) $(INCLUDE) $(CXXFLAGS) -c base/out.cpp

clean:
	rm $(OBJS)
cleanall:
	rm $(OBJS) $(APP)
