#ifndef __GENCODE_H__
#define __GENCODE_H__

#include "libneblina.h"
#include "neblina_sym.h"
#include "neblina_io.h"
#include "neblina_std.h"
#include "clutils.h"
#include "neblina_list.h"
#define push(v,tid) stack[top[tid]++] = *v;
#define pop(tid) stack[--top[tid]];
#define MAX_INCLUDE_DEPTH 100
#define N_THREADS 1024
extern int                       pc[N_THREADS];
extern int    rel_params;

#define ISNUMTYPE(v) (v == T_INT || v == T_FLOAT) ? 1 : 0
extern instruction_t  *  code;
extern info_source_t  *  info;

extern int               code_curr_size;
extern int               pos;
extern object_t          stack[1024];
extern unsigned int  *   stack_addr;
extern int               top_addr;
extern unsigned int      stack_addr_curr_size;
extern int               top[N_THREADS];
extern int curr_line[MAX_INCLUDE_DEPTH];
extern int current_file;
extern int include_stack_ptr;
extern char  file_names_hist[MAX_INCLUDE_DEPTH][256];





char * nebgetline( int hist_file, int nline ) ;

void runerror( char * strerr );
void init_machine();

void finalize_machine();
void realloc_code();

char * get_str( object_t v );
object_t concat( object_t v1, object_t v2 );



void print_stack();


void bytecode( enum code_ops operation, object_t arg );
void ibytecode( enum code_ops operation, unsigned int arg );
void rbytecode( enum code_ops operation, object_t * arg );
void ibytecode_addr( enum code_ops operation, unsigned int arg, int addr );
int cur_location();
int reserve_location() ;
 void release_mem( int pos, int tid ) ;
void execute();
void trace_code();
void print_info_source();
#endif
