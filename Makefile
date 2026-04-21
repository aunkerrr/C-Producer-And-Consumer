CFLAGS = -Wall -Wextra -O2 -pthread

.PHONY: all clean
all: prodcons-np-nc

consumer.o: consumer.c
	gcc -c consumer.c $(CFLAGS)

producer.o: producer.c
	gcc -c producer.c $(CFLAGS)

prodcons-np-nc: prodcons-np-nc.c producer.o consumer.o
	gcc prodcons-np-nc.c producer.o consumer.o -o prodcons-np-nc $(CFLAGS)

clean:
	rm -rf *.o prodcons-np-nc