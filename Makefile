all: array ring_buffer deque string_view

array: examples/array.c
	cc -Wall -Wextra -I. -o $@ $<
ring_buffer: examples/ring_buffer.c
	cc -Wall -Wextra -I. -o $@ $<
deque: examples/deque.c
	cc -Wall -Wextra -I. -o $@ $<
string_view: examples/string_view.c
	cc -Wall -Wextra -I. -o $@ $<

clean:
	rm -f array ring_buffer deque string_view
