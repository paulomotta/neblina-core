#include "libneblina.h"
#include <stdio.h>
#include <stdlib.h>

vector_t * vector_new( int len, data_type type ) {
    vector_t * ret = (vector_t *) malloc( sizeof( vector_t ) );
    int i = 0;
    
    if( type == T_INT ) {
        ret->value.i = (int *) malloc( len * sizeof( int ) );
    } else if( type == T_FLOAT )
        ret->value.f = (double *) malloc( len * sizeof( double ) ); 
    else if( type == T_COMPLEX )
        ret->value.f = (double *) malloc( 2 * len * sizeof( double ) );
    else if( type == T_STRING ) {
        ret->value.s = (void **) malloc( len * sizeof( char * ) );
        for(i=0; i < len; i++ ) {
            ret->value.s[i] = (void *) malloc( sizeof( char ) );
            char * tmp = (char *)ret->value.s[i];
            tmp[0] = 0;
        }  
    }         
    
    ret->type      = type;
    ret->len       = len;
    ret->location  = LOCHOS;
    ret->extra       = NULL;
    return ret;
}

void vector_delete( vector_t * v ) {
//    printf("vector_delete 1\n");
    if (v->value.f != NULL) {
//    printf("vector_delete 2\n");
        free (v->value.f);
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

    v->value.f = v->extra;
}

void vecreqdev ( vector_t * v ) {
//    if (v->extra != NULL) {
//    printf("vecreqdev 1\n");
//        free (v->extra);
//    printf("vecreqdev 2\n");
//    }
    v->extra = v->value.f;
    
}

