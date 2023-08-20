#include "libneblina.h"
#include <stdio.h>
#include <stdlib.h>

vector_t * vector_new( int len, data_type type, int initialize, void * data ) {
    vector_t * ret = (vector_t *) malloc( sizeof( vector_t ) );
    if (initialize && data == NULL) {
        if( type == T_INT ) {
            ret->value.i = (int *) malloc( len * sizeof( int ) );
        } else if( type == T_FLOAT )
            ret->value.f = (double *) malloc( len * sizeof( double ) ); 
        else if( type == T_COMPLEX )
            ret->value.f = (double *) malloc( 2 * len * sizeof( double ) );
        ret->externalData = 0;
    } else if (data != NULL) {
        ret->value.f = (double *)data;
        ret->externalData = 1;
    } else {
        ret->value.f = NULL;
        ret->externalData = 0;
    }
    
    ret->type      = type;
    ret->len       = len;
    ret->location  = LOCHOS;
    ret->extra       = NULL;
    return ret;
}

void vector_delete( vector_t * v ) {
//    printf("vector_delete 1\n");
    if (v->value.f != NULL && v->externalData == 0) {
//    printf("vector_delete 2\n");
        free (v->value.f);
//    printf("vector_delete 3\n");
    } else if (v->extra != NULL && v->externalData == 0) {
//    printf("vector_delete 2\n");
        free (v->extra);
//    printf("vector_delete 3\n");
    }
//    printf("vector_delete 4\n");
    free (v);
//    printf("vector_delete 5\n");
}

void vecreqhost( vector_t * v ) {
//    if (v->value.f != NULL) {
//    printf("vecreqhost 1\n");
//        free (v->value.f);
//    printf("vecreqhost 2\n");
//    }
    if (v->location == LOCHOS) return;
    v->location  = LOCHOS;
    v->value.f = v->extra;
    v->extra = NULL;
}

void vecreqdev ( vector_t * v ) {
//    if (v->extra != NULL) {
//    printf("vecreqdev 1\n");
//        free (v->extra);
//    printf("vecreqdev 2\n");
//    }
    if (v->location == LOCDEV) return;

    v->location  = LOCDEV;
    // printf("v->value.f %p\n",v->value.f);
    // printf("v->extra %p\n",v->extra);

    v->extra = v->value.f;
    v->value.f = NULL;
    // printf("v->value.f %p\n",v->value.f);
    // printf("v->extra %p\n",v->extra);
    // printf("vecreqdev %p\n",v->value.f);
}

