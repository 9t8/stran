objects = parse.o datum.o pair.o

stran: $(objects)
parse.o: parse.h datum.h
datum.o: datum.h
pair.o: pair.h

.PHONY: clean
clean:
	$(RM) stran $(objects)
