#include "libneblina.h"
#include <stdio.h>
#include <stdlib.h>

matrix_t * matrix_new( int nrow, int ncol, data_type type, int initialize ) {
    matrix_t * ret = (matrix_t *) malloc( sizeof( matrix_t ) );
    
    if (initialize) {
        if( type == T_INT ) {
            ret->value.i = (int *) malloc( nrow * ncol * sizeof( int ) );
        } else if( type == T_FLOAT ) {
            ret->value.f = (double *) malloc( nrow * ncol * sizeof( double ) );
        } else if( type == T_COMPLEX ) {
            // printf("matrix_new\n");
            ret->value.f = (double *) malloc( 2 * nrow * ncol * sizeof( double ) );
            // printf("matrix_new0\n");
        }
    } else {
        ret->value.f = NULL;
    }

    
    ret->type       = type;
    // printf("matrix_new1\n");
    ret->nrow       = nrow;
    // printf("matrix_new2\n");
    ret->ncol       = ncol;
    // printf("matrix_new3\n");
    ret->location   = LOCHOS;
    // printf("matrix_new4\n");
    ret->extra      = NULL;
    // printf("matrix_new5\n");
    return ret;
}

void matrix_delete( matrix_t * m ) {

    if (m->value.f != NULL) {
        free (m->value.f);
    } else if (m->extra != NULL) {
        free (m->extra);
    }
    free (m);
}

void matreqhost( matrix_t * v ) {
    v->location  = LOCHOS;
    v->value.f = v->extra;
    v->extra = NULL;
}

void matreqdev ( matrix_t * v ) {
    v->location  = LOCDEV;
    v->extra = v->value.f;
    v->value.f = NULL;

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
