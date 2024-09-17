#include "libneblina.h"
#include <stdio.h>
#include <stdlib.h>

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

void smatrix_pack(smatrix_t * m){
    //essa parte seta os valores que foram lidos do arquivo em smat para o formato de 
    //array continuo
    m->idx_col = (int *) malloc( m->nrow * m->maxcols * sizeof(int) );
    m->m = (double *) malloc( m->nrow * m->maxcols * sizeof(double) );
    
    memset( m->idx_col, -1, m->nrow * m->maxcols * sizeof(int));  
    memset( m->m      ,  0, m->nrow * m->maxcols * sizeof(double));

    slist * tmp = NULL;
    for(int i=0; i < m->nrow; i++ ) {
        tmp = m->smat[i];        
        // printf("%p \n",&tmp);
        while( tmp != NULL ) {
            int idx = i*m->maxcols + m->icount[i];
            // printf("i=%d tmp->col=%d idx=%d ",i, tmp->col, idx);
            m->idx_col[idx] = tmp->col;
            int midx = i*m->maxcols + m->icount[i];
            // printf("midx=%d ",midx);
            m->m[midx] = tmp->re;
            // printf("value=%lf ",tmp->re);
            // printf("m->icount[i]=%d ",m->icount[i]);
            m->icount[i]++;
            tmp = tmp->next;
            // printf("\n");
        }    
        // printf("\n");
        slist_clear( m->smat[i] );
        m->smat[i] = NULL;
    }
    // printf("--------\n");
    m->isPacked = 1;
}

void smatrix_pack_complex(smatrix_t * m){
    
    m->idx_col = (int *) malloc( m->nrow * m->maxcols * sizeof(int) );
    m->m = (double *) malloc( m->nrow * m->maxcols * COMPLEX_SIZE );
    
    memset( m->idx_col, -1, m->nrow * m->maxcols * sizeof(int));  
    memset( m->m      ,  0, m->nrow * m->maxcols * COMPLEX_SIZE);

    slist * tmp = NULL;
    for(int i=0; i < m->nrow; i++ ) {
        tmp = m->smat[i];        
        while( tmp != NULL ) {
            int idx = i*m->maxcols + m->icount[i];
            m->idx_col[idx] = tmp->col;
            int midx = 2*(i*m->maxcols + m->icount[i]);
            m->m[midx] = tmp->re;
            m->m[midx+1] = tmp->im;
            m->icount[i]++;
            tmp = tmp->next;

        }    
        slist_clear( m->smat[i] ); 
        m->smat[i] = NULL;
    }

    m->isPacked = 1;
}

void smatrix_set_real_value(smatrix_t *  m, int i, int j, double r){
    
    if (m->smat ==  NULL) {
        m->smat = (slist **) malloc( m->nrow * sizeof( slist * ) );
        for(int x=0; x < m->nrow; x++ )
            m->smat[x] = NULL;   
    }
        
    if( i < 0 ||  i >= m->nrow ) {
        printf("invalid row index on loading sparse matrix %d\n",i);
        exit( -1 );
    }
    if( j < 0 || j >= m->ncol ) {
        printf("invalid col index on loading sparse matrix\n");
        exit( -1 );
    }
    // printf("set_real i=%d j=%d value=%f \n",i,j,r);
    m->smat[i] = slist_add( m->smat[i], j, r, 0.0);
    m->rcount[i]++;
    if( m->rcount[i] > m->maxcols ) {
        m->maxcols = m->rcount[i];
    }
}

void smatrix_set_complex_value(smatrix_t *  m, int i, int j, double r, double im){
    
    if (m->smat ==  NULL) {
        m->smat = (slist **) malloc( m->nrow * sizeof( slist * ) );
        for(int x=0; x < m->nrow; x++ )
            m->smat[x] = NULL;   
    }
        
    if( i < 0 ||  i >= m->nrow ) {
        printf("invalid row index on loading sparse matrix %d\n",i);
        exit( -1 );
    }
    if( j < 0 || j >= m->ncol ) {
        printf("invalid col index on loading sparse matrix\n");
        exit( -1 );
    }
                    
    m->smat[i] = slist_add( m->smat[i], j, r, im);
    m->rcount[i]++;
    if( m->rcount[i] > m->maxcols ) {
        m->maxcols = m->rcount[i];
    }
}

smatrix_t * smatrix_new( int nrow, int ncol, data_type type ) {
    smatrix_t * smatrix = (smatrix_t *) malloc( sizeof( smatrix_t ) );
    smatrix->ncol = ncol;
    smatrix->nrow = nrow;
    smatrix->type = type;
    
    smatrix->rcount = (int *) malloc( smatrix->nrow * sizeof(int) );
    smatrix->icount = (int *) malloc( smatrix->nrow * sizeof(int) );

    memset( smatrix->rcount, 0, smatrix->nrow * sizeof(int));    
    memset( smatrix->icount, 0,smatrix->nrow * sizeof(int));    

    smatrix->location = LOCHOS;
    smatrix->smat = NULL;
    smatrix->extra = NULL;
    smatrix->idxColMem = NULL;
    smatrix->m = NULL;
    smatrix->isPacked = 0;
    smatrix->maxcols = 0;
  
    
    return smatrix;
}

void smatrix_t_clear( smatrix_t * smatrix ) {
    if( smatrix != NULL ) {    
        free( smatrix->m );
        free( smatrix->idx_col );
        free( smatrix );
    }
}

void smatrix_load_double( smatrix_t * m, FILE * f ) {
    double e = 0.0;
    int * rcount = (int *) malloc( m->nrow * sizeof(int) );
    int * icount = (int *) malloc( m->nrow * sizeof(int) );
    slist ** smat = (slist **) malloc( m->nrow * sizeof( slist * ) );
    int i = 0, j = 0;;    
    int maxcols = 0;
    memset( rcount, 0, m->nrow * sizeof(int));    
    memset( icount, 0,m->nrow * sizeof(int));    
        
    for(i=0; i < m->nrow; i++ )
        smat[i] = NULL;   
        
    while( fscanf( f, "%d %d %lf", &i, &j, &e ) != EOF ) {
        i--; j--;        
        if( i < 0 ||  i >= m->nrow ) {
            printf("invalid row index on loading sparse matrix\n");
            exit( -1 );
        }
        if( j < 0 || j >= m->ncol ) {
            printf("invalid col index on loading sparse matrix\n");
            exit( -1 );
        }
                    
        smat[i] = slist_add( smat[i], j, e, 0.0);
        rcount[i]++;
        if( rcount[i] > maxcols ) {
            maxcols = rcount[i];
        }

        /*if( rcount[i] > 4 ) {
            printf("(%d, %d) %lf [%d]\n", i, j, e,rcount[i] );
        }*/
    }

    m->maxcols = maxcols;
    //printf("MAX = %d MEM = %d\n",m->maxcols,  m->nrow * m->maxcols * sizeof(int) );
    m->idx_col = (int *) malloc( m->nrow * m->maxcols * sizeof(int) );
    m->m = (double *) malloc( m->nrow * m->maxcols * sizeof(double) );
    
    memset( m->idx_col, -1, m->nrow * m->maxcols * sizeof(int));  

    slist * tmp = NULL;
    for(i=0; i < m->nrow; i++ ) {
        tmp = smat[i];        
        while( tmp != NULL ) {
            m->idx_col[i*maxcols + icount[i]] = tmp->col;
            m->m[i*maxcols + icount[i]] = tmp->re;
            icount[i]++;
            tmp = tmp->next;
        }    
        slist_clear( smat[i] );    
    }
    free( rcount );
    free( icount );
    free( smat );
    fclose( f );
}


void smatrix_load_complex( smatrix_t * m, FILE * f ) {
    double re,im;
    int * rcount = (int *) malloc( m->nrow * sizeof(int) );
    int * icount = (int *) malloc( m->nrow * sizeof(int) );
    slist ** smat = (slist **) malloc( m->nrow * sizeof( slist * ) );
    int i = 0, j = 0; 
    int maxcols = 0;
    memset( rcount, 0, m->nrow * sizeof(int));    
    memset( icount, 0,m->nrow * sizeof(int));    
        
    for(i=0; i < m->nrow; i++ )
        smat[i] = NULL;   
        
    while( fscanf( f, "%d %d %lf %lf", &i, &j, &re, &im ) != EOF ) {
        i--; j--;
        if( i < 0 || i >= m->nrow ) {
            printf("invalid row on loading sparse matrix\n");
            exit( -1 );
        }
        if( j < 0 || j >= m->ncol ) {
            printf("invalid col on loading sparse matrix\n");
            exit( -1 );
        }
                    
        smat[i] = slist_add( smat[i], j, re, im );
        rcount[i]++;
        if( rcount[i] > maxcols )
            maxcols = rcount[i];
    }

    m->maxcols = maxcols;
    m->idx_col = (int *) malloc( m->nrow * m->maxcols * sizeof(int) );
    m->m = (double *) malloc( 2 * m->nrow * m->maxcols * sizeof(double) );
    
    memset( m->idx_col, -1, m->nrow * m->maxcols * sizeof(int));  

    slist * tmp = NULL;
    for(i=0; i < m->nrow; i++ ) {
        tmp = smat[i];        
        while( tmp != NULL ) {
            m->idx_col[i*maxcols + icount[i]] = tmp->col;
            m->m[2*(i*maxcols + icount[i])] = tmp->re;
            m->m[2*(i*maxcols + icount[i])+1] = tmp->im;
            icount[i]++;
            tmp = tmp->next;

        }    
        slist_clear( smat[i] );    
    }
    free( rcount );
    free( icount );
    free( smat );
    fclose( f );
}

void smatreqhost( smatrix_t * m ) {
    if( m->location != LOCHOS ) {
        m->location = LOCHOS;
        m->idx_col = m->idxColMem;
        m->m = m->extra;
        m->extra = NULL;
        m->idxColMem = NULL;
    }
}
void smatreqdev ( smatrix_t * m ) {
    if( m->location != LOCDEV ) {
        m->location = LOCDEV;
        m->idxColMem = m->idx_col;
        m->extra = m->m;
        m->idx_col = NULL;
        m->m = NULL;
    }
}

void smatrix_delete( smatrix_t * smatrix ) {
    if (smatrix->smat != NULL) {
        slist * tmp;
        for(int i=0; i < smatrix->nrow; i++ ) {
            tmp = smatrix->smat[i];        
            slist_clear( smatrix->smat[i] );
            smatrix->smat[i] = NULL;
        }
        free(smatrix->smat);
    }
    
    if (smatrix->idx_col != NULL) {
        free(smatrix->idx_col);
    }

    if (smatrix->rcount != NULL) {
        free(smatrix->rcount);
    }

    if (smatrix->icount != NULL) {
        free(smatrix->icount);
    }

    if (smatrix->m != NULL) {
        free (smatrix->m);
    }

    free (smatrix);
}

