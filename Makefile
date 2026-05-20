all: array ring_buffer deque

array: examples/array.c
	cc -Wall -Wextra -I. -o $@ $<
ring_buffer: examples/ring_buffer.c
	cc -Wall -Wextra -I. -o $@ $<
deque: examples/deque.c
	cc -Wall -Wextra -I. -o $@ $<

clean:
	rm -f array ring_buffer deque
