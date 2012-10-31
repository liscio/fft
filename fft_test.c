#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include <stdbool.h>
#include <float.h>
#include <time.h>
#include "fft_straightforward.h"

#define TEST_EPSILON 0.000000001
#define TEST_SIZE 1024
#define SPEED_TEST_ITERATIONS 128
//#define LOG_OUTPUT

bool compare_vectors_complex( const complex double *vecA, const complex double *vecB, const unsigned int len, const double eps ) {
    for ( unsigned int i = 0; i < len; i++ ) {
        if ( cabs( vecA[i] - vecB[i] ) > eps ) {
            printf( "Vector mismatch at index %d. %f+%fi vs %f+%fi.\n", i, creal(vecA[i]), cimag(vecA[i]), creal(vecB[i]), cimag(vecB[i]) );
            return false;
        }
    }
    return true;
}

void print_complex( const complex double i ) {
    printf( "%f+%fi", creal(i), cimag(i) );
}

void print_vector_complex( const complex double *vec, const unsigned int len ) {
    for ( unsigned int i = 0; i < len; i++ ) {
        print_complex( vec[i] );
        printf( ", " );
    }
    printf( "\n" );
}

complex double *create_complex_vector( const unsigned int length ) {
    return (complex double*)calloc( sizeof(complex double), length );
}

complex double *create_random_complex_vector( const unsigned int length ) {
    complex double *returnVec = create_complex_vector( length );
    
    for ( unsigned int i = 0; i < length; i++ ) {
        complex double real = (double)rand() / (double)RAND_MAX;
        complex double imag = (double)rand() / (double)RAND_MAX;
        returnVec[i] = real + ( imag * I );
    }

    return returnVec;
}

void add_vectors_complex( complex double *out, const complex double *in1, const complex double *in2, const unsigned int length ) {
    for ( unsigned int i = 0; i < length; i++ ) {
        out[i] = in1[i] + in2[i];
    }
}

void scale_vector_complex( complex double *out, const complex double *in, const complex double scalar, const unsigned int inLength ) {
    for ( unsigned int i = 0; i < inLength; i++ ) {
        out[i] = in[i] * scalar;
    }
}

int main( int argc, char *argv[] ) {

    srand( time( NULL ) );

    printf( "Running delta function check...\n" );
    {
        // Delta function is just 1, 0, 0, ..., 0
        complex double *delta = create_complex_vector( TEST_SIZE );
        delta[0] = 1;

        complex double *deltaFFT = FFT( delta, TEST_SIZE );

        // Result of a FFT of a delta function is 1, 1, 1, ..., 1
        complex double *deltaCheck = create_complex_vector( TEST_SIZE );
        for ( unsigned int i = 0; i < TEST_SIZE; i++ ) {
            deltaCheck[i] = 1.0;
        }


        if ( !compare_vectors_complex( deltaFFT, deltaCheck, TEST_SIZE, TEST_EPSILON ) ) {
            printf( "FFT does not compute delta result correctly.\n" );
            return -1;
        }

        free( deltaCheck );
        free( delta );
    }

    complex double *input1 = create_random_complex_vector( TEST_SIZE );
    complex double *input2 = create_random_complex_vector( TEST_SIZE );

    // Assert that FFT(input1) + FFT(input2) = FFT( input1 + input2 );
    printf( "Running Addition linearity check...\n" );
    {
        complex double *fft1 = FFT( input1, TEST_SIZE );
        complex double *fft2 = FFT( input2, TEST_SIZE );

        complex double *addedInputs = create_complex_vector( TEST_SIZE );
        add_vectors_complex( addedInputs, input1, input2, TEST_SIZE );

        complex double *fftOfAddedInputs = FFT( addedInputs, TEST_SIZE );

        complex double *addedFFTs = create_complex_vector( TEST_SIZE );
        add_vectors_complex( addedFFTs, fft1, fft2, TEST_SIZE );

        if ( !compare_vectors_complex( addedFFTs, fftOfAddedInputs, TEST_SIZE, TEST_EPSILON ) ) {
            printf( "FFT does not satisfy linearity. Addition.\n" );
            return -1;
        }

        free( addedFFTs );
        free( fftOfAddedInputs );
        free( addedInputs );
        free( fft2 );
        free( fft1 );
    }

    // Assert that C * FFT(input1) = FFT( C * input1 );
    printf( "Running Multiplication linearity check...\n" );
    {
        complex double scalar = 42;
        complex double *fft1 = FFT( input1, TEST_SIZE );

        complex double *scaledInput = create_complex_vector( TEST_SIZE );
        scale_vector_complex( scaledInput, input1, scalar, TEST_SIZE );

        complex double *fftOfScaledInput = FFT( scaledInput, TEST_SIZE );

        complex double *scaledFFT = create_complex_vector( TEST_SIZE );
        scale_vector_complex( scaledFFT, fft1, scalar, TEST_SIZE );

        if ( !compare_vectors_complex( scaledFFT, fftOfScaledInput, TEST_SIZE, TEST_EPSILON ) ) {
            printf( "FFT does not satisfy linearity. Multiplication.\n" );
            return -1;
        }

        free( scaledFFT );
        free( fftOfScaledInput );
        free( scaledInput );
        free( fft1 );
    }

    // And now, just run the FFT a whole bunch of times on the inputs
    printf( "Running speed test...\n" );
    {
        for ( unsigned int i = 0; i < SPEED_TEST_ITERATIONS; i++ ) {
            complex double *fft1 = FFT( input1, TEST_SIZE );
            complex double *fft2 = FFT( input2, TEST_SIZE );
            free( fft2 );
            free( fft1 );
        }
    }

    free( input2 );
    free( input1 );


    return 0;
}
