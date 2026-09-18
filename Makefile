all: src obj bin tmp server client
server: bin/monitor
client: bin/tracer

bin/monitor: obj/monitor.o obj/estruturas.o
	gcc -g obj/monitor.o obj/estruturas.o -o bin/monitor

obj/monitor.o: src/monitor.c
	gcc -Wall -g -c src/monitor.c -o obj/monitor.o

obj/estruturas.o: src/estruturas.c
	gcc -Wall -g -c src/estruturas.c -o obj/estruturas.o

bin/tracer: obj/tracer.o obj/estruturas.o
	gcc -g obj/tracer.o obj/estruturas.o -o bin/tracer

obj/tracer.o: src/tracer.c
	gcc -Wall -g -c src/tracer.c -o obj/tracer.o

clean:
	rm -f obj/* tmp/* bin/{tracer,monitor,FIFO}