#ifndef __NEBLINAIOPACK__
#define __NEBLINAIOPACK__
#include "package.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libneblina.h"
extern int doublePrintPlaces;

extern char formatFloat[33][16];
/**
 * IO Package
 */
package_t * getIOPackage();
void runerror( char * strerr );
void ** neblina_open  ( void ** i, int * status );
void ** neblina_close ( void ** i, int * status );
void ** neblina_write( void ** i, int * status );
void ** neblina_writeln( void ** i, int * status );
void ** neblina_write_stdout( void ** i, int * status );
void ** neblina_writeln_stdout( void ** i, int * status );
void ** neblina_read( void ** i, int * status );
void ** neblina_fmtdouble( void ** i, int * status );
void print_value( const object_t * v ) ;
int print_value_file( const object_t * v, FILE * file, unsigned char LN );
void * scanf_int();
object_t * read_value_file1( object_t * v, FILE * file );
object_t * read_value_file( object_t * v, FILE * file ) ;

#endif
