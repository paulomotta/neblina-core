#ifndef __LIBNEBLINA_H_
#define __LIBNEBLINA_H_

#ifdef	__cplusplus
extern "C" {
#endif
    

#include "neblina.h"
//#include "clutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ivget(vet,i) ((vet).value.i[i-1])
#define fvget(vet,i) ((vet).value.i[i-1])
#define COMPLEX_SIZE (2 * sizeof(double))

typedef struct __slist {
    int col;
    double re;
    double im;
    struct __slist * next;

} slist;


 void ** movetodev      ( void ** i, int * status );
 void ** movetohost     ( void ** i, int * status );

void ord_smat( double * m, int * idx, int max, int N );
void smatrix_line_to_col( double * out, int * idx_out, double * in, int * idx_in, int max, int N );
void print_data_type( data_type t );



void ** neblina_sparse( void ** i, int * status );

slist * slist_add( slist * l, int col, double re, double im );

void slist_clear( slist * l );
smatrix_t * smatrix_new( int nrow, int ncol, data_type type );
void smatrix_t_clear( smatrix_t * m );
void smatrix_load_double( smatrix_t * m, FILE * f );

void smatrix_load_complex( smatrix_t * m, FILE * f );
vector_t * vector_new( int len, data_type type ) ;
matrix_t * matrix_new( int nrow, int ncol, data_type type );
matrix_t * matrix_multiply( matrix_t * a, matrix_t * b );
vector_t * smatvec_multiply( smatrix_t * a, vector_t * b );
vector_t * matvec_multiply( matrix_t * a, vector_t * b );

void clear_input( void ** i, int nparams );

void neblina_strtype( data_type type, char out[256] );



void vecreqhost( vector_t * v );
void vecreqdev ( vector_t * v );
void matreqhost( matrix_t * v );
void matreqdev ( matrix_t * v );
void smatreqhost( smatrix_t * v ) ;
void smatreqdev ( smatrix_t * v );


#ifdef	__cplusplus
}
#endif

#endif




