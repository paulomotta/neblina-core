#include "libneblina.h"
#include <stdio.h>
#include <stdlib.h>

matrix_t * matrix_new( int nrow, int ncol, data_type type ) {
    matrix_t * ret = (matrix_t *) malloc( sizeof( matrix_t ) );
    
    if( type == T_INT ) {
        ret->value.i = (int *) malloc( nrow * ncol * sizeof( int ) );
    } else if( type == T_FLOAT ) {
        ret->value.f = (double *) malloc( nrow * ncol * sizeof( double ) );
    } else if( type == T_COMPLEX ) {
        ret->value.f = (double *) malloc( 2 * nrow * ncol * sizeof( double ) );
    }

    
    ret->type       = type;
    ret->nrow       = nrow;
    ret->ncol       = ncol;
    ret->location   = LOCHOS;   
    ret->mem        = NULL;   
    return ret;
}

void matrix_delete( matrix_t * m ) {
    if (m->mem != NULL) {
        cl_int status = clReleaseMemObject( m->mem );
        CLERR
    }
    if (m->value.f != NULL) {
        free (m->value.f);
    }
    free (m);
}

void matreqhost( matrix_t * v ) {
    if( v->location != LOCHOS ) {
        object_t * in[1]; 
        object_t o; vvalue( o ) = v; o.type = T_MATRIX;
        in[0] = &o; 
        movetohost( (void **) in, NULL );
    }
}
void matreqdev ( matrix_t * v ) {
    if( v->location != LOCDEV ) {
        object_t * in[1]; 
        object_t o; vvalue( o ) = v; o.type = T_MATRIX;
        in[0] = &o; 
        movetodev( (void **) in, NULL );
    }
}