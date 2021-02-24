#include "neblina_vm.h"
extern char formatFloat[33][16];
extern int doublePrintPlaces;
int                       pc[N_THREADS];
extern object_t         ** memory;
extern unsigned char    *  mem_avail;
extern int                 mem_curr_size;
extern int reg_min_store;
extern int reg_max_store;
int    rel_params = 1;


instruction_t  *  code;
info_source_t  *  info;
int               code_curr_size = 512;
int               pos = 0;
object_t          stack[1024];
unsigned int  *   stack_addr;
int               top_addr = 0;
unsigned int      stack_addr_curr_size = 256;
int               top[N_THREADS];

extern int curr_line[MAX_INCLUDE_DEPTH];
extern int current_file;
extern int include_stack_ptr;
extern char  file_names_hist[MAX_INCLUDE_DEPTH][256];


char * nebgetline( int hist_file, int nline ) {
    char * line;
    line = (char *) malloc( 1024 * sizeof(char) );
    FILE * f = fopen( file_names_hist[hist_file], "r" );
    int i = 0;
    char * r;
    for( i = 1; i <= nline; i++ )
        r = fgets( line, 1024, f );
    fclose( f );    
    return line;    
}

void runerror( char * strerr ) {
    int curr_pc = pc[0]; 
    if( code[curr_pc].operation == HALT )
        curr_pc --;
    fprintf(stderr, "%s:%d runtime error: %s\n", file_names_hist[info[curr_pc].file], info[curr_pc-1].line+1, strerr);
    fprintf(stderr, "%d\t %s", info[curr_pc].line+1, nebgetline( info[curr_pc].file, info[curr_pc-1].line+1 ) );
    exit( 1 );
}
void init_machine() {
    int i = 0;
    code = (instruction_t *) malloc( code_curr_size * sizeof(instruction_t) );
    info = (info_source_t *) malloc( code_curr_size * sizeof(info_source_t) );
    memory = (object_t **) malloc( N_THREADS * sizeof(object_t *) );
    memory[0] = (object_t *) malloc( mem_curr_size * sizeof(object_t) );
    mem_avail = (unsigned char *) malloc( mem_curr_size * sizeof( unsigned char ) );
    memset( mem_avail, MEM_AVAIL, sizeof(unsigned char) );    
    stack_addr = (unsigned int *) malloc( stack_addr_curr_size * sizeof( unsigned char ) );
    
    for( i = 0; i < mem_curr_size; i++ )
        type( memory[0][i] ) = T_NDEF;
    for( i = 0; i < N_THREADS; i++ ) {
        pc[i] = -1;
        top[i] = 0;    
    }
}


void finalize_machine() {
    free( code );
    free( memory[0] );
    free( mem_avail );
    free( stack_addr );
}

void realloc_code() {
    code_curr_size *= 2; 
    code = (instruction_t *) realloc( code, code_curr_size * sizeof(instruction_t) );
    info = (info_source_t *) realloc( info, code_curr_size * sizeof(info_source_t) );
    if( !code ) {
        fprintf(stderr, "Fail to alloc code: memory fails.");
        exit( 1 );
    }
        
}

char * get_str( object_t v ) {
    char * ret = NULL;
    if( type(v) == T_STRING ) {
        ret = svalue(v);
    } else if( type(v) == T_STRTOREL ) {
        ret = svalue(v);
    } else if( type(v) == T_INT ) {
        char * s = (char *) malloc(40 * sizeof(char));
        sprintf(s, "%d", ivalue(v) );
        ret = s;
    } else if( type(v) == T_FLOAT ) {
        char * s = (char *) malloc(40 * sizeof(char));
        sprintf(s, formatFloat[doublePrintPlaces], fvalue(v) );
        ret = s;
    } else {
        runerror( "invalid conversion to string");
    }
    
    return ret;
}

object_t concat( object_t v1, object_t v2 ) {
    char * str1 = get_str( v1 );
    char * str2 = get_str( v2 );
    int len = strlen(str1) + strlen(str2) + 1;
    
    char * ret = (char *) malloc(sizeof(char) * len );
    object_t v;
    type(v) = T_STRING;
    
    //printf("%s", str1, str2);
    sprintf(ret,"%s%s", str1, str2);
    svalue(v)  =  ret;
    
    
    return v;
}



void print_stack() {
    int i;
    printf("Stack %d\n", top[0]);
    for( i = 0; i < top[0]; i++ )
        print_value( &stack[i] );
}

#define push(v,tid) stack[top[tid]++] = *v;
#define pop(tid) stack[--top[tid]];

void bytecode( enum code_ops operation, object_t arg ) {
    if( operation == STORE || operation == STORE_REF ) {
        if( avalue(arg) < reg_min_store )  reg_min_store = avalue(arg);
        if( avalue(arg) > reg_max_store )  reg_max_store = avalue(arg);
    }           
    if( pos >= code_curr_size )
        realloc_code();

    info[pos].line  = curr_line[include_stack_ptr-1];
    info[pos].file  = current_file;

    
    code[pos].operation = operation;
    code[pos++].arg       = arg;    
}

void ibytecode( enum code_ops operation, unsigned int arg ) {

    if( pos >= code_curr_size )
        realloc_code();
        
    info[pos].line  = curr_line[include_stack_ptr-1];
    info[pos].file  = current_file;

    code[pos].operation           = operation;
    avalue(code[pos++].arg)       = arg;    
    if( operation == STORE || operation == STORE_REF ) {
        if( arg < reg_min_store )  reg_min_store = arg;
        if( arg > reg_max_store )  reg_max_store = arg;
    }          
    
}

void rbytecode( enum code_ops operation, object_t * arg ) {
    if( operation == STORE  || operation == STORE_REF) {
        if( avalue(*arg) < reg_min_store )  reg_min_store = avalue(*arg);
        if( avalue(*arg) > reg_max_store )  reg_max_store = avalue(*arg);
    }          
    if( pos >= code_curr_size )
        realloc_code();
    info[pos].line  = curr_line[include_stack_ptr-1];
    info[pos].file  = current_file;
    
    code[pos].operation = operation;
    code[pos++].arg       = *arg;    
}
void ibytecode_addr( enum code_ops operation, unsigned int arg, int addr ) {
    if( pos >= code_curr_size )
        realloc_code();
    info[addr].line  = curr_line[include_stack_ptr-1];
    info[addr].file  = current_file;
    code[addr].operation           = operation;
    avalue(code[addr].arg)         = arg;    
}

int cur_location() {
    return pos;
}
int reserve_location() {
    if( pos >= code_curr_size )
        realloc_code();
    return pos++;
}



 
 void release_mem( int pos, int tid ) {
    if( type( memory[tid][pos] ) == T_VECTOR ) {       
        
        int i;
        
        vector_t * ptr = (vector_t *) vvalue( memory[tid][pos] );
        
        if( ptr->type == T_STRING ) {
            for(i=0; i < ptr->len; i++ )
                free( ptr->value.s[i] );
        }
        if( ptr->location == LOCDEV )
            clReleaseMemObject( ptr->mem );
           
        if( ptr && ptr->value.f ) {
            free( ptr->value.f );
            free( ptr );
            
            ptr->value.f = NULL;
            ptr = NULL;
            
        }
       
        
        
    } else if( type( memory[tid][pos] ) == T_MATRIX ) {
        matrix_t * ptr = (matrix_t *) vvalue( memory[tid][pos] );
        if( ptr->location == LOCDEV )
            clReleaseMemObject( ptr->mem );  
        if( ptr && ptr->value.f ) {
            free( ptr->value.f );
            free( ptr );
        }
    } else if( type( memory[tid][pos] ) == T_STRING ){
        object_t * ptr = &memory[tid][pos];
        if( ptr->value.s )
            free( ptr->value.s );
        
    } else if( type( memory[tid][pos] ) == T_COMPLEX ){
        complex_t * ptr1 = (complex_t *)vvalue( memory[tid][pos] );
        
        if( ptr1 ) {         
            free( ptr1 );            
        }
    } else if( type( memory[tid][pos] ) == T_RMATRIX ) {
        rmatrix_t * m = (rmatrix_t *)vvalue( memory[tid][pos] ); 
        nkernelsRmat--;
        rmatstat[m->id] = 0;
        free( vvalue( memory[tid][pos] ) ); 
    } 
    type( memory[tid][pos] ) = T_NDEF;
    
}

void execute() {
    
    object_t arg;
    enum code_ops opcode;

    int tid = 0;
    pc[tid] = 0;
    

    // printf("tid : %d pc[tid] : %d min : %d max : %d\n", tid, pc[tid], min_store_idx, max_store_idx );
          
    // #pragma omp parallel
    do {
        instruction_t ir = code[pc[tid]++];
        opcode = ir.operation;
        arg = ir.arg;
        rel_params = (opcode == STORE) ? rel_params : 1;
        // print_stack();
        switch( opcode ) {
            case PUSH:
                push( &arg, tid );
                break;
            case STOREV: {
                
                if( type( memory[tid][avalue(arg)] ) == T_VECTOR ) {
                
                    object_t e = pop( tid );
                    object_t idx_v = pop( tid );
                    
                    vector_t * vec = (vector_t *)vvalue(memory[tid][avalue(arg)]);
                    if( type( *vec) == T_FLOAT || type( *vec) == T_COMPLEX ) 
                        vecreqhost( vec );
                    if( ivalue( idx_v ) < 1 || ivalue( idx_v ) > vec->len ) {
                        runerror("vector index out of range");                         
                        exit( 1 );
                    }
                    if( type( e ) == T_FLOAT && type( *vec ) == T_FLOAT ) {
                        vec->value.f[ivalue(idx_v)-1] = fvalue( e );
                    } else if( type( e ) == T_INT && type( *vec ) == T_INT ) {
                        vec->value.i[ivalue(idx_v)-1] = ivalue( e );    
                    } else if( type( e ) == T_INT && type( *vec ) == T_FLOAT ) {
                        vec->value.f[ivalue(idx_v)-1] = ivalue( e );
                    } else if( type( e ) == T_FLOAT && type( *vec ) == T_INT ) {
                        vec->value.i[ivalue(idx_v)-1] = fvalue( e );
                    } else if( type( e ) == T_COMPLEX && type( *vec ) == T_COMPLEX ) {
                        complex_t * r = (complex_t *) vvalue( e );
                        int idx = 2*(ivalue(idx_v)-1);
                        vec->value.f[idx] = r->re;
                        vec->value.f[idx+1] = r->im;
                        free( r );
                    } else if( type( e ) == T_STRING && type( *vec ) == T_STRING ) {
                        free( vec->value.s[ivalue(idx_v)-1] );
                        vec->value.s[ivalue(idx_v)-1] = (void *) svalue( e );
                    } else {
                        char to[256], from[256], err[512];
                        neblina_strtype(  type( e ), from );
                        neblina_strtype( type( *vec ), to );
                        sprintf( err, "invalid conversion from %s to %s", from, to );
                        runerror( err );
                    }
                } else if( type( memory[tid][avalue(arg)] ) == T_MATRIX ) {
                     
     
                    object_t e = pop( tid );
                    object_t idx_c = pop( tid );
                    object_t idx_l = pop( tid );
                    
                    matrix_t * mat = (matrix_t *)vvalue(memory[tid][avalue(arg)]);
                    if( type( *mat) == T_FLOAT || type( *mat) == T_COMPLEX ) 
                        matreqhost( mat );
                    if( ivalue( idx_c ) < 1 || ivalue( idx_c ) > mat->ncol || ivalue( idx_l ) < 1 || ivalue( idx_l ) > mat->nrow ) {
                        char err[256];
                        sprintf( err, "matrix index out of range MAT[%d,%d] Acess(%d, %d) value = %f\n", mat->nrow, mat->ncol, ivalue( idx_l ), ivalue( idx_c ), fvalue( e ) );
                        runerror( err );
                        exit( 1 );
                    }
                    int idx = (ivalue( idx_l ) - 1)*mat->nrow + ivalue(idx_c) - 1;                    
                    if( type( e ) == T_FLOAT && type( *mat ) == T_FLOAT ) {
                        mat->value.f[idx] = fvalue( e );
                    } else if( type( e ) == T_INT && type( *mat ) == T_INT ) {
                        mat->value.i[idx] = ivalue( e );    
                    } else if( type( e ) == T_INT && type( *mat ) == T_FLOAT ) {
                        mat->value.f[idx] = ivalue( e );
                    } else if( type( e ) == T_FLOAT && type( *mat ) == T_INT ) {
                        mat->value.i[idx] = fvalue( e );
                    } else if( type( e ) == T_COMPLEX && type( *mat ) == T_COMPLEX ) {
                        complex_t * r = (complex_t *) vvalue( e );
                        int idxR = 2*(ivalue( idx_l ) - 1)*mat->ncol + 2*(ivalue(idx_c) - 1);
                        int idxC = idxR + 1;
                        mat->value.f[idxR] = r->re;
                        mat->value.f[idxC] = r->im;
                        free( r );
                    }
                } else if( type( memory[tid][avalue(arg)] ) == T_LIST ) { 
                	object_t e = pop( tid );
                    object_t idx_v = pop( tid );
                    //printf("idx = %d\n", idx_v );
                } else {
                    runerror( "invalid index access");
                }
            }
                break;

            case STORE:{
                int t0, t1;
                object_t o = pop( tid ); 
                //printf("RELEASE = %d\n", rel_params );
               // if( type( memory[tid][avalue(arg)] ) == T_STRING ) {

               //     printf("STORE: %s\n", svalue( o ) );
               // }
                if( type( memory[tid][avalue(arg)] ) == T_NDEF  ||
                    type( o ) ==  type( memory[tid][avalue(arg)] ) ) {
                    if( rel_params )                    
                        release_mem( avalue(arg), tid );
                    memory[tid][avalue(arg)] = o;
                } else {
                    if( (type( o ) == T_INT                       ||  type( o ) == T_FLOAT) &&  
                        (type(memory[tid][avalue(arg)] ) == T_INT ||  type(memory[tid][avalue(arg)] ) == T_FLOAT ) ) {
                        memory[tid][avalue(arg)] = o;                       
                    } else {
                        char to[256], from[256], err[512];
                        neblina_strtype(  type(memory[tid][avalue(arg)] ), to );
                        neblina_strtype( type( o ), from );
                        sprintf( err, "invalid conversion from '%s' to '%s'", from, to );
                        runerror( err );
                    }
                }
                
                
                
                }
                break;
             case STORE_REF:
                if( type( memory[tid][avalue(arg)] ) == T_STRTOREL )
                     free( svalue( memory[tid][avalue(arg)] ) );                
                 
                memory[tid][avalue(arg)] = pop( 0 );
                break;
            case RELMEM:
                // release_mem( avalue(arg), tid );
                break;
            case LOAD:{
                if( type( memory[tid][avalue(arg)] ) == T_STRING ) {
                    object_t o;
                    type( o ) = T_STRING;
                    svalue( o ) = (char *) malloc( sizeof( char ) * (strlen( svalue( memory[tid][avalue(arg)] ) ) + 1 ) );
                    strcpy( svalue( o ), svalue(memory[tid][avalue(arg)]) );
                    push( &o, tid );
                } else if( type( memory[tid][avalue(arg)] ) == T_COMPLEX ) {
                    object_t o;
                    type( o ) = T_COMPLEX;
                    
                    complex_t * in = (complex_t *) vvalue( memory[tid][avalue(arg)] );
                    
                    complex_t * out = (complex_t *) malloc( sizeof( complex_t) );
                    out->re = in->re;
                    out->im = in->im;
                    
                    vvalue( o ) = (void *) out;
                    
                    push( &o, tid );
                } else {
                    push( &memory[tid][avalue( arg )], tid );
                }
               
                }
                break;

           case LOADV:{
                object_t v = memory[tid][avalue( arg )];
                //printf("loadv %d", 
                if ( type( v ) == T_VECTOR ) {
                    object_t idx_v = pop( tid );
                    if( type( idx_v ) != T_INT || ivalue(idx_v) < 1 ) {
                        runerror( "non positive int in legth of vector" );
                        exit( 1 );
                    }

                    object_t e;
                    vector_t * vec = vvalue( v );
                    if( type( *vec) == T_FLOAT || type( *vec) == T_COMPLEX ) 
                        vecreqhost( vec );
                    if( ivalue( idx_v ) < 1 || ivalue( idx_v ) > vec->len ) {
                        runerror( "vector index out of range");
                        exit( 1 );
                    }

                    if( type( *vec ) == T_INT )
                        ivalue( e ) = vec->value.i[ivalue(idx_v)-1];
                    else if( type( *vec ) == T_FLOAT )
                        fvalue( e ) = vec->value.f[ivalue(idx_v)-1];
                    else if( type( *vec ) == T_COMPLEX) {
                        int idx = 2*(ivalue(idx_v)-1);
                        complex_t * r = (complex_t *) malloc(sizeof(complex_t) );
                        r->re = vec->value.f[idx]; r->im = vec->value.f[idx+1];    
                        vvalue( e ) = (void *) r;
                    } else if( type( *vec ) == T_STRING) {
                        int idx = ivalue(idx_v)-1;
                        svalue( e ) = (char *) malloc( sizeof( char ) * (strlen( (char *) vec->value.s[idx] ) + 1 ) );
                        strcpy( svalue( e ), (char *) vec->value.s[idx] );
                    }
                    type( e ) = type( *vec );
                    push( &e, tid );                
                } else if( type( v ) == T_MATRIX ) {
                    object_t idx_c = pop( tid );
                    object_t idx_l = pop( tid );
                    matrix_t * mat = vvalue( v );
                    
                    if( type( *mat) == T_FLOAT || type( *mat) == T_COMPLEX ) 
                        matreqhost( mat );
                    if( ivalue( idx_c ) < 1 || ivalue( idx_c ) > mat->ncol || ivalue( idx_l ) < 1 || ivalue( idx_l ) > mat->nrow ) {
                        runerror( "matrix index out of range");
                    }
                    object_t e;
                    type( e ) = type( *mat );
                    int idx = (ivalue( idx_l ) - 1)* mat->ncol + ivalue(idx_c) - 1;                    
                    if( type( *mat ) == T_INT )
                        ivalue( e ) = mat->value.i[idx];
                    else if( type( *mat ) == T_FLOAT )
                        fvalue( e ) = mat->value.f[idx];
                    else if( type( *mat ) == T_COMPLEX) {
                        int idxR = 2*(ivalue( idx_l ) - 1)*mat->ncol + 2*(ivalue(idx_c) - 1);
                        int idxC = 2*(ivalue( idx_l ) - 1)*mat->ncol + 2*(ivalue(idx_c) - 1)+1;
                        complex_t * r = (complex_t *) malloc(sizeof(complex_t) );
                        r->re = mat->value.f[idxR]; 
                        r->im = mat->value.f[idxC];    
                        vvalue( e ) = (void *) r;
                    }
                    
                    push( &e, tid );                
                } else if ( type( v ) == T_LIST ) { 
                	object_t idx = pop( tid );
                	list_t * L = (list_t *) vvalue( v );
                    object_t e = list_get( L, ivalue( idx ) );
                    push( &e, tid );  
                    
                } else {
                    runerror( "unexpected [ ] in scalar variable");
                    exit( 1 );
                }
            }
                break;
                
            case GOTO:
                pc[tid] = avalue(arg);
                break;
            case GOTO_ST:
            {
                    int idx = pc[tid]-2;
                    while( code[idx].operation == LOAD && idx >= 0 ) {
                        if( type( memory[tid][avalue(code[idx].arg)] ) == T_MATRIX ||
                            type( memory[tid][avalue(code[idx].arg)] ) == T_VECTOR  ) {
                            memory[tid][avalue(code[idx].arg)].type = T_NDEF;
                        }    
                        idx--;       
                    } 
                    pc[tid] = stack_addr[--top_addr];
             }
                break;
            case LDADDR:
                
                // reg_addr = avalue( stack[top[tid]-1] ); top[tid]--;
                break;    
            case CALL:
                {
                                rel_params = 0;
                                pc[tid] = avalue( arg );
                }
                break;
            
            case CALL_C:
                {
                
                func_info_t * f  =  pvalue( arg );
                object_t * input[1024];
                int i;
                if( f->nparams != 0 ) {
                    for( i = 0; i < f->nparams; i++ ) {
                        if(  f->type_params[f->nparams - i - 1] == T_ANY ||
                             f->type_params[f->nparams - i - 1] == type( stack[top[tid] - 1] ) )
                            input[i] = &stack[--top[tid]];     
                        else {
                            char exp[256], passed[256];
                            neblina_strtype( f->type_params[f->nparams - i - 1], exp );
                            neblina_strtype( type( stack[top[tid] - 1] ), passed );
                            char err[512];
                            sprintf( err, "expected a '%s' type in argument %d. The type passed was '%s'", exp, f->nparams - i, passed ); 
                            runerror( err );
                        }   
                        
                    }         
                }
                int status;
                object_t ** output = (object_t **) f->ptr_func( (void **)input, &status );        
                
            
                for(i=0;i<f->nreturns;i++)
                    push( output[i], tid );      
                }
                
                
    
                break;
            case POP:
                pop( tid );        
                break;            
                
            case PUSH_PC:
                {
                object_t addr_back;
                type(addr_back) = T_ADDR;
                avalue(addr_back) = pc[tid] + avalue( arg );
                stack_addr[top_addr++] = avalue(addr_back);
                }
                break;
            case ALLOCV: {
                unsigned int type = (avalue( arg ) >> 8) & 0xFF;
                unsigned int internal_type = avalue( arg ) & 0xFF;                    
                unsigned int nparams  = (avalue( arg ) >> 16) & 0xFFFF;
                
                if( nparams == 0 ) {
                    if( type == T_VECTOR ) {
                        object_t obj = pop( tid );  
                        if( type( obj ) != T_INT || ivalue( obj ) < 1 ) {
                            runerror( "non positive int on length of matrix");
                            exit( 1 );
                        }
                       
                        vector_t * v = vector_new( ivalue( obj ), internal_type ); 
                        type( obj ) = T_VECTOR; 
                        vvalue( obj ) = (void *) v; 
                        v->location = LOCHOS;
                        push( &obj, tid );
                    } else if( type == T_MATRIX ) {
                        object_t ncol = pop( tid );
                        object_t nrow = pop( tid );
                            
                        if( type( nrow ) != T_INT || type( ncol ) != T_INT || ivalue( ncol ) < 1 || ivalue( nrow ) < 1  ) {
                            runerror( "non positive int on length of matrix");
                            exit( 1 );
                        }
                        matrix_t * m = matrix_new( ivalue(nrow), ivalue(ncol), internal_type );
                        object_t obj; 
                        type( obj ) = T_MATRIX;
                        vvalue( obj ) = (void *) m;
                        m->location = LOCHOS;
                        push( &obj, tid );
                    }else if( type == T_SMATRIX ) {
                        object_t ncol = pop( tid );
                        object_t nrow = pop( tid );
                            
                        if( type( nrow ) != T_INT || type( ncol ) != T_INT || ivalue( ncol ) < 1 || ivalue( nrow ) < 1  ) {
                            runerror( "non positive int on length of sparse matrix");
                            exit( 1 );
                        }
                       
                        smatrix_t * m = smatrix_new( ivalue(nrow), ivalue(ncol), internal_type );
                        m->maxcols=0;
                        object_t obj; 
                        type( obj ) = T_SMATRIX;
                        vvalue( obj ) = (void *) m;
                        m->location = LOCHOS;
                        push( &obj, tid );                    
                    } else if( type == T_NDEF ) {
                        object_t obj = pop( tid );
                        object_t final = pop( tid );  
                        object_t init = pop( tid );  
                        vector_t * vet = (vector_t *) vvalue( obj );
                        if( type( final ) != T_INT || type( init ) != T_INT ) {
                            runerror( "non positive int on length of vector");
                            exit( 1 );
                        }

                        if(  ivalue( final ) < ivalue( init ) || ivalue( init ) < 1 || ivalue( final ) >  vet->len ) {
                            runerror( "invalid range in vector");
                            exit( 1 );  
                        }

                        vector_t * v = vector_new( ivalue( final ) - ivalue( init ) + 1 , vet->type ); 
                        if( vet->type == T_FLOAT )
                            memcpy( v->value.f, &vet->value.f[(ivalue( init ) - 1)],  sizeof(double)*(ivalue( final ) - ivalue( init )+1));
                        else if( vet->type == T_INT ) {
                            memcpy( v->value.i, &vet->value.i[(ivalue(init)-1)],  sizeof(int)*(ivalue(final)-ivalue(init)+1));                        
                        }
                        else 
                            runerror( "indefined type error");
                        type( obj ) = T_VECTOR; 
                        vvalue( obj ) = (void *) v; 
                        push( &obj, tid );
                    }
                } else {
                     vector_t * v = vector_new( nparams, internal_type );   
                     object_t o;
                     int i;
                     if( internal_type == T_FLOAT ) {                            
                         for( i = nparams - 1; i >= 0; i-- ) {
                            o = pop( tid );
                            if( type(o) == T_FLOAT ) 
                                v->value.f[i] = fvalue(o);
                            else if ( type(o) == T_INT )
                                v->value.f[i] = ivalue(o);                                
                            else {
                                runerror( "diferent type");
                            }
                         }
                     } else if( internal_type == T_INT ) {   
                         for( i = nparams - 1; i >= 0; i-- ) {
                            o = pop( tid );
                
                            if( type(o) == T_FLOAT ) 
                                v->value.i[i] = fvalue(o);
                            else if ( type(o) == T_INT )
                                v->value.i[i] = ivalue(o);                                
                            else {
                                runerror( "diferent type");
                            }
                         }
                     } else {
                               runerror( "internal type of vector not defined");
                     }
                     vvalue( o ) = (void *) v;
                      
                     type( o ) = T_VECTOR;
                     push( &o, tid );
                }
                break;
            }

            
            case ADD:
          
                if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    fvalue( stack[top[tid] - 2] ) = fvalue( stack[top[tid] - 2] ) + fvalue( stack[top[tid]-1] );
                    top[tid]--;
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_INT ) {
                    
                    ivalue( stack[top[tid] - 2] ) = ivalue( stack[top[tid] - 2] ) + ivalue( stack[top[tid] -1] );
                    top[tid]--;
                                        
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    fvalue( stack[top[tid] - 2] ) = fvalue( stack[top[tid] - 2] ) + ivalue( stack[top[tid]-1] );               
                    top[tid]--;
                } else if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_INT ) {
                    type( stack[top[tid] - 2] ) = T_FLOAT;
                    fvalue( stack[top[tid] - 2] ) = (double)ivalue(stack[top[tid] - 2]);
                    fvalue( stack[top[tid] - 2] ) = fvalue( stack[top[tid] - 2] ) + fvalue( stack[top[tid]-1] );    
                    top[tid]--;
                
                } else if( type( stack[top[tid] - 2] ) == T_COMPLEX || type( stack[top[tid] - 1] ) == T_COMPLEX  ) {
                    if( type( stack[top[tid] - 1] ) == T_COMPLEX ) {
                        object_t tmp = stack[top[tid] - 1];           // force stack[-2] be a complex
                        stack[top[tid] - 1] = stack[top[tid] - 2];
                        stack[top[tid] - 2] = tmp;
                    }
                    
                    complex_t * c = (complex_t *) vvalue( ( stack[top[tid] - 2] ) );
                    if( type( stack[top[tid] - 1] ) == T_FLOAT ) {
                        c->re = c->re +  fvalue(stack[top[tid] - 1]);
                        
                        top[tid]--;
                    } else if( type( stack[top[tid] - 1] ) == T_INT ) {
                        c->re = c->re +  ivalue(stack[top[tid] - 1]);
                        top[tid]--;
                    } else if( type( stack[top[tid] - 1] ) == T_COMPLEX ) {
                        complex_t * d = (complex_t *) vvalue( ( stack[top[tid] - 1] ) );
                        c->re = c->re + d->re;  
                        c->im = c->im + d->im;
                        free( d ); 
                        top[tid]--;
                    } else {
                        runerror("operator + error");                         
                        exit( 1 );
                    }
                } else /* if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_INT )*/ {
                    object_t v = concat(stack[top[tid] - 2], stack[top[tid] - 1] );
                    
                    if( type( stack[top[tid] - 2] ) == T_STRING ) {
                        free( svalue( stack[top[tid] - 2] ));    
                    }
                    if( type( stack[top[tid] - 1] ) == T_STRING ) {
                        free( svalue( stack[top[tid] - 1] ));
                    }
                    stack[top[tid] - 2] = v;               
                    top[tid]--;
                }
         
                break; 
            case SUB:
                if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    fvalue( stack[top[tid] - 2] ) = fvalue( stack[top[tid] - 2] )-fvalue( stack[top[tid]-1] ); top[tid]--;
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) =  ivalue( stack[top[tid] - 2] )-ivalue( stack[top[tid]-1] ); top[tid]--;                    
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    fvalue( stack[top[tid] - 2] ) = fvalue( stack[top[tid] - 2] ) - ivalue( stack[top[tid]-1] ); top[tid]--;               
                } else if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_INT ) {
                    type( stack[top[tid] - 2] ) = T_FLOAT;
                    fvalue( stack[top[tid] - 2] ) = (double)ivalue(stack[top[tid] - 2]);
                    fvalue( stack[top[tid] - 2] ) = fvalue( stack[top[tid] - 2] ) - fvalue( stack[top[tid]-1] ); top[tid]--;
                } else if( type( stack[top[tid] - 2] ) == T_COMPLEX || type( stack[top[tid] - 1] ) == T_COMPLEX  ) {
                    int mul = 1;
                    if( type( stack[top[tid] - 1] ) == T_COMPLEX ) {
                        object_t tmp = stack[top[tid] - 1];           // force stack[-2] be a complex
                        stack[top[tid] - 1] = stack[top[tid] - 2];
                        stack[top[tid] - 2] = tmp;
                        mul = -1;
                        
                    }
                    
                    complex_t * c = (complex_t *) vvalue( ( stack[top[tid] - 2] ) );
                    if( type( stack[top[tid] - 1] ) == T_FLOAT ) {
                        c->re = mul*(c->re -  fvalue(stack[top[tid] - 1]));
                        
                        top[tid]--;
                    } else if( type( stack[top[tid] - 1] ) == T_INT ) {
                        c->re = mul*(c->re -  ivalue(stack[top[tid] - 1]));
                        top[tid]--;
                    } else if( type( stack[top[tid] - 1] ) == T_COMPLEX ) {
                        complex_t * d = (complex_t *) vvalue( ( stack[top[tid] - 1] ) );
                        c->re = mul*(c->re - d->re);  
                        c->im = mul*(c->im - d->im);
                        free( d ); 
                        top[tid]--;
                    } else {
                        runerror("operator - error");                         
                        exit( 1 );
                    }
                }
                break; 
            case DIV:
                if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    fvalue( stack[top[tid] - 2] ) = fvalue( stack[top[tid] - 2] ) / fvalue( stack[top[tid]-1] ); top[tid]--;
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = ivalue( stack[top[tid] - 2] )/ivalue( stack[top[tid]-1] ); top[tid]--;                    
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    fvalue( stack[top[tid] - 2] ) = fvalue( stack[top[tid] - 2] )/ivalue( stack[top[tid]-1] ); top[tid]--;               
                } else if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_INT ) {
                    type( stack[top[tid] - 2] ) = T_FLOAT;
                    fvalue( stack[top[tid] - 2] ) = (double)ivalue(stack[top[tid] - 2]);
                    fvalue( stack[top[tid] - 2] ) = fvalue( stack[top[tid] - 2] )/fvalue( stack[top[tid]-1] ); top[tid]--;
                } else if( type( stack[top[tid] - 2] ) == T_COMPLEX || type( stack[top[tid] - 1] ) == T_COMPLEX  ) {
                    complex_t d; complex_t n;
                    if( type( stack[top[tid] - 2] ) != T_COMPLEX ) {
                    
                        if( type( stack[top[tid] - 2] ) == T_FLOAT ) {
                            n.im = 0.0;
                            n.re = fvalue( stack[top[tid] - 2] ); 
                        } else if( type( stack[top[tid] - 2] ) == T_INT ) {
                            n.im = 0.0;
                            n.re = ivalue( stack[top[tid] - 2] ); 
                        } else {
                            runerror("operator / error");
                        }
                    
                    } else {
                        complex_t * tmp = (complex_t *) vvalue(stack[top[tid] - 2]);
                        n.im = tmp->im;
                        n.re = tmp->re;
                    }
                    
                    if( type( stack[top[tid] - 1] ) != T_COMPLEX ) {
                        if( type( stack[top[tid] - 1] ) == T_FLOAT ) {
                            d.im = 0.0;
                            d.re = fvalue( stack[top[tid] - 1] ); 
                        } else if( type( stack[top[tid] - 1] ) == T_INT ) {
                            d.im = 0.0;
                            d.re = ivalue( stack[top[tid] - 1] ); 
                        } else {
                            runerror("operator / error");
                        }
                    } else {
                        complex_t * tmp = (complex_t *) vvalue(stack[top[tid] - 1]);
                        d.im = tmp->im;
                        d.re = tmp->re;
                    }
                    
                    double rd = d.im * d.im + d.re*d.re;
                    if( rd == 0.0 )
                        runerror("division by zero");
                    
                    if( type( stack[top[tid] - 2] ) == T_COMPLEX && type( stack[top[tid] - 1] ) == T_COMPLEX  ) {
                        free( vvalue( stack[top[tid] - 1] ) );
                    } else if( type( stack[top[tid] - 2] ) != T_COMPLEX ) {
                        object_t tmp = stack[top[tid] - 1];           // force stack[-2] be a complex
                        stack[top[tid] - 1] = stack[top[tid] - 2];
                        stack[top[tid] - 2] = tmp;
                    }
                    complex_t * tmp = (complex_t *) vvalue(stack[top[tid] - 2]);
                    tmp->re = ( n.re * d.re  + n.im * d.im)/rd;
                    tmp->im = (n.im*d.re-n.re*d.im)/rd;
                    top[tid]--;
                } else {
                    runerror("operator / error");         
                }
                break; 

            case EQ:
                if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    ivalue( stack[top[tid] - 2] ) = (fvalue( stack[top[tid] - 2] ) == fvalue( stack[top[tid] - 1] )) ? 1 : 0;
                    type(stack[top[tid] - 2]) = T_INT;
                    top[tid]--;
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = (ivalue( stack[top[tid] - 2] ) == ivalue( stack[top[tid] - 1] )) ? 1 : 0;                    
                    type(stack[top[tid] - 2]) = T_INT;
                    top[tid]--;                    
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    ivalue( stack[top[tid] - 2] ) = (fvalue( stack[top[tid] - 2] ) == ivalue( stack[top[tid] -1] )) ? 1 : 0; top[tid]--;               
                    type(stack[top[tid] - 1]) = T_INT;
                } else if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = (ivalue( stack[top[tid] - 2] ) == fvalue(stack[top[tid] -1] )) ? 1 : 0; top[tid]--;
                    type( stack[top[tid] - 1] ) = T_INT;
                } else if( type( stack[top[tid] - 1] ) == T_STRING && type( stack[top[tid] - 2] ) == T_STRING ) {
                    int r = strcmp( svalue( stack[top[tid] - 2] ), svalue( stack[top[tid] - 1] ) ) ? 0 : 1;
                    free( svalue( stack[top[tid] - 2] )  );
                    free( svalue( stack[top[tid] - 1] )  );
                    
                    type(stack[top[tid] - 2]) = T_INT;
                    ivalue(stack[top[tid] - 2]) = r;
                    top[tid]--;                    
                } else if( type( stack[top[tid] - 1] ) == T_COMPLEX && type( stack[top[tid] - 2] ) == T_COMPLEX ) {
                    complex_t * c1 = (complex_t *) vvalue(  stack[top[tid] - 1] );
                    complex_t * c2 = (complex_t *) vvalue(  stack[top[tid] - 2] );   
                    int res = (c1->im == c2->im) && (c1->re == c2->re) ? 1 : 0;
                    type(stack[top[tid] - 2]) = T_INT;
                    ivalue(stack[top[tid] - 2]) = res;
                    top[tid]--;
                } else {
                    runerror("== type not found");                         
                    exit( 1 );
                    // printf("Error: EQ type not found\n");
                }
                break;
            case NEQ:
                if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    ivalue( stack[top[tid] - 2] ) = (fvalue( stack[top[tid] - 2] ) != fvalue( stack[top[tid] - 1] )) ? 1 : 0;
                    type(stack[top[tid] - 2]) = T_INT;
                    top[tid]--;
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = (ivalue( stack[top[tid] - 2] ) != ivalue( stack[top[tid] - 1] )) ? 1 : 0;                    
                    type(stack[top[tid] - 2]) = T_INT;
                    top[tid]--;                    
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    ivalue( stack[top[tid] - 2] ) = (fvalue( stack[top[tid] - 2] ) != ivalue( stack[top[tid] -1] )) ? 1 : 0; top[tid]--;               
                    type(stack[top[tid] - 1]) = T_INT;
                } else if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = (ivalue( stack[top[tid] - 2] ) != fvalue(stack[top[tid] -1] )) ? 1 : 0; top[tid]--;
                    type( stack[top[tid] - 1] ) = T_INT;
                }else if( type( stack[top[tid] - 1] ) == T_STRING && type( stack[top[tid] - 2] ) == T_STRING ) {
                    int r = strcmp( svalue( stack[top[tid] - 2] ), svalue( stack[top[tid] - 1] ) ) ? 1 : 0; 
                    free( svalue( stack[top[tid] - 2] )  );
                    free( svalue( stack[top[tid] - 1] )  );
                    
                    type(stack[top[tid] - 2]) = T_INT;
                    ivalue(stack[top[tid] - 2]) = r;
                    top[tid]--; 
                } else if( type( stack[top[tid] - 1] ) == T_COMPLEX && type( stack[top[tid] - 2] ) == T_COMPLEX ) {
                    complex_t * c1 = (complex_t *) vvalue(  stack[top[tid] - 1] );
                    complex_t * c2 = (complex_t *) vvalue(  stack[top[tid] - 2] );   
                    int res = (c1->im == c2->im) && (c1->re == c2->re) ? 0 : 1;
                    type(stack[top[tid] - 2]) = T_INT;
                    ivalue(stack[top[tid] - 2]) = res;
                    top[tid]--;
                } else {
                    runerror("type not found for !=");                         
                    exit( 1 );
                    // printf("Error: NEQ type not found\n");
                }
                break;

            case MUL:
                if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    fvalue( stack[top[tid] - 2] ) = fvalue( stack[top[tid] - 2] ) * fvalue( stack[top[tid]-1] ); top[tid]--;
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = ivalue( stack[top[tid] - 2] ) * ivalue( stack[top[tid]-1] ); top[tid]--;                    
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    fvalue( stack[top[tid] - 2] ) = fvalue( stack[top[tid] - 2] ) * ivalue( stack[top[tid]-1] ); top[tid]--;               
                } else if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_INT ) {
                    type( stack[top[tid] - 2] ) = T_FLOAT;
                    fvalue( stack[top[tid] - 2] ) = (double)ivalue(stack[top[tid] - 2]);
                    fvalue( stack[top[tid] - 2] ) = fvalue( stack[top[tid] - 2] ) * fvalue( stack[top[tid]-1] ); top[tid]--;
                } else if( type( stack[top[tid] - 2] ) == T_COMPLEX || type( stack[top[tid] - 1] ) == T_COMPLEX  ) {
                    if(  type( stack[top[tid] - 1] ) == T_COMPLEX ) {
                        object_t tmp = stack[top[tid] - 1];           // force stack[-2] be a complex
                        stack[top[tid] - 1] = stack[top[tid] - 2];
                        stack[top[tid] - 2] = tmp;
                    }
                    
                    complex_t * c = (complex_t *) vvalue( ( stack[top[tid] - 2] ) );
                    if( type( stack[top[tid] - 1] ) == T_FLOAT ) {
                        c->re = c->re *  fvalue(stack[top[tid] - 1]);
                        c->im = c->im *  fvalue(stack[top[tid] - 1]);
                        top[tid]--;
                    } else if( type( stack[top[tid] - 1] ) == T_INT ) {
                        c->re = c->re *  ivalue(stack[top[tid] - 1]);
                        c->im = c->im *  ivalue(stack[top[tid] - 1]);
                        top[tid]--;
                    } else if( type( stack[top[tid] - 1] ) == T_COMPLEX ) {
                        complex_t * d = (complex_t *) vvalue( ( stack[top[tid] - 1] ) );
                        complex_t t; t.re = c->re; t.im = c->im;
                        c->re = c->re * d->re - c->im * d->im;  
                        c->im = t.re * d->im + t.im * d->re;
                        free( d ); 
                        top[tid]--;
                    } else {
                        runerror("operator * error");                         
                        exit( 1 );
                    }
                    
                } else if( type( stack[top[tid] - 2] ) == T_MATRIX || type( stack[top[tid] - 1] ) == T_VECTOR  ) {
                    
                    // TODO: matvec_mul3( 
                } else {
                    runerror("operator * error");                         
                    exit( 1 );
                    // printf("Error: NEQ type not found\n");
                }
                break;  
            case SIGCH:{
                object_t o = pop( tid );
                if( type( o ) == T_FLOAT ) {
                    fvalue( o ) = (-1.0)* fvalue( o );
                    push( &o, tid );
                } else if( type( o ) == T_INT ) {
                    ivalue( o ) = (-1.0)* ivalue( o );
                    push( &o, tid );
                } else if( type( o ) == T_COMPLEX ) {
                    complex_t * c = (complex_t *) vvalue( o );
                    c->re = -c->re;
                    c->im = -c->im; 
                    push( &o, tid );
                } else {
                    runerror( "invalid usage of signal"); 
                }
                break;
                
            }  
            case MOD:
                if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = ivalue( stack[top[tid] - 2] ) % ivalue( stack[top[tid]-1] ); top[tid]--;
                } else {
                    runerror("invalid operands to binary %%");                         
                    exit( 1 );
                }
                break;
                
             case LEQ:
                if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    ivalue( stack[top[tid] - 2] ) = (fvalue( stack[top[tid] - 2] ) <= fvalue( stack[top[tid] - 1] )) ? 1 : 0;
                    type(stack[top[tid] - 2]) = T_INT;
                    top[tid]--;
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = (ivalue( stack[top[tid] - 2] ) <= ivalue( stack[top[tid] - 1] )) ? 1 : 0;                    
                    type(stack[top[tid] - 2]) = T_INT;
                    top[tid]--;                    
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    ivalue( stack[top[tid] - 2] ) = (fvalue( stack[top[tid] - 2] ) <= ivalue( stack[top[tid] -1] )) ? 1 : 0; top[tid]--;               
                    type(stack[top[tid] - 1]) = T_INT;
                } else if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = (ivalue( stack[top[tid] - 2] ) <= fvalue(stack[top[tid] -1] )) ? 1 : 0; top[tid]--;
                    type( stack[top[tid] - 1] ) = T_INT;
                } else {
                    runerror("LEQ type not found");                         
                    exit( 1 );
                    // printf("Error: LEQ type not found\n");
                }
                break;
             case LT:
                if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    ivalue( stack[top[tid] - 2] ) = (fvalue( stack[top[tid] - 2] ) < fvalue( stack[top[tid] - 1] )) ? 1 : 0;
                    type(stack[top[tid] - 2]) = T_INT;
                    top[tid]--;
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = (ivalue( stack[top[tid] - 2] ) < ivalue( stack[top[tid] - 1] )) ? 1 : 0;                    
                    type(stack[top[tid] - 2]) = T_INT;
                    top[tid]--;                    
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    ivalue( stack[top[tid] - 2] ) = (fvalue( stack[top[tid] - 2] ) < ivalue( stack[top[tid] -1] )) ? 1 : 0; top[tid]--;               
                    type(stack[top[tid] - 1]) = T_INT;
                } else if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = (ivalue( stack[top[tid] - 2] ) < fvalue(stack[top[tid] -1] )) ? 1 : 0; top[tid]--;
                    type( stack[top[tid] - 1] ) = T_INT;
                } else {
                    runerror("LT type not found");                         
                    exit( 1 );
                }
                break;
            case GEQ:

                if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    ivalue( stack[top[tid] - 2] ) = (fvalue( stack[top[tid] - 2] ) >= fvalue( stack[top[tid] - 1] )) ? 1 : 0;
                    type(stack[top[tid] - 2]) = T_INT;
                    top[tid]--;
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = (ivalue( stack[top[tid] - 2] ) >= ivalue( stack[top[tid] - 1] )) ? 1 : 0;                    
                    type(stack[top[tid] - 2]) = T_INT;
                    top[tid]--;                    
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    ivalue( stack[top[tid] - 2] ) = (fvalue( stack[top[tid] - 2] ) >= ivalue( stack[top[tid] - 1] )) ? 1 : 0;               
                    type(stack[top[tid] - 2]) = T_INT;
                    top[tid]--;
                } else if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = (ivalue( stack[top[tid] - 2] ) >= fvalue(stack[top[tid] - 1])) ? 1 : 0;
                    type( stack[top[tid] - 2] ) = T_INT;
                    top[tid]--;
                } else {
                    runerror("GEQ type not found");                         
                    exit( 1 );
                }
                break;
                
            case ANDL:

                if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = (ivalue( stack[top[tid] - 2] ) && ivalue( stack[top[tid] - 1] )) ? 1 : 0;                    
                    type(stack[top[tid] - 2]) = T_INT;
                    top[tid]--;                    
                } else {
                    runerror("ANDL type not found");                         
                    exit( 1 );
                    // printf("Error, AND type not found\n");
                }
                break;
                
            case ORL:
                if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = (ivalue( stack[top[tid] - 2] ) || ivalue( stack[top[tid] - 1] )) ? 1 : 0;                    
                    type(stack[top[tid] - 2]) = T_INT;
                    top[tid]--;                    
                } else {
                    runerror("ORL type not found");                         
                    exit( 1 );
                }
                
                break;
            case GT:
             
                if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    ivalue( stack[top[tid] - 2] ) = (fvalue( stack[top[tid] - 2] ) > fvalue( stack[top[tid] - 1] )) ? 1 : 0;
                    type(stack[top[tid] - 2]) = T_INT;
                    top[tid]--;
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = (ivalue( stack[top[tid] - 2] ) > ivalue( stack[top[tid] - 1] )) ? 1 : 0;                    
                    type(stack[top[tid] - 2]) = T_INT;
                    top[tid]--;                    
                } else if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_FLOAT ) {
                    ivalue( stack[top[tid] - 2] ) = (fvalue( stack[top[tid] - 2] ) > ivalue( stack[top[tid] -1] )) ? 1 : 0; top[tid]--;               
                    type(stack[top[tid] - 1]) = T_INT;
                } else if( type( stack[top[tid] - 1] ) == T_FLOAT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = (ivalue( stack[top[tid] - 2] ) > fvalue(stack[top[tid] -1] )) ? 1 : 0; top[tid]--;
                    type( stack[top[tid] - 1] ) = T_INT;
                } else {
                    printf("Error, GT type not found\n");
                }
             
                break;
            case ANDA:
                if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = (ivalue( stack[top[tid] - 2] ) & ivalue( stack[top[tid] - 1] ));                    
                    type(stack[top[tid] - 2]) = T_INT;
                    top[tid]--;                    
                } else {
                    runerror("AND invalid type");                         
                    exit( 1 );
                    // printf("Error, AND type not found\n");
                }
                break;
                
            case ORA:
                if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = (ivalue( stack[top[tid] - 2] ) | ivalue( stack[top[tid] - 1] ));                    
                    type(stack[top[tid] - 2]) = T_INT;
                    top[tid]--;                    
                } else {
                    runerror("OR invalid type");                         
                    exit( 1 );
                    // printf("Error, AND type not found\n");
                }
                break;
            case NEGL:
                if( type( stack[top[tid] - 1] ) == T_INT ) {
                    
                    ivalue( stack[top[tid] - 1] ) = !ivalue( stack[top[tid] - 1] );                   
                } else {
                    runerror("NEG invalid type");                         
                    exit( 1 );
                    // printf("Error, AND type not found\n");
                }
                break;
            case SHR:
                if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = (ivalue( stack[top[tid] - 2] ) >> ivalue( stack[top[tid] - 1] ));                    
                    type(stack[top[tid] - 2]) = T_INT;
                    top[tid]--;                    
                } else {
                    runerror("SHIFTR invalid type");                         
                    exit( 1 );
                    // printf("Error, AND type not found\n");
                }
                break;
            case SHL:
                if( type( stack[top[tid] - 1] ) == T_INT && type( stack[top[tid] - 2] ) == T_INT ) {
                    ivalue( stack[top[tid] - 2] ) = (ivalue( stack[top[tid] - 2] ) << ivalue( stack[top[tid] - 1] ));                    
                    type(stack[top[tid] - 2]) = T_INT;
                    top[tid]--;                    
                } else {
                    runerror("SHIFTR invalid type");                         
                    exit( 1 );
                    // printf("Error, AND type not found\n");
                }
                break;
            case INC:
                 if( type( memory[tid][avalue(arg)] ) == T_INT )
                    ivalue( memory[tid][avalue(arg)] )++;
                 else if( type( memory[tid][avalue(arg)] ) == T_FLOAT )
                    fvalue( memory[tid][avalue(arg)] )++;                 
                 else {
                    printf("Execution error: INC undef.");
                 }             
                 break;  
            case DEC:
                 if( type( memory[tid][avalue(arg)] ) == T_FLOAT ) {
                    fvalue( memory[tid][avalue(arg)] )--;
                 } else if( type( memory[tid][avalue(arg)] ) == T_INT ) {
                    ivalue( memory[tid][avalue(arg)] )--;
                 } else {
                    printf("Execution error: DEC undef.");
                 }             
                 break;
            case ALLOCS: {
                object_t o;
                type( o ) = T_STRING;
                svalue( o ) = (char *) malloc(sizeof(char)*(strlen(svalue(arg))+1));
                strcpy( svalue(o), svalue(arg) );
                push( &o, tid );                 
                }
                break;
                  
            case APPEND:
                {
                object_t v = concat(stack[top[tid] - 2], stack[top[tid] - 1] );
                
                if( type( stack[top[tid] - 2] ) == T_STRING ) {
                    free( svalue( stack[top[tid] - 2] ));
                    svalue( stack[top[tid] - 2] ) = 0;
                }
                
                if( type( stack[top[tid] - 1] ) == T_STRING ) {
                    free( svalue( stack[top[tid] - 1] ));
                    svalue( stack[top[tid] - 1] ) = 0;
                }
                
                stack[top[tid] - 2] = v;               
                top[tid]--;
                
                }
                break;
//            case READ_INT: {

//                   int r = scanf("%d", &ivalue( stack[top[tid] - 1] ) );
//                    type( stack[top[tid] - 1] ) = T_INT;
//                }
//                break;
//            case READ_FLOAT:
//                {
//                scanf("%f", &fvalue( stack[top[tid] - 1] ) );
//                type( stack[top[tid] - 1] ) = T_FLOAT;
//                break;
//            case READ_STRING:
//                #ifdef WIN32
//                    fflush( stdin );
//                #else
//                    __fpurge( stdin );
//                #endif
//                svalue( stack[top[tid] - 1] ) = (char *) malloc( sizeof( char ) * (avalue(arg) + 1) );
//                fgets( svalue( stack[top[tid] - 1] ), avalue(arg) + 1, stdin );
//                type( stack[top[tid] - 1] ) = T_STRTOREL;
//                break;
            
            case JMP_FALSE:            
                // printf("JMP FALSE %d top : %d\n", avalue(arg), top);                
                top[tid]--;
                if( ivalue( stack[top[tid]] ) == 0 )
                    pc[tid] = ivalue( arg );                
                break;
            case HALT: {
//                printf("HAAAALT\n");
                int i = 0;
                for( i = 1; i < N_THREADS; i++ ) {
                    if( pc[i] != -1 ) {                                             
                        //printf("******************* Join Thread %d!\n", i);                     
                        // pthread_join( thread_hand[i], NULL);                    
                    }                                    
                }

                // for( i = 0; i <  mem_curr_size; i++ ) {
                //     release_mem( i, tid );
                // }
                    
                break;
            }   
            default:
                printf("instruction not found: %d\n", opcode);            
                opcode = HALT;
                
        }
    
    
    } while( opcode != HALT );
}

void trace_code() {
    int pc = 0;
    object_t arg;
    enum code_ops opcode;    
    printf("=-=-=- CODE TRACE -=-=-=-=\n");
    do {
        instruction_t ir = code[pc++];
        opcode = ir.operation;
        arg = ir.arg;
        switch( opcode ) {
            case STORE:
                printf("%d [%d]\tSTORE %d\n", (pc-1), STORE, avalue( arg ) );
                break;
            case STOREV:
                printf("%d [%d]\tSTOREV %d\n", (pc-1), STOREV, avalue( arg ) );
                break;
            case STORE_REF:
                printf("%d [%d]\tSTORE_REF %d\n", (pc-1), STORE_REF, avalue( arg ) );
                break;

            case LOAD:
                printf("%d [%d]\tLOAD %d\n", (pc-1), LOAD, avalue( arg ) );
                break;
            case LOADV:
                printf("%d [%d]\tLOADV %d\n", (pc-1), LOADV, avalue( arg ) );
                break;
            case ADD:
                printf("%d [%d]\tADD\n", (pc-1), ADD);
                break;
            case MUL:
                printf("%d [%d]\tMUL\n", (pc-1), MUL);
                break;                                  
            case SUB:
                printf("%d [%d]\tSUB\n", (pc-1), SUB);
                break;
                               
            case PUSH:
                if( type(arg) == T_INT )
                    printf("%d [%d]\tPUSH (const value) %d\n", (pc-1), PUSH, ivalue( arg ));
                else if(type(arg) == T_FLOAT)
                    printf("%d [%d]\tPUSH (const value) %f\n", (pc-1), PUSH, fvalue( arg ));
                else if(type(arg) == T_STRING)    
                    printf("%d [%d]\tPUSH (const value) %s\n", (pc-1), PUSH, svalue( arg ));
                else if(type(arg) == T_ADDR)    
                    printf("%d [%d]\tPUSH %u\n", (pc-1), PUSH, avalue( arg ));
                else
                    printf("%d [%d]\tPUSH (const value) %u\n", (pc-1), PUSH,avalue( arg ) );
                    
                break;                
            case GOTO_ST:
                printf("%d [%d]\tGOTO_ST\n", (pc-1), GOTO_ST);
                break;
            case LDADDR:
                printf("%d [%d]\tLDADDR \n", (pc-1), LDADDR );
                break;
            case CALL:
                printf("%d [%d]\tCALL %u\n", (pc-1), CALL, avalue( arg ) );                
                break;
            case POP:
                printf("%d [%d]\tPOP\n", (pc-1), POP);
                break;
            case WRITE:
                printf("%d [%d]\tPRINT\n", (pc-1), WRITE);
                break;
            case EQ:
                printf("%d [%d]\tEQ\n", (pc-1), EQ );
                break;
            case GOTO:
                printf("%d [%d]\tGOTO %d\n", (pc-1), GOTO, avalue(arg));
                break;
            case APPEND:
                printf("%d [%d]\tAPPEND\n", (pc-1), APPEND);
                break;
            case INC:
                printf("%d [%d]\tINC %d\n", (pc-1), INC, avalue(arg));
                break;
            case GT:
                printf("%d [%d]\tGREATER\n", (pc-1), GT);
                break;
                
            case GEQ:
                printf("%d [%d]\tGREATER EQUAL\n", (pc-1), GEQ);
                break;
            case LEQ:
                printf("%d [%d]\tLESS EQUAL\n", (pc-1), LEQ);
                break;
            case JMP_FALSE:
                printf("%d [%d]\tJMP FALSE %d\n", (pc-1), JMP_FALSE, avalue(arg) );
                break;
            case HALT:
                printf("%d [%d]\tHALT\n", (pc-1), HALT);
                break;
            case MOD:
                printf("%d [%d]\tMOD\n", (pc-1), MOD);
                break;
            case DEC:
                printf("%d [%d]\tDEC\n", (pc-1), DEC);
                break;
            case LT:
                printf("%d [%d]\tLT\n", (pc-1), LT);
                break;
            case NEQ:
                printf("%d [%d]\tNEQ\n", (pc-1), NEQ);
                break;
            case ANDL:
                printf("%d [%d]\tANDL\n", (pc-1), ANDL);
                break;
             case CALL_C:
                printf("%d [%d]\tCALL_C\n", (pc-1), CALL_C);
                break;
            case PUSH_PC:
                printf("%d [%d]\tPUSH_PC %d\n", (pc-1), PUSH_PC, avalue(arg) );
                break;
            case CALL_THREAD:
                printf("%d [%d]\tCALL_THREAD %d %d %d %d\n", (pc-1), PUSH_PC, evalue(arg)[0], evalue(arg)[1],evalue(arg)[2],evalue(arg)[3] );
                break;
            case READ_INT:
            
                printf("%d [%d]\tREAD_INT %d\n", (pc-1), READ_INT, avalue(arg) );
                break;
            case SIGCH:
                printf("%d [%d]\tSIGCH\n", (pc-1), SIGCH );
                break;
            case READ_STRING:
                printf("%d [%d]\tREAD_STRING %d\n", (pc-1), READ_INT, avalue(arg) );
                break;
            case RELMEM:
                printf("%d [%d]\tRELMEM %d\n", (pc-1), RELMEM, avalue(arg) );
                break;
            case ALLOCV:
                printf("%d [%d]\tALLOCV %d\n", (pc-1), ALLOCV, ivalue(arg) );
                break;
            case ORL:
                printf("%d [%d]\tORL\n", (pc-1), ORL);
                break;
            case NEGL:
                printf("%d [%d]\tNEGL\n", (pc-1), ORL);
                break;
             case ALLOCS:
                printf("%d [%d]\tALLOCS (%s)\n", (pc-1), ALLOCS, svalue(arg));
                break;    
            default:
                printf("NOP %d %d\n", opcode, avalue(arg) );
                
        }
    
    
    } while( opcode != HALT );
    
    printf("=-=-=-=-=-=-=-=--=--=-=-=-\n");
}

void print_info_source() {
    int i = 0;
    printf("INFO SOURCE\n");
    for( i = 0; code[i].operation != HALT; i++ ) {
        printf("%d [%d]\t %s\t %d\n",info[i].file,i , file_names_hist[info[i].file], info[i].line);
    }
}
