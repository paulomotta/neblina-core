/* 
 * File:   neblina_matrix.h
 * Author: paulo
 *
 * Created on 25 de agosto de 2021, 00:07
 */

#ifndef NEBLINA_MATRIX_H
#define NEBLINA_MATRIX_H

#ifdef __cplusplus
extern "C" {
#endif
#include "neblina.h"
#include <stdio.h>
#include <stdlib.h>
    
 typedef struct __matrix_t {
    data_vector_u      value;
    int                ncol;
    int                nrow;
    data_type          type;
    unsigned char    location;
    void*             extra;
} matrix_t;

matrix_t * matrix_new( int nrow, int ncol, data_type type );
void matrix_delete( matrix_t * v );
void matreqhost( matrix_t * v );
void matreqdev ( matrix_t * v );




#ifdef __cplusplus
}
#endif

#endif /* NEBLINA_MATRIX_H */

