#include "libneblina.h"
#include <stdio.h>
#include <stdlib.h>

vector_t * vector_new( int len, data_type type, int initialize ) {
    vector_t * ret = (vector_t *) malloc( sizeof( vector_t ) );
    if (initialize) {
        if( type == T_FLOAT )
            ret->value.f = (double *) malloc( len * sizeof( double ) ); 
        else if( type == T_COMPLEX )
            ret->value.f = (double *) malloc( 2 * len * sizeof( double ) );
    } else {
        ret->value.f = NULL;
    }
    
    ret->type      = type;
    ret->len       = len;
    ret->location  = LOCHOS;
    ret->extra       = NULL;
    return ret;
}

void vector_delete( vector_t * v ) {
    if (v->value.f != NULL) {
        free (v->value.f);
    } else if (v->extra != NULL) {
        free (v->extra);
    }
    free (v);
}

void vecreqhost( vector_t * v ) {
    if (v->location == LOCHOS) return;
    v->location  = LOCHOS;
    v->value.f = v->extra;
    v->extra = NULL;
}

void vecreqdev ( vector_t * v ) {
    if (v->location == LOCDEV) return;
    v->location  = LOCDEV;
    v->extra = v->value.f;
    v->value.f = NULL;
}

