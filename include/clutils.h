#ifndef ____cl_utils
#define ____cl_utils
#ifdef	__cplusplus
extern "C" {
#endif

#include <CL/opencl.h>

#define DEBUGCL  0
#define NKERNELS 27

#define KERVETADD        0
#define KERVETSUB        1
#define KERVETMSC        2
#define KERMATVECMUL3    3
#define KERMATTRANSP     4 
#define KERVETSUM        5
#define KERMATMUL        6
#define KERVETNORM       7
#define KERVETDOT        8
#define KERMATMUL2       9
#define KERMATVECMUL3COMPLEX    10
#define KERMATVECMULSPARSE    11
#define KERMATVECMULSPARSECOMPLEX    12
#define KERLUDECOMP    13
#define KERVETPROD      14
#define KERVETCOMPLEXPROD      15
#define KERVETCONJ         16
#define KERVETADDOFF2      17
#define KERVETADDOFF      18
#define KERMATMATMUL      19
#define KERVETDOTCOMPLEX  20
#define KERVETCONJUGATE   21
#define KERMATMULCOMPLEX  22
#define KETMATMULCOMPLEX 23
#define KERMATADDCOMPLEX 24
#define KERMATMULSCROW 25
#define KERMATMULSCCOL 26
#define KER_MUL_COMPLEX_SCALAR_FLOAT_VET        27
#define KER_MUL_COMPLEX_SCALAR_COMPLEX_VET        28
#define KER_MUL_FLOAT_SCALAR_COMPLEX_VET        29
#define KER_MAT_ADD_FLOAT_COMPLEX 30
#define KER_VET_SUB_COMPLEX        31

#if DEBUGCL
    #define CLERR checkErrors (status, __FILE__, __LINE__); 
#else
    #define CLERR checkErrors (status, __FILE__, __LINE__);
#endif 

#define LOCDEV 128
#define LOCHOS 1


typedef struct __cl_info {
    cl_program        p; 
    cl_command_queue  q; 
    cl_context        c;
    cl_uint           n;   // n compute units
    cl_device_id      d;   // current device
    cl_uint           fp64;                    
} cl_info;

cl_device_type proctype;
int id_device;

cl_info clinfo;
cl_kernel clkernels[NKERNELS];

int nkernelsRmat;
cl_kernel clkernelsRmat[32];
char strkernelsRmat[32][4096];
char rmatstat[32];


cl_info GetCLInfo( char * filename );
                      /*         IN               IN                    OUT */
cl_info GetCLInfoRMat( char * strKernel, char * kernelName, cl_kernel * kernel );
size_t roundUp(int group_size, int global_size);
void checkErrors(cl_int status, char *label, int line);
char* filetobuf(char *file);
cl_info GetCLInfo( char * filename );
void ReleaseCLInfo( cl_info info );
void InitCLEngine(int device);

/**
 * modified code from http://dhruba.name/2012/08/14/opencl-cookbook-listing-all-devices-and-their-critical-attributes/
 */
 void showDevicesList();

#define RMATKERNAME "rmatVecMulComplex%d"
#define RMATKER "#pragma OPENCL EXTENSION cl_amd_printf : enable \n\
    __kernel void rmatVecMulComplex%d(__global real * out, __global real * v, int ncols, int nrows, __local real * pDotR, __local real * pDotC ) { \n \
    int i = get_group_id(0); \n \
    int n = get_num_groups(0); \n \
    uint j, t; \n \
    for (; i < nrows; i +=n ) { \n \
        real sumR = 0; \n \
        real sumC = 0; \n \
        for ( j = get_local_id(0); j < ncols; j += get_local_size(0)) { \n \
            real re = 0; \n \
            real im = 0; \n \
            %s \n \
            sumR += re * v[2*j] - im * v[2*j+1]; \n \
            sumC += re * v[2*j+1] + im * v[2*j]; \n \
        } \n \
        pDotR[get_local_id(0)] = sumR; \n \
        pDotC[get_local_id(0)] = sumC; \n \
        barrier(CLK_LOCAL_MEM_FENCE); \n \
        for (t=1;t<get_local_size(0); t*=2) { \n \
            barrier(CLK_LOCAL_MEM_FENCE); \n \
            uint idx = 2 * t * get_local_id(0); \n \
            if (idx < get_local_size(0)) { \n \
                pDotR[idx] += pDotR[idx+t]; \n \
                pDotC[idx] += pDotC[idx+t]; \n \
            }     \n \
        } \n \
        if (get_local_id(0) == 0) { \n \
            out[2*i] = pDotR[0]; \n \
            out[2*i+1] = pDotC[0]; \n \
        } \n \
        barrier(CLK_LOCAL_MEM_FENCE); \n \
	} \n \
}" 

 #ifdef	__cplusplus
}
#endif

#endif
