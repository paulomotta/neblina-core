#include "neblina_io.h"
#include "neblina_list.h"
#include "package.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int doublePrintPlaces = 8;

char formatFloat[33][16] = {"%.0f", "%.1f", "%.2f", "%.3f", "%.4f", "%.5f", "%.6f", "%.7f", "%.8f", "%.9f", "%.10f", "%.11f", "%.12f", "%.13f", "%.14f", "%.15f", "%.16f", "%.17f", "%.18f", "%.19f", "%.20f", "%.21f", "%.22f", "%.23f", "%.24f", "%.25f", "%.26f", "%.27f", "%.28f", "%.29f", "%.30f", "%.31f", "%.32f"};
void print_value( const object_t * v ) {
    switch( type( *v ) ) {
        case T_INT:
            printf("%d\n", ivalue( *v ) );
            break;
        case T_FLOAT:
            printf("%lf\n", fvalue( *v ) );
            break;   
        case T_STRING:        
            if( svalue( *v ) != NULL )
                printf("%s", svalue( *v ) );
            printf("\n");    
            break;
        case T_STRTOREL:        
            printf("%s\n", svalue( *v ) );
            break;
        case T_VECTOR:
            {
                
                int k = 0;
                vector_t * vv;
                vv = (vector_t *) vvalue( *v );
                vecreqhost( vv );
                for(k=0;k<vv->len;k++) {
                    printf("[%d] %lf\n", k+1, vv->value.f[k]);
                }
                
                break;
            }
        case T_MATRIX:
            {

                int k = 0, l = 0;
                
                matrix_t * m = (matrix_t *) vvalue( *v );
                matreqhost( m );
                if( m->type == T_FLOAT ) {
                    for(k=0;k<m->nrow;k++) {
                        for(l=0;l<m->ncol;l++)
                            printf("%lf\t",m->value.f[k*m->ncol+l]);
                        printf("\n");
                    }
                } else if ( m->type == T_COMPLEX ) {
                    for(k=0;k<m->nrow;k++) {
                        for(l=0;l<m->ncol;l++) 
                            printf("%lf %lf\t",m->value.f[2*(k*m->ncol+l)], m->value.f[2*(k*m->ncol+l)+1]);
                        printf("\n");
                    }
                }
                break;
            }  
        case T_SMATRIX:
            
        default:
            printf("type not found\n");
    }
}

int print_value_file( const object_t * v, FILE * file, unsigned char LN ) {
    int stat = 0; 
    switch( type( *v ) ) {
        case T_INT:
            fprintf(file,"%d", ivalue( *v ) );
            break;
        case T_FLOAT:
            fprintf(file, formatFloat[doublePrintPlaces], fvalue( *v ) );
            break;   
        case T_STRING:        
            if( svalue( *v ) != 0 )
                fprintf(file,"%s", svalue( *v ) );
            break;
        case T_LIST:        
            {
                list_t * lst = (list_t *) vvalue( *v ); 
                int len = list_len( lst), i;
                
                printf("[");
                for(i=1; i <= len; i++ ) {
                    object_t o = list_get( lst, i );
                    
                    print_value_file( &o,file, 0 );
                    if( i != len )
                     printf(", ");
                    else
                     printf("]");                
                }
            
            }
            break;
        
        case T_VECTOR:
            {
                int k = 0;
                vector_t * vv;
                vv = (vector_t *) vvalue( *v );
                vecreqhost( vv );
                if( vv->type == T_FLOAT ) {
                    for(k=0;k<vv->len;k++) {
                        
                        fprintf(file,formatFloat[doublePrintPlaces], vv->value.f[k]);
                fprintf(file,"\n");
                    }
                } else if( vv->type == T_INT ) {
                    for(k=0;k<vv->len;k++) {
                        fprintf(file,"%i\n", vv->value.i[k]);
                    }
                }else if( vv->type == T_COMPLEX ) {
                    for(k=0;k<vv->len;k++) {
                        fprintf(file,formatFloat[doublePrintPlaces], vv->value.f[2*k]);
                        fprintf(file," ");
                        fprintf(file,formatFloat[doublePrintPlaces], vv->value.f[2*k+1] );
                        fprintf(file,"\n");
                    }
                } else if( vv->type == T_STRING ) {
                    for(k=0;k<vv->len;k++) {
                        fprintf(file,"%s\n", (char *)vv->value.s[k]);
                    }
                }
                break;
            }
        case T_MATRIX:
            {
                int k = 0, l = 0;
                
                matrix_t * m = (matrix_t *) vvalue( *v );
                matreqhost( m );
                if( m->type == T_FLOAT ) {
                    for(k=0;k<m->nrow;k++) {
                        for(l=0;l<m->ncol;l++) {
                            fprintf(file,formatFloat[doublePrintPlaces],m->value.f[k*m->ncol+l]);
                            fprintf(file,"\t" );
                        }
                        fprintf(file,"\n");
                    }
                } else if(  m->type == T_INT ) {
                    for(k=0;k<m->nrow;k++) {
                        for(l=0;l<m->ncol;l++)
                            fprintf(file,"%i\t",m->value.i[k*m->ncol+l]);
                        fprintf(file,"\n");
                    }
                } else if ( m->type == T_COMPLEX ) {
                    for(k=0;k<m->nrow;k++) {
                        for(l=0;l<m->ncol;l++) {
                            fprintf(file,formatFloat[doublePrintPlaces],m->value.f[2*(k*m->ncol+l)]);
                            fprintf(file, " ");             
                            fprintf(file,formatFloat[doublePrintPlaces],m->value.f[2*(k*m->ncol+l)+1]);           
                            fprintf(file, "\t");           
                        }
                        fprintf(file, "\n");
                    }
                }
                
                break;
            } 
        case T_SMATRIX:
        {
            int i,j;
            smatrix_t * m = (smatrix_t *) vvalue( *v );
            smatreqhost( m );
            for(i = 0; i < m->nrow; i++ ) {
                for( j = 0; j < m->maxcols; j++ ) {
                    if( m->idx_col[i*m->maxcols + j] != -1 ) {
                        if( m->type == T_FLOAT ) {                                            
                            fprintf(file, "%d %d ", i + 1, m->idx_col[i*m->maxcols + j] + 1);
                            fprintf(file,  formatFloat[doublePrintPlaces], m->m[i*m->maxcols + j] );
                            fprintf(file, "\n" );
                             
                        } else if( m->type == T_COMPLEX ) {
                            fprintf(file, "%d %d ", i + 1, m->idx_col[i*m->maxcols + j] + 1);  
                            fprintf(file,  formatFloat[doublePrintPlaces], m->m[2*(i*m->maxcols + j)]);
                            fprintf(file, " " );
                            fprintf(file,  formatFloat[doublePrintPlaces], m->m[2*(i*m->maxcols + j)+1]);
                            fprintf(file, "\n" );       
                        }
                        
                    }                
                }
            }
            break;
        }        
        case T_COMPLEX:
        {
            char re[32];
            char im[32];
            
            
            complex_t * c =  (complex_t *) vvalue( *v );
            
            if( c->re == 0 && c->im == 0 )
                fprintf( file, "0" );
            else {
                re[0] = 0;
                if( c->re != 0 )
                    sprintf( re, "%lf", c->re );
                
                im[0] = 0;
                if( c->im != 0 ) {
                    if( c->im > 0 ) {
                        sprintf( im, "+%lfi", c->im );
                    } else {
                        sprintf( im, "%lfi", c->im );
                    }
                }
                fprintf( file, "%s%s", re, im );
            }
            
            
        
        } 
        default:
            stat = 1;
    }
    if( LN )
         fprintf(file,"\n");
    return stat;
}

void * scanf_int() { return 0; }

object_t * read_value_file1( object_t * v, FILE * file ) {
    int r = fscanf(file,"%d",&ivalue(*v) );
    return v; 
}
object_t * read_value_file( object_t * v, FILE * file ) {
    int stat = 0; 
    switch( type( *v ) ) {
        case T_INT:
	    {
            int r = fscanf(file,"%d", &ivalue(*v) );
            return v;
            }
		break;
        case T_FLOAT:
            {
	    int r = fscanf(file,"%lf", &fvalue( *v ) );
            return v;
	    }
            break;   
        case T_VECTOR:
            {
                int k = 0;
                vector_t * vv;
                vv = (vector_t *) vvalue( *v );
                vecreqhost( vv );
                if( vv->type == T_FLOAT ) {
                    for(k=0;k<vv->len;k++) {
                        
                        if( fscanf(file,"%lf", &vv->value.f[k]) == EOF )
                            return (object_t *) vv;
                        //printf("READDDD -> %lf\n", vv->value.f[k] );
                    }
                } else if( vv->type == T_INT ) {
                    for(k=0;k<vv->len;k++) {
                        if( fscanf(file,"%i", &vv->value.i[k]) == EOF )
                            return (object_t *) vv; 
                    }
                } else if( vv->type == T_COMPLEX ) {
                    for(k=0;k<vv->len;k++) {
                        if( fscanf(file,"%lf", &vv->value.f[2*k]) == EOF )
                            return (object_t *) vv;
                        if( fscanf(file,"%lf", &vv->value.f[2*k+1]) == EOF )
                            return (object_t *) vv; 
                    }
                } 
                return v;
                break;
                }
        case T_SMATRIX:
        {
            int i,j;
            smatrix_t * m = (smatrix_t *) vvalue( *v );
            smatreqhost( m );
            if( m->type == T_FLOAT )    {        
                smatrix_load_double( m, file );
                /*for(i=0; i < m->nrow; i++ ) {
                    for(j=0; j < m->maxcols; j++ ) {
                        printf("%d ",  m->idx_col[i*(m->maxcols)+j] );
                    }
                    printf("\n");
                }*/
            } else if ( m->type == T_COMPLEX )  {          
                smatrix_load_complex( m, file );
             }
            return (object_t *) m;
            break;
        }        
        case T_MATRIX:
            {
                int k = 0, l = 0;
                int nzeros = 0;
                matrix_t * m = (matrix_t *) vvalue( *v );
                matreqhost( m );
                if( m->type == T_FLOAT ) {
                    for(k=0;k<m->nrow;k++) {
                        for(l=0;l<m->ncol;l++) {
                            if( fscanf(file,"%lf",&m->value.f[k*m->ncol+l]) == EOF )
                                return (object_t *) m;
                            if( m->value.f[k*m->ncol+l] == 0.0 )
                                nzeros++;
                            
                            //printf("%lf\n",  m->value.f[k*m->ncol+l]);   
                        }
                    }
                    //printf("nzeros = %d\n", nzeros );
                } else if(  m->type == T_INT ) {
                    for(k=0;k<m->nrow;k++) {
                        for(l=0;l<m->ncol;l++) {
                            if( fscanf(file,"%i\t",&m->value.i[k*m->ncol+l]) == EOF )
                                return (object_t *) m;
                        }
                    }
                } else if( m->type == T_COMPLEX ) {
                    for(k=0;k<m->nrow;k++) {
                        for(l=0;l<m->ncol;l++) {
                            int idx = 2*(k*m->ncol+l);
                            if( fscanf(file,"%lf",&m->value.f[idx]) == EOF )
                                return (object_t *) m;
                            if( fscanf(file,"%lf",&m->value.f[idx+1]) == EOF )
                                return (object_t *) m;
                            //printf("%lf\n",  m->value.f[k*m->ncol+l]);   
                        }
                    }
                }
                // printf("Return v\n");
                return v;
                break;
            }  
        default:
            stat = 1;
    }
    return NULL;
}

package_t * getIOPackage() {
    package_t * pkg   = package_init( 8, "io" );
    int ty_params[]   = { T_FILE };
    int ty_returns[]  = { T_STRING, T_STRING };   
    int ty_close[]    = { T_FILE };
    int ty_write[]    = { T_FILE, T_ANY };
    int ty_read[]     = { T_ANY };
    int ty_int[]      = { T_INT };
    int i = 0;
    pkg->functions[i++] = package_new_func( "open", 2, 1, ty_returns, ty_params, neblina_open ); 
    pkg->functions[i++] = package_new_func( "close", 1, 0, ty_close, NULL, neblina_close );
    pkg->functions[i++] = package_new_func( "print", 2, 0, ty_write, NULL, neblina_write );
    pkg->functions[i++] = package_new_func( "println", 2, 0, ty_write, NULL, neblina_writeln );
    pkg->functions[i++] = package_new_func( "print", 1, 0, ty_read, NULL, neblina_write_stdout );
    pkg->functions[i++] = package_new_func( "println", 1, 0, ty_read, NULL, neblina_writeln_stdout );
    pkg->functions[i++] = package_new_func( "read", 2, 0, ty_write, ty_read, neblina_read );
    pkg->functions[i++] = package_new_func( "fmtdouble", 1, 0, ty_int, NULL, neblina_fmtdouble );  
    return pkg;
}

void ** neblina_open( void ** i, int * status ) {
    object_t * out = (object_t *) malloc( sizeof( object_t ) );
    object_t ** in = (object_t ** ) i;
    vvalue( *out ) = (void *) fopen( svalue( *in[1] ), svalue( *in[0] ) );
    type( *out ) = T_FILE;
    
    if( vvalue( *out ) == NULL ) {
        char str[1014];
        sprintf( str, "cannot open file '%s' with permission '%s'", svalue( *in[1] ), svalue( *in[0] ) );
        runerror(  str );
    }
    
    static void * ret[1];
    clear_input(i,2);
    ret[0] = (void *) out;
    return ret;
}

void ** neblina_close( void ** i, int * status ) {
    object_t ** in = (object_t ** ) i;
    fclose( (FILE *) vvalue( *in[0] ) );
    clear_input(i,1);
    return NULL;
}

void ** neblina_write( void ** i, int * status ) {
    object_t ** in = (object_t ** ) i;
    
    print_value_file( in[0], (FILE *) vvalue( *in[1] ), 0 );
    clear_input(i,1);
    return (void *) NULL;
}

void ** neblina_writeln( void ** i, int * status ) {
    object_t ** in = (object_t ** ) i;
    print_value_file( in[0], (FILE *) vvalue( *in[1] ), 1 );
    clear_input(i,2);
    return (void *) NULL;
}

void ** neblina_write_stdout( void ** i, int * status ) {
    object_t ** in = (object_t ** ) i;
    
    print_value_file( in[0], (FILE *) stdout, 0 );
    clear_input(i,1);
    return (void *) NULL;
}

void ** neblina_writeln_stdout( void ** i, int * status ) {
    object_t ** in = (object_t ** ) i;
    print_value_file( in[0], (FILE *) stdout , 1 );
    clear_input(i,1);
    return (void *) NULL;
}

void ** neblina_read( void ** i, int * status ) {
    object_t out;// = (object_t *) malloc( sizeof( object_t ) );
    object_t ** in = (object_t ** ) i;
       
    object_t * tmp = read_value_file( in[0], (FILE *) vvalue( *in[1] ) );
    type( out ) = type( *tmp );
    vvalue( out ) = vvalue( *tmp );
    
    static void * ret[1];
    clear_input(i, 2);
    ret[0] = (void *) &out;
    return ret;
}

void ** neblina_fmtdouble( void ** i, int * status ) {
    object_t ** in = (object_t ** ) i;
    
    int fmt = ivalue( *in[0] );
    if( fmt >= 32 )
        doublePrintPlaces = 32;
    else if( fmt <= 0 )
        doublePrintPlaces = 0;
    else
        doublePrintPlaces = fmt;
    clear_input(i,1);
    return NULL;
}

