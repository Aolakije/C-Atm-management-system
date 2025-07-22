# List of object files (added db.o)
objects = src/main.o src/system.o src/auth.o src/db.o

# Final executable
atm: $(objects)
	cc -o atm $(objects) -lncurses -lsqlite3

# Rules to build object files
src/main.o: src/main.c src/header.h src/db.h
	cc -c src/main.c -o src/main.o

src/system.o: src/system.c src/header.h
	cc -c src/system.c -o src/system.o

src/auth.o: src/auth.c src/header.h
	cc -c src/auth.c -o src/auth.o

src/db.o: src/db.c src/db.h
	cc -c src/db.c -o src/db.o

# Clean up
clean:
	rm -f $(objects) atm
