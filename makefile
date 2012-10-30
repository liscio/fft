all: slowft fastft

clean:
	rm slowft fastft

slowft:
	cc -o slowft fft_test.c fft_straightforward.c

fastft:
	cc -o fastft -DUSE_FFT fft_test.c fft_straightforward.c
