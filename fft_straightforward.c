#include "fft_straightforward.h"
#include <stdlib.h>
#include <complex.h>
#include <math.h>

static complex double *FFT_recurse( complex double *x, int N, int skip ) {
    complex double *X = (complex double*)malloc( sizeof(complex double) * N );
    complex double *O, *E;
    
    // We've hit the scalar case, and copy the input to the output.
    if ( N == 1 ) {
        X[0] = x[0];
        return X;
    }
    
    E = FFT_recurse( x, N/2, skip * 2 );
    O = FFT_recurse( x + skip, N/2, skip * 2 );

    for ( int k = 0; k < N / 2; k++ ) {
        O[k] = ( cexp( 2.0 * I * M_PI * k / N ) * O[k] );
    }
    
    // While E[k] and O[k] are of length N/2, and X[k] is of length N, E[k] and
    // O[k] are periodic in k with length N/2. See p.609 of Numerical Recipes
    // in C (3rd Ed, 2007). [CL]
    for ( int k = 0; k < N / 2; k++ ) {
        X[k] = E[k] + O[k];
        X[k + N/2] = E[k] + O[k];
    }
    
    free( O );
    free( E );
    
    return X;
}

static complex double *FFT_math( complex double *x, int N ) {
    complex double *X = (complex double *)malloc( sizeof(complex double) * N );
    for ( int k = 0; k < N; k++ ) {
        X[k] = 0;
        for ( int j = 0; j < N; j++ ) {
            X[k] += x[j] * cexp( 2.0 * I * M_PI * j * k / N );
        }
    }
    return X;
}

complex double *FFT( complex double *x, int N ) {
#if defined( USE_FFT )
    return FFT_recurse( x, N, 1 );
#else
    return FFT_math( x, N );
#endif
}
