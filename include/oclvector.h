#ifndef __NEBLINAVECTOROCL
#define __NEBLINAVECTOROCL
#define  BLOCK_DIM 16

#include <CL/opencl.h>
#include <math.h>
#include "clutils.h"
#include "libneblina.h"
#include "neblina.h"


double * addVector( double * v1, double * v2, int n ) {
    cl_int status;
    cl_mem v1Dev, v2Dev, outDev;
    double * out = (double *) malloc( n * sizeof(double) );
    cl_kernel kernel = clkernels[KERVETADD];
    //CLERR
    outDev = clCreateBuffer (clinfo.c, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, n*sizeof(double), out, &status);
    CLERR    
    v1Dev = clCreateBuffer (clinfo.c, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, n*sizeof(double), v1, &status);
    CLERR
    v2Dev = clCreateBuffer (clinfo.c, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, n*sizeof(double), v2, &status);
    CLERR   
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(v1Dev), &v1Dev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(v2Dev), &v2Dev);
    CLERR
    size_t globalWorkSize = n;
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR
            
    status = clEnqueueReadBuffer(clinfo.q, outDev, CL_TRUE, 0, n*sizeof(double), out, 0, NULL, NULL);
    CLERR

    status = clFinish(clinfo.q);
    return out;
}
/**   PUT IN A L*U WHERE 
 *    L diagonal is 1
 *        
 */ 
void luDecomp( cl_mem ADev, int n ) {
    cl_int status;
    cl_kernel kernel = clkernels[KERLUDECOMP]; 
    /*
    ADev = clCreateBuffer (clinfo.c, CL_MEM_USE_HOST_PTR, n*n*sizeof(double), A, &status);
    CLERR
    */
    status = clSetKernelArg (kernel, 0, sizeof(ADev), &ADev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(int), (void *)&n);
    CLERR
    size_t globalWorkSize = (n-1);
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, &globalWorkSize, 0, NULL, NULL);
    CLERR
    status = clFinish(clinfo.q);
}

cl_mem addVectorF(cl_mem v1Dev, cl_mem v2Dev, int n ) {
    
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KERVETADD];
    outDev = clCreateBuffer(clinfo.c, CL_MEM_WRITE_ONLY, n*sizeof(double), NULL, &status);
    CLERR    
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(v1Dev), &v1Dev);
    CLERR
    
    status = clSetKernelArg (kernel, 2, sizeof(v2Dev), &v2Dev);
    CLERR
    size_t globalWorkSize = n;
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR 
    status = clFinish(clinfo.q);
    CLERR 
    return outDev;
}

cl_mem addVectorC(cl_mem v1Dev, cl_mem v2Dev, int n ) {
    
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KERMATADDCOMPLEX];
    outDev = clCreateBuffer(clinfo.c, CL_MEM_WRITE_ONLY, 2 * n*sizeof(double), NULL, &status);
    CLERR    
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(v1Dev), &v1Dev);
    CLERR
    
    status = clSetKernelArg (kernel, 2, sizeof(v2Dev), &v2Dev);
    CLERR
    size_t globalWorkSize = n;
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR 
    status = clFinish(clinfo.q);
    CLERR 
    return outDev;
}

cl_mem addVectorFC(cl_mem v1Dev, cl_mem v2Dev, int n ) {
    
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KER_MAT_ADD_FLOAT_COMPLEX];
    outDev = clCreateBuffer(clinfo.c, CL_MEM_WRITE_ONLY, 2 * n*sizeof(double), NULL, &status);
    CLERR    
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(v1Dev), &v1Dev);
    CLERR
    
    status = clSetKernelArg (kernel, 2, sizeof(v2Dev), &v2Dev);
    CLERR
    size_t globalWorkSize = n;
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR 
    status = clFinish(clinfo.q);
    CLERR 
    return outDev;
}

cl_mem prodVector(cl_mem v1Dev, cl_mem v2Dev, int n ) {
    
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KERVETPROD];
    outDev = clCreateBuffer(clinfo.c, CL_MEM_WRITE_ONLY, n*sizeof(double), NULL, &status);
    CLERR    
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(v1Dev), &v1Dev);
    CLERR
    
    status = clSetKernelArg (kernel, 2, sizeof(v2Dev), &v2Dev);
    CLERR
    size_t globalWorkSize = n;
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR 
    status = clFinish(clinfo.q);
    CLERR 
    return outDev;
}

cl_mem vecConj(cl_mem v1Dev, int n ) {
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KERVETCONJ];
    outDev = clCreateBuffer(clinfo.c, CL_MEM_WRITE_ONLY, n*sizeof(double), NULL, &status);
    CLERR    
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(v1Dev), &v1Dev);
    CLERR
    size_t globalWorkSize = n;
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR 
    status = clFinish(clinfo.q);
    CLERR 
    return outDev;
}

cl_mem vecConjugate(cl_mem v1Dev, int n ) {
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KERVETCONJUGATE];
    
    outDev = clCreateBuffer(clinfo.c, CL_MEM_WRITE_ONLY, 2*n*sizeof(double), NULL, &status);
    CLERR    
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(v1Dev), &v1Dev);
    CLERR
    size_t globalWorkSize = n;
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR 
    status = clFinish(clinfo.q);
    CLERR 
    return outDev;
}

cl_mem vecAddOff2(cl_mem v1Dev, int n ) {
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KERVETADDOFF2];
    int out_size = n/2;
    outDev = clCreateBuffer(clinfo.c, CL_MEM_WRITE_ONLY, n*sizeof(double), NULL, &status);
    CLERR    
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(v1Dev), &v1Dev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(int), (void *)&out_size);
    CLERR
    size_t globalWorkSize = out_size;
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR 
    status = clFinish(clinfo.q);
    CLERR 
    return outDev;
}

cl_mem vecAddOff(cl_mem v1Dev, int off, int parts ) {
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KERVETADDOFF];
    // printf("BufferSIze = %d\n", off*sizeof(double) );
    outDev = clCreateBuffer(clinfo.c, CL_MEM_WRITE_ONLY, off*sizeof(double), NULL, &status);
    CLERR    
    // printf("Depois do Erro\n");
    
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(v1Dev), &v1Dev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(int), (void *)&off);
    CLERR
    status = clSetKernelArg (kernel, 3, sizeof(int), (void *)&parts);
    CLERR
    size_t globalWorkSize = off;
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR 
    status = clFinish(clinfo.q);
    CLERR 
    return outDev;
}

cl_mem prodComplexVector(cl_mem v1Dev, cl_mem v2Dev, int n ) {
    
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KERVETCOMPLEXPROD];
    outDev = clCreateBuffer(clinfo.c, CL_MEM_WRITE_ONLY, n * COMPLEX_SIZE, NULL, &status);
    CLERR    
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(v1Dev), &v1Dev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(v2Dev), &v2Dev);
    CLERR
    size_t globalWorkSize = n;
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR 
    status = clFinish(clinfo.q);
    CLERR 
    return outDev;
}

cl_mem subVector(cl_mem v1Dev, cl_mem v2Dev, int n ) {
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KERVETSUB];
    outDev = clCreateBuffer(clinfo.c, CL_MEM_WRITE_ONLY, n*sizeof(double), NULL, &status);
    CLERR      
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(v1Dev), &v1Dev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(v2Dev), &v2Dev);
    CLERR
    size_t globalWorkSize = n;
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR    
    status = clFinish(clinfo.q);
    CLERR
    return outDev;
}

cl_mem subVectorC(cl_mem v1Dev, cl_mem v2Dev, int n ) {
    
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KER_VET_SUB_COMPLEX];
    outDev = clCreateBuffer(clinfo.c, CL_MEM_WRITE_ONLY, 2 * n*sizeof(double), NULL, &status);
    CLERR    
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(v1Dev), &v1Dev);
    CLERR
    
    status = clSetKernelArg (kernel, 2, sizeof(v2Dev), &v2Dev);
    CLERR
    size_t globalWorkSize = n;
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR 
    status = clFinish(clinfo.q);
    CLERR 
    return outDev;
}


cl_mem mulScalarVector( cl_mem v1Dev, double scalar, int n ) {
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KERVETMSC];
    outDev = clCreateBuffer (clinfo.c, CL_MEM_WRITE_ONLY, n*sizeof(double), NULL, &status);
    CLERR   
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(v1Dev), &v1Dev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(double), (void *)&scalar);
    CLERR
    size_t globalWorkSize = n;
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR
    status = clFinish(clinfo.q);
    CLERR
    return outDev;
}

cl_mem mulComplexScalarVector( cl_mem v1Dev, double real, double imaginary, int n ) {
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KER_MUL_COMPLEX_SCALAR_FLOAT_VET];
    outDev = clCreateBuffer (clinfo.c, CL_MEM_WRITE_ONLY, 2*n*sizeof(double), NULL, &status);
    CLERR   
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(v1Dev), &v1Dev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(double), (void *)&real);
    CLERR
    status = clSetKernelArg (kernel, 3, sizeof(double), (void *)&imaginary);
    CLERR
    size_t globalWorkSize = n;
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR
    status = clFinish(clinfo.q);
    CLERR
    return outDev;
}

cl_mem mulComplexScalarComplexVector( cl_mem v1Dev, double real, double imaginary, int n ) {
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KER_MUL_COMPLEX_SCALAR_COMPLEX_VET];
    outDev = clCreateBuffer (clinfo.c, CL_MEM_WRITE_ONLY, 2*n*sizeof(double), NULL, &status);
    CLERR   
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(v1Dev), &v1Dev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(double), (void *)&real);
    CLERR
    status = clSetKernelArg (kernel, 3, sizeof(double), (void *)&imaginary);
    CLERR
    size_t globalWorkSize = n;
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR
    status = clFinish(clinfo.q);
    CLERR
    return outDev;
}

cl_mem mulFloatScalarComplexVector( cl_mem v1Dev, double real, int n ) {
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KER_MUL_FLOAT_SCALAR_COMPLEX_VET];
    outDev = clCreateBuffer (clinfo.c, CL_MEM_WRITE_ONLY, 2*n*sizeof(double), NULL, &status);
    CLERR   
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(v1Dev), &v1Dev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(double), (void *)&real);
    CLERR
    size_t globalWorkSize = n;
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR
    status = clFinish(clinfo.q);
    CLERR
    return outDev;
}

void mulScalarMatRow( cl_mem m, double scalar, int nrow, int ncols, int row) {
    cl_int status;
    cl_kernel kernel = clkernels[KERMATMULSCROW];
    status = clSetKernelArg (kernel, 0, sizeof(m), &m);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(double), (void *)&scalar);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(int), (void *)&ncols);
    CLERR
    status = clSetKernelArg (kernel, 3, sizeof(int), (void *)&row);
    CLERR
    size_t globalWorkSize = nrow;
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR
    status = clFinish(clinfo.q);
    CLERR
}

void mulScalarMatCol( cl_mem m, double scalar, int nrow, int ncols, int col) {
    cl_int status;
    cl_kernel kernel = clkernels[KERMATMULSCCOL];
    status = clSetKernelArg (kernel, 0, sizeof(m), &m);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(double), (void *)&scalar);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(int), (void *)&ncols);
    CLERR
    status = clSetKernelArg (kernel, 3, sizeof(int), (void *)&col);
    CLERR
    size_t globalWorkSize = nrow;
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR
    status = clFinish(clinfo.q);
    CLERR
}


cl_mem matVecMul1(  cl_mem mDev, cl_mem vDev, int ncols, int nrows ) {
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KERMATVECMUL3];
    CLERR
    outDev = clCreateBuffer (clinfo.c, CL_MEM_WRITE_ONLY, nrows*sizeof(double), NULL, &status);
    CLERR    
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(mDev), &mDev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(vDev), &vDev);
    CLERR
    status = clSetKernelArg (kernel, 3, sizeof(int), (void *)&ncols);
    CLERR
    status = clSetKernelArg (kernel, 4, sizeof(int), (void *)&nrows);
    CLERR
    size_t globalWorkSize = nrows;
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR
    status = clFinish(clinfo.q);
    CLERR
    return outDev;
}

cl_mem matVecMul2(  cl_mem mDev, cl_mem vDev, int ncols, int nrows ) {
    cl_int status;
    cl_mem outDev, pDotDev;
    cl_kernel kernel = clkernels[KERMATVECMUL3];
    size_t localWorkSize = 256;
    size_t globalWorkSize = 2 * clinfo.n * localWorkSize;    
    outDev = clCreateBuffer (clinfo.c, CL_MEM_WRITE_ONLY, nrows*sizeof(double), NULL, &status);
    CLERR    
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(mDev), &mDev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(vDev), &vDev);
    CLERR
    status = clSetKernelArg (kernel, 3, sizeof(int), (void *)&ncols);
    CLERR
    status = clSetKernelArg (kernel, 4, sizeof(int), (void *)&nrows);
    CLERR
    status = clSetKernelArg (kernel, 5, localWorkSize* sizeof(double), NULL);
    CLERR    
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, &localWorkSize, 0, NULL, NULL);
    CLERR
    status = clFinish(clinfo.q);
    CLERR
    return outDev;
}

cl_mem matMul(  cl_mem m1Dev, cl_mem m2Dev, int nrows, int ncols, int qq, int atype, int btype ) {
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel;
    int ss;
    if( atype == T_FLOAT && btype == T_FLOAT) {
        kernel = clkernels[KERMATMUL];
        ss = nrows*ncols*sizeof(double);
    } else if( atype == T_COMPLEX && btype == T_COMPLEX ) {
        kernel = clkernels[KERMATMULCOMPLEX];
        ss = 2 * nrows*ncols*sizeof(double);    
    } else if( atype == T_FLOAT && btype == T_COMPLEX ) {
        kernel = clkernels[KETMATMULCOMPLEX];
        ss = 2 * nrows*ncols*sizeof(double); 
    }
        
    size_t globalWorkSize [] = { nrows, ncols };
    
    outDev = clCreateBuffer (clinfo.c, CL_MEM_WRITE_ONLY, ss, NULL, &status);
    CLERR    
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(m1Dev), &m1Dev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(m2Dev), &m2Dev);
    CLERR
    status = clSetKernelArg (kernel, 3, sizeof(int), (void *)&qq);
    CLERR
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 2, NULL, globalWorkSize, NULL, 0, NULL, NULL);
    CLERR
    status = clFinish(clinfo.q);
    CLERR
    return outDev;
}

cl_mem matMul2(  cl_mem m1Dev, cl_mem m2Dev, int nrows, int ncols, int qq ) {
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KERMATMUL2];
    size_t globalSize[2];
    size_t localSize[2];   
    localSize[0] = BLOCK_DIM;
    localSize[1] = BLOCK_DIM;
    globalSize[0] = roundUp(nrows, BLOCK_DIM);
    globalSize[1] = roundUp(ncols, BLOCK_DIM);
    
    outDev = clCreateBuffer (clinfo.c, CL_MEM_WRITE_ONLY, nrows*ncols*sizeof(double), NULL, &status);
    CLERR    
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(m1Dev), &m1Dev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(m2Dev), &m2Dev);
    CLERR
    status = clSetKernelArg (kernel, 3, sizeof(int), (void *)&qq);
    CLERR
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 2, NULL, globalSize, localSize, 0, NULL, NULL);
    CLERR
    status = clFinish(clinfo.q);
    CLERR
    return outDev;
}

void matSquare( cl_mem * outLin, cl_mem * idxOutLin, 
                cl_mem * outCol, cl_mem * idxOutCol, 
                cl_mem mLin, cl_mem idxLin, 
                cl_mem mCol, cl_mem idxCol, 
                int maxcols, int N ) {
    cl_int status;
    cl_kernel kernel = clkernels[KERMATMATMUL];
    cl_mem count, sem;
    int i, ptrcount[N];
    size_t globalWorkSize = N;
    for(i = 0; i < N; i++ ) {
        ptrcount[i] = 0;
    }
    count = clCreateBuffer( clinfo.c,  CL_MEM_USE_HOST_PTR, N* sizeof(int), ptrcount, &status);
    CLERR
    sem   = clCreateBuffer( clinfo.c,  CL_MEM_USE_HOST_PTR, N* sizeof(int), ptrcount, &status);
    CLERR
    status = clSetKernelArg (kernel, 0, sizeof(outLin), outLin);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(idxOutLin), idxOutLin);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(int), (void *)&maxcols);
    CLERR
    
    status = clSetKernelArg (kernel, 3, sizeof(outCol), outCol);
    CLERR
    status = clSetKernelArg (kernel, 4, sizeof(idxOutCol), idxOutCol);
    CLERR
    status = clSetKernelArg (kernel, 5, sizeof(int), (void *)&maxcols);
    CLERR
    
    
    status = clSetKernelArg (kernel, 6, sizeof(mLin), &mLin);
    CLERR
    status = clSetKernelArg (kernel, 7, sizeof(idxLin), &idxLin);
    CLERR
    status = clSetKernelArg (kernel, 8, sizeof(int), (void *)&maxcols);
    CLERR
    
    status = clSetKernelArg (kernel, 9, sizeof(mCol), &mCol);
    CLERR
    status = clSetKernelArg (kernel, 10, sizeof(idxCol), &idxCol);
    CLERR
    status = clSetKernelArg (kernel, 11, sizeof(int), (void *)&maxcols);
    CLERR
    
    status = clSetKernelArg (kernel, 12, sizeof(count), &count);
    CLERR
    status = clSetKernelArg (kernel, 13, sizeof(sem), &sem);
    CLERR
    
    status = clSetKernelArg (kernel, 14, sizeof(int), (void *)&N);
    CLERR
    
    
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR
    status = clFinish(clinfo.q);
    CLERR
}

cl_mem matVecMul3(  cl_mem mDev, cl_mem vDev, int ncols, int nrows ) {
    cl_int status;
    cl_mem outDev, pDotDev;
    cl_kernel kernel = clkernels[KERMATVECMUL3];
    size_t localWorkSize = 256;
    size_t globalWorkSize = 2 * clinfo.n * localWorkSize;   
    outDev = clCreateBuffer(clinfo.c, CL_MEM_WRITE_ONLY, nrows*sizeof(double), NULL, &status);
    CLERR   
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(mDev), &mDev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(vDev), &vDev);
    CLERR
    status = clSetKernelArg (kernel, 3, sizeof(int), (void *)&ncols);
    CLERR
    status = clSetKernelArg (kernel, 4, sizeof(int), (void *)&nrows);
    CLERR
    status = clSetKernelArg (kernel, 5, localWorkSize* sizeof(double), NULL);
    CLERR    
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, &localWorkSize, 0, NULL, NULL);
    CLERR
    status = clFinish(clinfo.q);
    CLERR
    return outDev;
}

cl_mem sparseVecMul(cl_mem mDev, cl_mem idxCol, cl_mem vDev, int nrows, int maxCols ) {
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KERMATVECMULSPARSE];
    size_t globalWorkSize = nrows;   
    outDev = clCreateBuffer(clinfo.c, CL_MEM_WRITE_ONLY, nrows*sizeof(double), NULL, &status);
    CLERR   
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(mDev), &mDev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(idxCol), &idxCol);    
    CLERR
    status = clSetKernelArg (kernel, 3, sizeof(vDev), &vDev);
    CLERR  
    status = clSetKernelArg (kernel, 4, sizeof(int), (void *)&nrows);
    CLERR
    status = clSetKernelArg (kernel, 5, sizeof(int), (void *)&maxCols);
    CLERR
    // printf("GLOB = %d", globalWorkSize );
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR
    status = clFinish(clinfo.q);
    CLERR
    return outDev;
}
cl_mem sparseComplexVecMul(cl_mem mDev, cl_mem idxCol, cl_mem vDev, int nrows, int maxCols ) {
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KERMATVECMULSPARSECOMPLEX];
    size_t globalWorkSize = nrows;   
    outDev = clCreateBuffer(clinfo.c, CL_MEM_WRITE_ONLY, 2*nrows*sizeof(double), NULL, &status);
    CLERR   
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(mDev), &mDev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(idxCol), &idxCol);    
    CLERR
    status = clSetKernelArg (kernel, 3, sizeof(vDev), &vDev);
    CLERR  
    status = clSetKernelArg (kernel, 4, sizeof(int), (void *)&nrows);
    CLERR
    status = clSetKernelArg (kernel, 5, sizeof(int), (void *)&maxCols);
    CLERR
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    CLERR
    status = clFinish(clinfo.q);
    CLERR
    return outDev;
}

cl_mem matVecMul3Complex(  cl_mem mDev, cl_mem vDev, int ncols, int nrows ) {
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = clkernels[KERMATVECMUL3COMPLEX];
    size_t localWorkSize = 256;
    size_t globalWorkSize = 2 * clinfo.n * localWorkSize;   
    outDev = clCreateBuffer(clinfo.c, CL_MEM_WRITE_ONLY, 2*nrows*sizeof(double), NULL, &status);
    CLERR   
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(mDev), &mDev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(vDev), &vDev);
    CLERR
    status = clSetKernelArg (kernel, 3, sizeof(int), (void *)&ncols);
    CLERR
    status = clSetKernelArg (kernel, 4, sizeof(int), (void *)&nrows);
    CLERR
    status = clSetKernelArg (kernel, 5, localWorkSize * sizeof(double), NULL);
    CLERR
    status = clSetKernelArg (kernel, 6, localWorkSize * sizeof(double), NULL);
    CLERR    
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, &localWorkSize, 0, NULL, NULL);
    CLERR
    status = clFinish(clinfo.q);
    CLERR
    return outDev;
}



cl_mem matTranspose(  cl_mem mDev, int ncols, int nrows ) {
    cl_int status;
    cl_mem outDev, pDotDev;
    cl_kernel kernel = clkernels[KERMATTRANSP];
    size_t globalSize[2];
    size_t localSize[2];   
    localSize[0] = BLOCK_DIM;
    localSize[1] = BLOCK_DIM;
    globalSize[0] = roundUp(ncols, BLOCK_DIM);
    globalSize[1] = roundUp(nrows, BLOCK_DIM);
    
    outDev = clCreateBuffer (clinfo.c, CL_MEM_WRITE_ONLY, ncols * nrows*sizeof(double), NULL, &status);
    CLERR   
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(mDev), &mDev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(int), (void *)&ncols);
    CLERR
    status = clSetKernelArg (kernel, 3, sizeof(int), (void *)&nrows);
    CLERR
    status = clSetKernelArg (kernel, 4, (BLOCK_DIM)*(BLOCK_DIM+1)* sizeof(double), NULL);
    CLERR    
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 2, NULL, globalSize, localSize, 0, NULL, NULL);
    CLERR
    status = clFinish(clinfo.q);
    CLERR
    return outDev;
}

double sumVector( cl_mem vDev, int len ) {
    cl_int status;
    cl_mem tmpDev, outDev;
    double out = 1;
    cl_kernel kernel = clkernels[KERVETSUM];
    size_t localSize = 64;
    size_t globalSize = roundUp( len, 64 );
    
    tmpDev = clCreateBuffer(clinfo.c, CL_MEM_READ_WRITE, (globalSize/localSize)*sizeof(double), NULL, &status);
    CLERR
    outDev = clCreateBuffer(clinfo.c, CL_MEM_READ_WRITE , sizeof(double), NULL, &status);
    CLERR    
    status = clSetKernelArg (kernel, 0, sizeof(vDev), &vDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(tmpDev), &tmpDev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(int), (void *) &len);
    CLERR
    status = clSetKernelArg (kernel, 3, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 4, localSize * sizeof(double), NULL );
    CLERR
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalSize, &localSize, 0, NULL, NULL);
    CLERR
    // status = clEnqueueReadBuffer (clinfo.q, outDev, CL_TRUE, 0, sizeof(double), &out, 0, NULL, NULL);
    // CLERR
    int lenOut = globalSize / localSize;
    
    //double * outToSum = (double *) malloc( lenOut * sizeof(double));
    // printf("len = %d out = %p\n", lenOut, outToSum );
    //status = clEnqueueReadBuffer(clinfo.q, tmpDev, CL_TRUE, 0, lenOut*sizeof(double), outToSum, 0, NULL, NULL);
    //CLERR
     if( clinfo.fp64 ) {
        double * outToSum = (double *) malloc( lenOut * sizeof(double));
        status = clEnqueueReadBuffer(clinfo.q, tmpDev, CL_TRUE, 0, lenOut*sizeof(double), outToSum, 0, NULL, NULL);
        CLERR
        int ii;
        out = 0.0;
        #pragma omp parallel for reduction (+:out)
        for( ii=0; ii<lenOut; ii++) {            
            out += outToSum[ii];
        }
        free( outToSum );
    } else {
        float * outToSum = (float *) malloc( lenOut * sizeof(float));
        status = clEnqueueReadBuffer(clinfo.q, tmpDev, CL_TRUE, 0, lenOut*sizeof(float), outToSum, 0, NULL, NULL);
        CLERR
        int ii;
        out = 0.0;
        #pragma omp parallel for reduction (+:out)
        for( ii=0; ii<lenOut; ii++) {            
            out += outToSum[ii];
        }
        free( outToSum );
        }
    clReleaseMemObject(tmpDev);
    clReleaseMemObject(outDev);
    // printf("out = %f\n", out );
    return out;
}

double normVector( cl_mem vDev, int len ) {
    cl_int status;
    cl_mem tmpDev, outDev;
    double out = 0;
    cl_kernel kernel = clkernels[KERVETNORM];
    size_t localSize = 64;
    size_t globalSize = roundUp( len, 64 );
    
    tmpDev = clCreateBuffer(clinfo.c, CL_MEM_READ_WRITE, (globalSize/localSize)*sizeof(double), NULL, &status);
    CLERR
    outDev = clCreateBuffer(clinfo.c, CL_MEM_READ_WRITE , sizeof(double), NULL, &status);
    CLERR    
    status = clSetKernelArg (kernel, 0, sizeof(vDev), &vDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(tmpDev), &tmpDev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(int), (void *) &len);
    CLERR
    status = clSetKernelArg (kernel, 3, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 4, localSize * sizeof(double), NULL );
    CLERR
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalSize, &localSize, 0, NULL, NULL);
    CLERR
    int lenOut = globalSize / localSize;
    
    if( clinfo.fp64 ) {
        double * outToSum = (double *) malloc( lenOut * sizeof(double));
        status = clEnqueueReadBuffer(clinfo.q, tmpDev, CL_TRUE, 0, lenOut*sizeof(double), outToSum, 0, NULL, NULL);
        CLERR
        int ii;
        out = 0.0;
        #pragma omp parallel for reduction (+:out)
        for( ii=0; ii<lenOut; ii++) {            
            out += outToSum[ii];
        }
        free( outToSum );
    } else {
        float * outToSum = (float *) malloc( lenOut * sizeof(float));
        status = clEnqueueReadBuffer(clinfo.q, tmpDev, CL_TRUE, 0, lenOut*sizeof(float), outToSum, 0, NULL, NULL);
        CLERR
        int ii;
        out = 0.0;
        #pragma omp parallel for reduction (+:out)
        for( ii=0; ii<lenOut; ii++) {            
            out += outToSum[ii];
        }
        free( outToSum );
    }
    clReleaseMemObject( tmpDev );
    clReleaseMemObject( outDev );
    return sqrt( out );
}


double dotVector(cl_mem v1Dev, cl_mem v2Dev, int len ) {
    cl_int status;
    cl_mem tmpDev, outDev;
    double out = 0;
    cl_kernel kernel = clkernels[KERVETDOT];
    size_t localSize = 1024;
    size_t globalSize = roundUp( len, 1024 );
    //printf("global size : %d\n",globalSize); 
    tmpDev = clCreateBuffer(clinfo.c, CL_MEM_READ_WRITE, (globalSize/localSize)*sizeof(double), NULL, &status);
    CLERR
    outDev = clCreateBuffer(clinfo.c, CL_MEM_READ_WRITE , sizeof(double), NULL, &status);
    CLERR    
    status = clSetKernelArg (kernel, 0, sizeof(v1Dev), &v1Dev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(v2Dev), &v2Dev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(tmpDev), &tmpDev);
    CLERR
    status = clSetKernelArg (kernel, 3, sizeof(int), (void *) &len);
    CLERR
    status = clSetKernelArg (kernel, 4, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 5, localSize * sizeof(double), NULL );
    CLERR
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalSize, &localSize, 0, NULL, NULL);
    CLERR
    int lenOut = globalSize / localSize;
    
     if( clinfo.fp64 ) {
        double * outToSum = (double *) malloc( lenOut * sizeof(double));
        status = clEnqueueReadBuffer(clinfo.q, tmpDev, CL_TRUE, 0, lenOut*sizeof(double), outToSum, 0, NULL, NULL);
        CLERR
        int ii;
        out = 0.0;
        #pragma omp parallel for reduction (+:out)        
        for( ii=0; ii<lenOut; ii++) {            
            out += outToSum[ii];
        }
        free( outToSum );
    } else {
        float * outToSum = (float *) malloc( lenOut * sizeof(float));
        status = clEnqueueReadBuffer(clinfo.q, tmpDev, CL_TRUE, 0, lenOut*sizeof(float), outToSum, 0, NULL, NULL);
        CLERR
        int ii;
        out = 0.0;
        #pragma omp parallel for reduction (+:out)        
        for( ii=0; ii<lenOut; ii++) {            
            out += outToSum[ii];
        }
        free( outToSum );
    }
    clReleaseMemObject( tmpDev );
    clReleaseMemObject( outDev );
    return out;
}
void dotVectorComplex( double * out_re, double * out_im,  cl_mem v1Dev, cl_mem v2Dev, int len ) {
    cl_int status;
    cl_mem outDev_re, outDev_im;
    cl_kernel kernel = clkernels[KERVETDOTCOMPLEX];
    size_t localSize = 1024;
    size_t globalSize = roundUp( len, 1024 );
    //printf("global size : %d\n",globalSize); 
    outDev_re = clCreateBuffer(clinfo.c, CL_MEM_READ_WRITE, (globalSize/localSize)*sizeof(double), NULL, &status);
    CLERR
    outDev_im = clCreateBuffer(clinfo.c, CL_MEM_READ_WRITE, (globalSize/localSize)*sizeof(double), NULL, &status);
    CLERR
    status = clSetKernelArg (kernel, 0, sizeof(v1Dev), &v1Dev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(v2Dev), &v2Dev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(outDev_re), &outDev_re);
    CLERR
    status = clSetKernelArg (kernel, 3, sizeof(outDev_im), &outDev_im);
    CLERR
    status = clSetKernelArg (kernel, 4, sizeof(int), (void *) &len);
    CLERR
    status = clSetKernelArg (kernel, 5, localSize * sizeof(double), NULL );
    CLERR
    status = clSetKernelArg (kernel, 6, localSize * sizeof(double), NULL );
    CLERR
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalSize, &localSize, 0, NULL, NULL);
    CLERR
    int lenOut = globalSize / localSize;
    
     if( clinfo.fp64 ) {
        double * outToSum_re = (double *) malloc( lenOut * sizeof(double));
        double * outToSum_im = (double *) malloc( lenOut * sizeof(double));
        
        status = clEnqueueReadBuffer(clinfo.q, outDev_re, CL_TRUE, 0, lenOut*sizeof(double), outToSum_re, 0, NULL, NULL);
        CLERR
        status = clEnqueueReadBuffer(clinfo.q, outDev_im, CL_TRUE, 0, lenOut*sizeof(double), outToSum_im, 0, NULL, NULL);
        CLERR
        
        int ii;
        *out_re = 0.0;
        *out_im = 0.0;
        #pragma omp parallel for reduction (+:out)        
        for( ii=0; ii<lenOut; ii++) {            
            *out_re += outToSum_re[ii];
            *out_im += outToSum_im[ii];
        }
        free( outToSum_re );
        free( outToSum_im );
        
    } else {
        /*float * outToSum_re = (float *) malloc( lenOut * sizeof(float));
        float * outToSum_im = (float *) malloc( lenOut * sizeof(float));
        
        status = clEnqueueReadBuffer(clinfo.q, outDev_re, CL_TRUE, 0, lenOut*sizeof(float), outToSum_re, 0, NULL, NULL);
        CLERR
        status = clEnqueueReadBuffer(clinfo.q, outDev_im, CL_TRUE, 0, lenOut*sizeof(float), outToSum_im, 0, NULL, NULL);
        CLERR
        int ii;
        out = 0.0;
        #pragma omp parallel for reduction (+:out)        
        for( ii=0; ii<lenOut; ii++) {            
            out += outToSum[ii];
        }
        free( outToSum );*/
    }
    
    clReleaseMemObject( outDev_re );
    clReleaseMemObject( outDev_im );
}


cl_mem rmatVecMul3Complex(  rmatrix_t * M, cl_mem vDev, int ncols, int nrows ) {
    cl_int status;
    cl_mem outDev;
    cl_kernel kernel = M->kernel;
    size_t localWorkSize = 256;
    size_t globalWorkSize = 2 * clinfo.n * localWorkSize;   
    outDev = clCreateBuffer(clinfo.c, CL_MEM_WRITE_ONLY, 2*nrows*sizeof(double), NULL, &status);
    CLERR   
    status = clSetKernelArg (kernel, 0, sizeof(outDev), &outDev);
    CLERR
    status = clSetKernelArg (kernel, 1, sizeof(vDev), &vDev);
    CLERR
    status = clSetKernelArg (kernel, 2, sizeof(int), (void *)&ncols);
    CLERR
    status = clSetKernelArg (kernel, 3, sizeof(int), (void *)&nrows);
    CLERR
    status = clSetKernelArg (kernel, 4, localWorkSize * sizeof(double), NULL);
    CLERR
    status = clSetKernelArg (kernel, 5, localWorkSize * sizeof(double), NULL);
    CLERR    
    status = clEnqueueNDRangeKernel(clinfo.q, kernel, 1, NULL, &globalWorkSize, &localWorkSize, 0, NULL, NULL);
    CLERR
    status = clFinish(clinfo.q);
    CLERR
    return outDev;
}
    
#endif
