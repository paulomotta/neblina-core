#include "libneblina.h"
#include <stdio.h>
#include <stdlib.h>
void ord_smat( double * m, int * idx, int max, int N ) {
    int i, j, tmpi, k;
    double tmpd;
    for(k=0; k < N; k++ ) {
        
        for(i=max - 1; i >= 1; i-- ) {
            for(j=0; j < i; j++ ) {
                if(idx[k*max+j+1] != -1 && idx[k*max+j] != -1 && idx[k*max+j] > idx[k*max+j+1] ) {
                    tmpi = idx[k*max+j];
                    idx[k*max+j] = idx[k*max+j+1];
                    idx[k*max+j+1] = tmpi;
                    
                    tmpd = m[2*(k*max+j)];
                    m[2*(k*max+j)] =  m[2*(k*max+j+1)];
                    m[2*(k*max+j+1)] = tmpd;
                    
                    
                    tmpd = m[2*(k*max+j)+1];
                    m[2*(k*max+j)+1] = m[2*(k*max+j+1)+1];
                    m[2*(k*max+j+1)+1] = tmpd;
                    
                }   
            }
        
        
        }
    } 



}


void smatrix_line_to_col( double * out, int * idx_out, double * in, int * idx_in, int max, int N ) {
    int i, col, lin, j;
    int count[N];
    for(i=0; i < N; i++ ) {
        count[i] = 0;
        for(j=0; j < max; j++ ) 
            idx_out[i*max+ j] = -1;
    }
    
    for(i=0; i < N; i++ ){
        for(j=0; j < max; j++ ) {
            col = idx_in[i*max+j];
            idx_out[col*max+ count[col]] = i;
            out[2*(col*max+ count[col])] = in[2*(i*max + j)];
            out[2*(col*max+ count[col])+1] = in[2*(i*max + j)+1];
            count[col]++;
        }
    }
}


void print_data_type( data_type t ) {
    switch( t ) {
        case T_STRING:
            printf("type: T_STRING\n");
            break;
        case T_INT:
            printf("type: T_INT\n");
            break;
        case T_FLOAT:
            printf("type: T_FLOAT\n");
            break;
        case T_ADDR:
            printf("type: T_ADDR\n");
            break;
        case T_NDEF:
            printf("type: T_NDEF\n");
            break;
        case T_LIST:
            printf("type: T_LIST\n");
            break;
        case T_STRTOREL:
            printf("type: T_STRTOREL\n");
            break;
        case T_CFUNC:
            printf("type: T_CFUNC\n");
            break;
        case T_VECTOR:
            printf("type: T_VECTOR\n");
            break;
        case T_MATRIX:
            printf("type: T_MATRIX\n");
            break;
        case T_SMATRIX:
            printf("type: T_SMATRIX\n");
            break;
        case T_COMPLEX:
            printf("type: T_COMPLEX\n");
            break;
        case T_FILE:
            printf("type: T_FILE\n");
            break;
        case T_ANY:
            printf("type: T_ANY\n");
            break;
        default:
            printf("type: INDEFINED\n");
    };
}
slist * slist_add( slist * l, int col, double re, double im ) {
    slist * nlist = (slist *) malloc( sizeof(slist) );
    nlist->col = col;
    nlist->re = re;
    nlist->im = im;
    nlist->next = l;
    return nlist;
}


void slist_clear( slist * l ) {
    if( l == NULL )
        return;    
    do {    
        slist * tmp = l->next;
        free( l );
        l = tmp;                
    } while( l != NULL );
}

matrix_t * matrix_multiply( matrix_t * a, matrix_t * b ) {
    matrix_t * ret = (matrix_t *) malloc( sizeof( matrix_t ) );
    int i, j, k;
    int nrow = a->nrow;
    int ncol = b->ncol;

    if( type(*a) == T_FLOAT && type(*b) == T_FLOAT ) {
        ret->value.f = (double *) malloc( nrow * ncol * sizeof( double ) );
        double * tmp = (double *) malloc( ncol * sizeof( double ) );
        double sum;
        for( i = 0; i < nrow; i++ ) {
            
            for( j = 0; j < ncol; j++ ) {
                sum = 0;          
                for( k = 0; k < b->nrow; k++ )
                    sum += a->value.f[i*a->ncol + k]*b->value.f[k*b->ncol + j];
                ret->value.f[i*ncol + j] = sum;            
            }
            
        }
        ret->type       = T_FLOAT;        
        
    } else if( type(*a) == T_INT && type(*b) == T_INT ) {
        ret->value.i = (int *) malloc( nrow * ncol * sizeof( int ) );
        int * tmp = (int *) malloc( ncol * sizeof( int ) );
        int sum;
        int ii = 0;
        for( i = 0; i < nrow; i++ ) {

            ii = i*a->ncol;
            for( j = 0; j < ncol; j++ ) {
                sum = 0;                
                for( k = 0; k < b->nrow; k++ )
                    tmp[k] = b->value.i[k*b->ncol + j];
                for( k = 0; k < b->nrow; k++ )
                    sum += a->value.i[ii + k]*tmp[k];
                ret->value.i[i*ncol + j] = sum;
            }
            
        }
        ret->type       = T_INT;
        free( tmp );
    } else if( type(*a) == T_FLOAT && type(*b) == T_INT ) {
        ret->value.f = (double *) malloc( nrow * ncol * sizeof( double ) );
    } else {

    }
    ret->nrow       = nrow;
    ret->ncol       = ncol;

    return ret;
}

vector_t * smatvec_multiply( smatrix_t * a, vector_t * b ) {
    vector_t * ret = (vector_t *) malloc( sizeof( vector_t ) );
    ret->value.f = (double *) malloc( a->nrow * sizeof( double ) );
    
    int nrows = a->nrow;
    int maxcols = a->maxcols;
    int idx,i;
    for(idx=0; idx < nrows; idx++ ) {
//        printf("idx=%d\n",idx);
        if( idx >= nrows ) //?
            return NULL;
            
        double sum = 0.0f;
        int row = idx;
        for (i = 0; i < maxcols; i++) {
//            printf("row=%d maxcols=%d i=%d result=%d ",row,maxcols,i,((row * maxcols) + i));
            int col = a->idx_col[((row * maxcols) + i)];
//            printf("col=%d ",col);
            if( col == -1 )
                break;
            double value = a->m[((row * maxcols) + i)];
//            printf("value= %lf \n",value);
            sum += value * b->value.f[col]; 
        }
        ret->value.f[row] = sum;
    }
    ret->type       = T_FLOAT;
    ret->len        = a->nrow;
    ret->extra        = NULL;
    ret->location   = LOCHOS;
    return ret;
}


vector_t * matvec_multiply( matrix_t * a, vector_t * b ) {
    vector_t * ret = (vector_t *) malloc( sizeof( vector_t ) );
    int i, j, k;
   
    if( type(*a) == T_FLOAT && type(*b) == T_FLOAT ) {
        ret->value.f = (double *) malloc( a->nrow * sizeof( double ) );
        double sum;
        for( i = 0; i < a->nrow; i++ ) {
            sum = 0.0; 
            for( j = 0; j < a->ncol; j++ ) {
                sum += a->value.f[i*a->ncol + j]*b->value.f[j];                            
            }
            ret->value.f[i] = sum;
            
        }
        ret->type       = T_FLOAT;
        ret->len        = a->nrow;       
        
    } else if( type(*a) == T_COMPLEX && type(*b) == T_COMPLEX ) { 
        double sumR = 0.0;
        double sumC = 0.0;
        ret->value.f = (double *) malloc( 2* a->nrow * sizeof( double ) );
        int idx = 0;
        for( i = 0; i < a->nrow; i++ ) {
            sumR = 0.0;
            sumC = 0.0; 
            for( j = 0; j < a->ncol; j++ ) {
                idx = 2*(i*a->ncol + j);
                sumR += a->value.f[idx]*b->value.f[2*j] - a->value.f[idx+1]*b->value.f[2*j+1];
                sumC += a->value.f[idx]*b->value.f[2*j+1] + a->value.f[idx+1]*b->value.f[2*j] ;
            }
            ret->value.f[2*i] = sumR;
            ret->value.f[2*i+1] = sumC;
        }
        
        ret->type       = T_COMPLEX;
        ret->len        = a->nrow;
    
    } else {
        fprintf(stderr, "Invalid types for matrix vector multiplication");
        exit( 1 );
    }
    

    return ret;
}


void clear_input( void ** i, int nparams ) {
    int k = 0;
    object_t ** in = (object_t **) i;
    for( k = 0; k < nparams; k++ ) {
        if( type(*in[k]) == T_STRING )
            free( svalue( *in[k] ) );
        
        if( type(*in[k]) == T_COMPLEX ) {
            complex_t * r = (complex_t *)vvalue( *in[k] );
            free( r );
        }         
    }
}


void neblina_strtype( data_type type, char out[256] ) {
    switch( type ) {
        case( T_STRING ):
            sprintf(out, "string");
            break;
        case( T_INT ):
            sprintf(out, "int");
            break;
        case( T_FLOAT ):
            sprintf(out, "double");
            break;
        case( T_VECTOR):
            sprintf(out, "vector");
            break;
        case( T_MATRIX ):
            sprintf(out, "matrix");
            break;
        case( T_FILE ):
            sprintf(out, "file");
            break;
        case( T_COMPLEX ):
            sprintf(out, "complex");
            break;
        case( T_LIST ):
            sprintf(out, "list");
            break;
        default:
            sprintf(out, "unknown:%d", type);
            break;
    }
}

 void ** movetodev( void ** i, int * s ) {
        cl_int status;
        object_t ** in = (object_t **) i;
        
        size_t size_type = (clinfo.fp64) ? sizeof(double) : sizeof(float);   
        
        if( type( *in[0] ) == T_VECTOR ) {
            vector_t * v = (vector_t *) vvalue( *in[0] );
            int len = (v->type == T_COMPLEX) ? (2*v->len) : (v->len);
            if( v->location == LOCDEV )
                return (void **) NULL;
            v->location = LOCDEV;
            if( clinfo.fp64 ) {
                v->extra = clCreateBuffer( clinfo.c,  CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, len * size_type, v->value.f, &status);
                CLERR
            } else {
                int i;
                float * tmp = (float *) malloc( sizeof(float) * len );
                #pragma omp parallel for
                for( i = 0; i < len; i++){ tmp[i] = v->value.f[i]; /*printf("VV-> %f\n", tmp[i] );*/ }
                
                v->extra = clCreateBuffer( clinfo.c,  CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, len * size_type, tmp, &status);
                CLERR
                free( tmp );
            }
            
        } else if ( type( *in[0] ) == T_MATRIX ) {
            matrix_t * m = (matrix_t *) vvalue( *in[0] );
            int len = ( m->type == T_COMPLEX ) ? (2 * m->ncol * m->nrow) : (m->ncol * m->nrow);
            if( m->location == LOCDEV )
                return (void **) NULL;
            m->location = LOCDEV;
            if( clinfo.fp64 ) {
                m->extra = clCreateBuffer( clinfo.c,  CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, len * size_type, m->value.f, &status);
                CLERR
            } else {
                int i;
                float * tmp = (float *) malloc( sizeof(float) * len );
                #pragma omp parallel for
                for( i = 0; i < len; i++) tmp[i] = (float) m->value.f[i];
                m->extra = clCreateBuffer( clinfo.c,  CL_MEM_READ_ONLY |  CL_MEM_COPY_HOST_PTR, len * size_type, tmp, &status);
                CLERR
                free( tmp );
            
            }
        } else if ( type( *in[0] ) == T_SMATRIX ) {
            
            smatrix_t * m = (smatrix_t *) vvalue( *in[0] );
            if( m->location == LOCDEV )
                return (void **) NULL;
            int len = ( m->type == T_COMPLEX ) ? (2 * m->maxcols * m->nrow) : (m->maxcols * m->nrow);
            m->location = LOCDEV;
            m->idxColMem = clCreateBuffer( clinfo.c,  CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, m->maxcols * m->nrow * sizeof(int), m->idx_col, &status);
            CLERR
            if( clinfo.fp64 ) {          
                /*printf("Tot dev\n");
                int ii;
                for(ii = 0;ii < len; ii++ )
                    printf("%lf\n", m->m[ii] );
                */
                m->extra = clCreateBuffer( clinfo.c,  CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, len * size_type, m->m, &status);
                CLERR            
            } else {
                int i;
                float * tmp = (float *) malloc( sizeof(float) * len );
                #pragma omp parallel for
                for( i = 0; i < len; i++) tmp[i] = (float) m->m[i];
                m->extra = clCreateBuffer( clinfo.c,  CL_MEM_READ_ONLY |  CL_MEM_COPY_HOST_PTR, len * size_type, tmp, &status);
                CLERR
                free( tmp );
            }
        } 
        return (void **) NULL;
}

 void ** movetohost( void ** i, int * s ) {
        cl_int status;
        object_t ** in = (object_t **) i;
        size_t size_type = (clinfo.fp64) ? sizeof(double) : sizeof(float);   
        if( type( *in[0] ) == T_VECTOR ) {
//            vector_t * v = (vector_t *) vvalue( *in[0] );
//           
//            v->location = LOCHOS;
//            int len = (v->type == T_COMPLEX) ? (2*v->len) : (v->len);
//            if(clinfo.fp64) {   
//                status = clEnqueueReadBuffer (clinfo.q, v->mem, CL_TRUE, 0, len*size_type, v->value.f, 0, NULL, NULL);
//                CLERR
//            } else {
//                int i;
//                float * tmp = (float *) malloc( sizeof(float) * len );               
//                status = clEnqueueReadBuffer (clinfo.q, v->mem, CL_TRUE, 0, len*size_type,tmp, 0, NULL, NULL);
//                CLERR
//                #pragma omp parallel for
//                for( i = 0; i < len; i++){ v->value.f[i] = tmp[i]; /*printf("V -> %f\n", tmp[i]);*/ }
//                free( tmp );
//            }
//            clReleaseMemObject( v->mem );
//            CLERR                                
        } else if ( type( *in[0] ) == T_MATRIX ) {
//            matrix_t * m = (matrix_t *) vvalue( *in[0] );
//            int len = (m->type == T_COMPLEX) ? (2*m->ncol * m->nrow ) : (m->ncol * m->nrow);
//            m->location = LOCHOS;
//            if( clinfo.fp64 ) {
//                status = clEnqueueReadBuffer (clinfo.q, m->mem, CL_TRUE, 0, len * size_type, m->value.f, 0, NULL, NULL);
//                CLERR
//            } else {
//                int i;
//                // OpenMP
//                float * tmp = (float *) malloc( sizeof(float) * len );
//                status = clEnqueueReadBuffer (clinfo.q, m->mem, CL_TRUE, 0, len * size_type, tmp, 0, NULL, NULL);
//                CLERR 
//                #pragma omp parallel for
//                for( i = 0; i < len; i++) m->value.f[i] = tmp[i];
//                free( tmp );
//            }    
//            clReleaseMemObject( m->mem );
//            CLERR                           
        } else if ( type( *in[0] ) == T_SMATRIX ) {
//            smatrix_t * m = (smatrix_t *) vvalue( *in[0] );
//            m->location = LOCHOS;
//            int len = (m->type == T_COMPLEX) ? (2*m->maxcols * m->nrow  ) : ( m->maxcols * m->nrow );
//            status = clEnqueueReadBuffer (clinfo.q, m->idxColMem, CL_TRUE, 0, m->maxcols * m->nrow * sizeof(int), m->idx_col, 0, NULL, NULL);
//            CLERR            
//            if( clinfo.fp64 ) {
//            
//                status = clEnqueueReadBuffer (clinfo.q, m->mMem, CL_TRUE, 0, len * size_type, m->m, 0, NULL, NULL);
//                CLERR
//            } else {
//                int i;
//                float * tmp = (float *) malloc( sizeof(float) * len );
//                status = clEnqueueReadBuffer (clinfo.q, m->mMem, CL_TRUE, 0, len * size_type, tmp, 0, NULL, NULL);
//                CLERR
//                #pragma omp parallel for
//                for( i = 0; i < len; i++) m->m[i] = tmp[i];
//                free( tmp );
//            }              
//            clReleaseMemObject( m->idxColMem );
//            CLERR                             
//            clReleaseMemObject( m->mMem );
//            CLERR                 
        }
        return (void *) NULL;
}
