CC=gcc
CFLAGS=-I. -g -O0
LDFLAGS=-pthread

all: tracker run

run:
	@cp config.ini.example config.ini
	@./bin/tracker

gdb:
	@cp config.ini.example config.ini
	@gdb ./bin/tracker

tracker: src/tracker.c src/connection_handler.c src/files.c src/port.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	@if [ ! -d "./bin" ]; then \
			mkdir ./bin; \
		fi; \
		
	@if [ -f "./tracker" ]; then \
		mv ./tracker ./bin; \
	fi

#faketracker: src/faketracker.c src/connection_handler.c

clean:
	rm bin/*