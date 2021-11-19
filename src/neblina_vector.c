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
    
    if (v->extra != NULL) {
        cl_int status = clReleaseMemObject( (cl_mem)v->extra );
        CLERR
    }
    if (v->value.f != NULL) {
        free (v->value.f);
    }
    free (v);
}

void vecreqhost( vector_t * v ) {
    cl_int status;
    if( v->location != LOCHOS ) {
        size_t size_type = (clinfo.fp64) ? sizeof(double) : sizeof(float);
        if (v->value.f == NULL) {
            if( v->type == T_FLOAT ) {
                v->value.f = (double *) malloc( v->len * sizeof(double) );
            } else {
                v->value.f = (double *) malloc( v->len * COMPLEX_SIZE );
            }
        }
                    
        v->location = LOCHOS;
        int len = (v->type == T_COMPLEX) ? (2*v->len) : (v->len);
        if(clinfo.fp64) {   
            status = clEnqueueReadBuffer (clinfo.q, (cl_mem)v->extra, CL_TRUE, 0, len*size_type, v->value.f, 0, NULL, NULL);
            CLERR
        } else {
            int i;
            float * tmp = (float *) malloc( sizeof(float) * len );               
            status = clEnqueueReadBuffer (clinfo.q, (cl_mem)v->extra, CL_TRUE, 0, len*size_type,tmp, 0, NULL, NULL);
            CLERR
            #pragma omp parallel for
            for( i = 0; i < len; i++){ v->value.f[i] = tmp[i]; /*printf("V -> %f\n", tmp[i]);*/ }
            free( tmp );
        }
        clReleaseMemObject( (cl_mem)v->extra );
        CLERR  
        v->extra = NULL;
    }
}

void vecreqdev ( vector_t * v ) {
    if( v->location != LOCDEV ) {
        object_t * in[1]; 
        object_t o; vvalue( o ) = v; o.type = T_VECTOR;
        in[0] = &o; 
        movetodev( (void **) in, NULL );
    }
}

