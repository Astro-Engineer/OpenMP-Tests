CC = gcc
CFLAGS = mpicc


TARGETS = p1 p2_1 p2_2 p2_3

all: $(TARGETS)

p1: p1.c
	$(CFLAGS) -o $@ $< $(LDFLAGS)

p2_1: p2_1.c
	$(CFLAGS) -o $@ $< $(LDFLAGS)

p2_2: p2_2.c
	$(CFLAGS) -o $@ $< $(LDFLAGS)

p2_3: p2_3.c
	$(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGETS)
