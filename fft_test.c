#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include "fft_straightforward.h"

#define TEST_SIZE 1024

int main( int argc, char *argv[] ) {

    complex double *x = (complex double *)malloc( TEST_SIZE * sizeof(complex double) );

    // Build a dirac delta function
    x[0] = 1;
    for ( int i = 1; i < TEST_SIZE; i++ ) {
        x[i] = 0;
    }

#if defined( LOG_OUTPUT )
    printf( "Input: \n" );
    for ( int i = 0; i < TEST_SIZE; i++ ) {
        printf( "%f+%fi, ", creal(x[i]), cimag(x[i]) );
    }
    printf( "\n" );
    printf( "\n" );
#endif //defined( LOG_OUTPUT )

    complex double *output;
    for ( int i = 0; i < 100; i++ ) {
        output = FFT( x, TEST_SIZE );
#if defined( LOG_OUTPUT )
        printf( "Output: \n" );
        for ( int i = 0; i < TEST_SIZE; i++ ) {
            printf( "%f+%fi, ", creal(output[i]), cimag(output[i]) );
        }
        printf( "\n" );
#endif //defined( LOG_OUTPUT )
        free( output );
    }

    return 0;
}
