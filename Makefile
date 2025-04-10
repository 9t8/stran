objects = stran.o parse.o datum.o pair.o

stran.out : $(objects)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o stran.out $(objects)
stran.o : parse.h
parse.o : parse.h datum.h
datum.o : datum.h
pair.o : pair.h

.PHONY : clean
clean :
	$(RM) stran.out $(objects)
