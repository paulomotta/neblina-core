bin_PROGRAMS = neblina
neblina_SOURCES =  clutils.c lex.yy.c libneblina.c neblina_io.c neblina.tab.c package.c neblina_list.c neblina_packages.c neblina_vm.c neblina_str.c neblina_sym.c neblina_math.c neblina_time.c neblina_std.c
EXTRA_DIST = clutils.h libneblina.h neblina_io.h neblina.h neblina.tab.h package.h neblina_list.h neblina_packages.h neblina_vm.h neblina_str.h neblina_sym.h neblina_math.h neblina_time.h neblina_std.h vector.cl neblina.l neblina.y oclvector.h CL mathext.nbl
cl_dir = /etc/neblina
AM_CFLAGS = -O5
CFLAGS =

cl__DATA = vector.cl \
           mathext.nbl
neblina_LDADD = -lOpenCL -lm
