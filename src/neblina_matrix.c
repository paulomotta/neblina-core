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
    ret->extra      = NULL;   
    return ret;
}

void matrix_delete( matrix_t * m ) {
    if (m->extra != NULL) {
        cl_int status = clReleaseMemObject( (cl_mem) m->extra );
        CLERR
    }
    if (m->value.f != NULL) {
        free (m->value.f);
    }
    free (m);
}

void matreqhost( matrix_t * m ) {
    cl_int status;
    if( m->location != LOCHOS ) {
        int len = (m->type == T_COMPLEX) ? (2*m->ncol * m->nrow ) : (m->ncol * m->nrow);
        size_t size_type = (clinfo.fp64) ? sizeof(double) : sizeof(float);
        m->location = LOCHOS;
        
        if (m->value.f == NULL) {
            if( m->type == T_FLOAT ) {
                m->value.f = (double *) malloc( len * sizeof(double) );
            } else {
                m->value.f = (double *) malloc( len * COMPLEX_SIZE );
            }
        }
        
        if( clinfo.fp64 ) {
            status = clEnqueueReadBuffer (clinfo.q, (cl_mem) m->extra, CL_TRUE, 0, len * size_type, m->value.f, 0, NULL, NULL);
            CLERR
        } else {
            int i;
            // OpenMP
            float * tmp = (float *) malloc( sizeof(float) * len );
            status = clEnqueueReadBuffer (clinfo.q, (cl_mem) m->extra, CL_TRUE, 0, len * size_type, tmp, 0, NULL, NULL);
            CLERR 
            #pragma omp parallel for
            for( i = 0; i < len; i++) m->value.f[i] = tmp[i];
            free( tmp );
        }    
        clReleaseMemObject( (cl_mem) m->extra );
        CLERR
        m->extra = NULL;
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

void matrix_set_real_value(matrix_t *  m, int i, int j, double r) {
    m->value.f[i * m->ncol + j] = r;
}

double matrix_get_real_value(matrix_t *  m, int i, int j) {
    return m->value.f[i * m->ncol + j];
}

void matrix_set_complex_value(matrix_t *  m, int i, int j, double r, double im) {
    int idx = 2 * (i * m->ncol + j);
    m->value.f[idx] = r;
    m->value.f[idx + 1] = im;
}

double matrix_get_complex_real_value(matrix_t *  m, int i, int j){
    int idx = 2 * (i * m->ncol + j);
    return m->value.f[idx];
}

double matrix_get_complex_imaginary_value(matrix_t *  m, int i, int j){
    int idx = 2 * (i * m->ncol + j);
    return m->value.f[idx + 1];
}
