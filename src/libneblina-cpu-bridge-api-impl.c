#ifndef __NEBLINAVECTOROCL
#define __NEBLINAVECTOROCL
#define  BLOCK_DIM 16

#include <math.h>
#include "libneblina.h"
#include "neblina.h"
#include "bridge_api.h"

void InitEngine(int device){
    //printf("InitEngine\n");
    //InitCLEngine(device);
    //printf("end InitEngine\n");
}

void StopEngine(){
    //ReleaseCLInfo(clinfo);
}

long get_Engine_Max_Memory_Allocation(){
    return 0L;
}

void luDecomp(void* v1Dev, int n ) {
    
    return (void *)NULL;
}

double * addVectorF( double * v1, double * v2, int n ) {
    double * out = (double *) malloc( n * sizeof(double) );
    // #pragma omp parallel for
    for (int i=0; i<n; i++) {
        //printf("%f %f\n",v1[i] , v2[i]);
        out[i] = v1[i] + v2[i];
    }

    return out;
}


void* addVectorC(double* v1, double* v2, int n ) {
    
    double * out = (double *) malloc( 2 * n * sizeof(double) );
    // #pragma omp parallel for
    for (int i=0; i<n; i++) {
        out[2*i] = v1[2*i] + v2[2*i];
        out[2*i+1] = v1[2*i+1] + v2[2*i+1];
    }

    return out;
}

void* addVectorFC(double* v1, double* v2, int n ) {
    double * out = (double *) malloc( 2 * n * sizeof(double) );
    // #pragma omp parallel for
    for (int i=0; i<n; i++) {
        out[2*i] = v1[i] + v2[2*i];
        out[2*i+1] = v2[2*i+1];
    }

    return out;
}

void* prodVector(double* v1, double* v2, int n ) {
    double * out = (double *) malloc( n * sizeof(double) );
    // #pragma omp parallel for
    for (int i=0; i<n; i++) {
        out[i] = v1[i] * v2[i];
    }

    return out;
}

void* vecConj(void* v1Dev, int n ) {
    
    return (void *)NULL;
}

void* vecConjugate(double* v1, int n ) {
    double * out = (double *) malloc(2 * n * sizeof(double) );
    // #pragma omp parallel for
    for (int i=0; i<n; i++) {
        out[2*i] = v1[2*i];
        out[2*i+1] = -v1[2*i+1];
    }
    return out;
}

void* vecAddOff2(void* v1Dev, int n ) {
    
    return (void *)NULL;
}

void* vecAddOff(double* v1, int offset, int parts ) {
    size_t n = parts * offset;

    double * out = (double *) malloc( offset * sizeof(double) );
    // #pragma omp parallel for
    for (int i=0; i < offset; i++) {
        double s = 0;
        for(int l=0; l < parts; l++ ) {
            int idx = i + l * offset;
            s += v1[idx];   
        }
        out[i] = s;
    }
    return out;
}

void* prodComplexVector(double* v1, double* v2, int n ) {
    
    double * out = (double *) malloc(2 * n * sizeof(double) );
    // #pragma omp parallel for
    for (int i=0; i<n; i++) {
        int idx_re = 2*i;
        int idx_im = 2*i+1;
        out[idx_re]  = v1[idx_re] * v2[idx_re] - v1[idx_im] * v2[idx_im];
        out[idx_im]  = v1[idx_re] * v2[idx_im] + v1[idx_im] * v2[idx_re];
    }

    return out;
}

void* subVector(double* v1, double* v2, int n ) {
    double * out = (double *) malloc( n * sizeof(double) );
    // #pragma omp parallel for
    for (int i=0; i<n; i++) {
        //printf("%f %f\n",v1[i] , v2[i]);
        out[i] = v1[i] - v2[i];
    }

    return out;
}

void* subVectorC(double* v1, double* v2, int n ) {
    
    double * out = (double *) malloc( 2 * n * sizeof(double) );
    // #pragma omp parallel for
    for (int i=0; i<n; i++) {
        out[2*i] = v1[2*i] - v2[2*i];
        out[2*i+1] = v1[2*i+1] - v2[2*i+1];
    }

    return out;
}


void* mulScalarVector( double* v1, double scalar, int n ) {
    double * out = (double *) malloc( n * sizeof(double) );
    // #pragma omp parallel for
    for (int i=0; i<n; i++) {
        out[i] = scalar * v1[i];
    }

    return out;
}

void* mulComplexScalarVector( double* v1, double real, double imaginary, int n ) {
    double * out = (double *) malloc( 2 * n * sizeof(double) );
    // #pragma omp parallel for
    for (int i=0; i<n; i++) {
        out[2*i] = real * v1[i];
        out[2*i+1] = imaginary;
    }

    return out;
}

void* mulComplexScalarComplexVector( double* v1, double real, double imaginary, int n ) {
    double * out = (double *) malloc( 2 * n * sizeof(double) );
    // #pragma omp parallel for
    for (int i=0; i<n; i++) {
        out[2*i] = real * v1[2*i];
        out[2*i+1] = imaginary * v1[2*i+1];
    }

    return out;
}

void* mulFloatScalarComplexVector( double* v1, double real, int n ) {
    double * out = (double *) malloc( 2 * n * sizeof(double) );
    // #pragma omp parallel for
    for (int i=0; i<n; i++) {
        out[2*i] = real * v1[2*i];
        out[2*i+1] = v1[2*i+1];
    }

    return out;
}

void mulScalarMatRow( void* m, double scalar, int nrow, int ncols, int row) {
    //void* v1Dev, void* v2Dev, int n ) {
    
    return (void *)NULL;
}

void mulScalarMatCol( void* m, double scalar, int nrow, int ncols, int col) {
    //void* v1Dev, void* v2Dev, int n ) {
    
    return (void *)NULL;
}


void* matVecMul1(  void* mDev, void* vDev, int ncols, int nrows ) {
    //void* v1Dev, void* v2Dev, int n ) {
    
    return (void *)NULL;
}

void* matVecMul2(  void* mDev, void* vDev, int ncols, int nrows ) {
    //void* v1Dev, void* v2Dev, int n ) {
    
    return (void *)NULL;
}

int matrix_get_complex_real_index(int ncol, int i, int j){
    return 2 * (i * ncol + j);
}

int matrix_get_real_index(int ncol, int i, int j){
    return (i * ncol + j);
}

int matrix_get_complex_imag_index(int ncol, int i, int j){
    return matrix_get_complex_real_index(ncol, i, j) + 1;
}

void* matMulFloat(  double* m1, double* m2, int nrows, int ncols, int ncol_m1 ) {
    
    double * out = (double *) malloc( nrows * ncols * sizeof(double) );
    #pragma omp parallel for collapse(2)
    for (int i=0; i<nrows; i++) {
        for(int j=0; j < ncols; j++) {
            double sum = 0;
            double v1;
            double v2;
            #pragma omp unroll
            for(int k=0; k < ncol_m1; k++) {
                v1 = m1[i*ncol_m1+k]; //m1 row
                v2 = m2[k*ncols+j];   //m2 col
                sum += v1 * v2;
            }
            out[i*ncols+j] = sum;
        }    
    }

    return out;
}

void* matMulComplex(  double* m1, double* m2, int nrows, int ncols, int ncol_m1 ) {
    
    double * out = (double *) malloc( 2 * nrows * ncols * sizeof(double) );
    #pragma omp parallel for collapse(2)
    for (int i=0; i<nrows; i++) {
        for(int j=0; j < ncols; j++) {
            int k;
            double sumre = 0, sumim = 0;
            double re1, im1, re2, im2;
            #pragma omp unroll
            for(k=0; k < ncol_m1; k++) {
                int idx = matrix_get_complex_real_index(ncols,i,k);
                re1 = m1[idx];
                im1 = m1[idx+1];

                idx = matrix_get_complex_real_index(ncols,k,j);
                re2 = m2[idx];
                im2 = m2[idx+1];

                sumre += re1*re2 - im1*im2;
                sumim += re1*im2 + re2*im1;
            }
            int idx_out = matrix_get_complex_real_index(ncols,i,j);
            out[idx_out] = sumre; 
            out[idx_out+1] = sumim;
        }    
    }

    return out;
}

void* matMulFloatComplex(  double* m1, double* m2, int nrows, int ncols, int ncol_m1 ) {
    
    double * out = (double *) malloc( 2 * nrows * ncols * sizeof(double) );
    #pragma omp parallel for collapse(2)
    for (int i=0; i<nrows; i++) {
        for(int j=0; j < ncols; j++) {
            int k;
            double sumre = 0, sumim = 0;
            double re1, re2, im2;
            #pragma omp unroll
            for(k=0; k < ncol_m1; k++) {
                int idx = matrix_get_complex_real_index(ncol_m1,i,k);
                re1 = m1[idx];

                idx = matrix_get_complex_real_index(ncols,k,j);
                re2 = m2[idx];
                im2 = m2[idx+1];

                sumre += re1*re2;
                sumim += re1*im2;
            }
            int idx_out = matrix_get_complex_real_index(ncols,i,j);
            out[idx_out] = sumre;
            out[idx_out+1] = sumim;
        }    
    }
    return out;
}

void* matMul(  double* m1, double* m2, int nrows, int ncols, int ncol_m1, int atype, int btype ) {
    
    if( atype == T_FLOAT && btype == T_FLOAT) {
        return matMulFloat(m1, m2, nrows, ncols, ncol_m1);
        
    } else if( atype == T_COMPLEX && btype == T_COMPLEX ) {
        return matMulComplex(m1, m2, nrows, ncols, ncol_m1);
    
    } else if( atype == T_FLOAT && btype == T_COMPLEX ) {
        return matMulFloatComplex(m1, m2, nrows, ncols, ncol_m1);

    }
}

void* matMul2(  void* m1Dev, void* m2Dev, int nrows, int ncols, int qq ) {
    //void* v1Dev, void* v2Dev, int n ) {
    
    return (void *)NULL;
}

void matSquare( void* * outLin, void* * idxOutLin, 
                void* * outCol, void* * idxOutCol, 
                void* mLin, void* idxLin, 
                void* mCol, void* idxCol, 
                int maxcols, int N ) {
    //void* v1Dev, void* v2Dev, int n ) {
    
    return (void *)NULL;
}

void* matVecMul3(  double* mat, double* vec, int ncols, int nrows ) {
    
    double * out = (double *) malloc( nrows * sizeof(double) );
    
    #pragma omp parallel for
    for (int i=0; i<nrows; i++) {
        double sum = 0;
        #pragma omp unroll
        for(int j=0; j < ncols; j++) {
            double v1;
            double v2;
            int idx1 = matrix_get_real_index(ncols, i, j);
            v1 = mat[idx1];
            v2 = vec[j];
            sum += v1 * v2;
        }
            int idx_out = i; //matrix_get_real_index(ncols, i, j);
            out[idx_out] = sum;
    }
    return out;    
}

void* sparseVecMul(void* mDev, void* idxCol, void* vDev, int nrows, int maxCols ) {
//    const int idx = get_global_id(0);
//    if( idx >= nrows )
//        return;
//        
//    real sum = 0;
//    int row = idx;
//    int midx = idx * maxcols;
//    int i;
//    for (i = 0; i < maxcols; i++) {
//        int col = col_idx[midx + i];
//        sum += ( col != -1 ) ? m[midx + i] * vec_in[col] : 0;             
//    }
//    vec_out[row] = sum;
    
    double * vec_out = (double *) malloc( nrows * maxCols * sizeof(double) );
    double sum_re = 0, re_m, re_v;
    double * m = (double *) mDev;
    double * vec_in = (double *) vDev;
    int * col_idx = (int *) idxCol;

    #pragma omp parallel for
    for (int idx = 0; idx < nrows; idx++)
    {
           double sum = 0;
           int row = idx;
           int midx = idx * maxCols;
           int i;
           for (i = 0; i < maxCols; i++) {
               int col = col_idx[midx + i];
               sum += ( col != -1 ) ? m[midx + i] * vec_in[col] : 0;             
           }
           vec_out[row] = sum;
    }

    return (void *)vec_out;
}
void* sparseComplexVecMul(void* mDev, void* idxCol, void* vDev, int nrows, int maxCols ) {

    double * vec_out = (double *) malloc( 2 * nrows * maxCols * sizeof(double) );

    #pragma omp parallel for
    for (size_t idx = 0; idx < nrows; idx++)
    {
        double sum_re = 0,sum_im = 0, re_m, im_m, re_v, im_v;
        double * m = (double *) mDev;
        double * vec_in = (double *) vDev;
        int * col_idx = (int *) idxCol;
        int row = idx,i, col, idxt;
        for (i = 0; i < maxCols; i++) {
            idxt = (row * maxCols) + i;         
            col = col_idx[idxt];
            if( col == -1 )
                continue;
            re_m = m[2*idxt];
            im_m = m[2*idxt+1]; 
            re_v = vec_in[2*col];
            im_v = vec_in[2*col+1];    
            sum_re += re_m*re_v - im_m*im_v;
            sum_im += re_m*im_v + im_m*re_v; 
        }
        vec_out[2*row] = sum_re;
        vec_out[2*row+1] = sum_im;
    }
    return (void *)vec_out;
}

void* matVecMul3Complex(  double* mat, double* vec, int ncols, int nrows ) {
    
    // printf("matVecMul3Complex 1 ---------\n");
    double * out = (double *) malloc( 2 * nrows * sizeof(double) );
    // printf("matVecMul3Complex 2\n");
    
    #pragma omp parallel for
    for (int i=0; i<nrows; i++) {
        double sumre = 0, sumim = 0;
        for(int j=0; j < ncols; j++) {

            double re1, im1, re2, im2;
            int idx = matrix_get_complex_real_index(ncols,i,j);
            re1 = mat[idx];
            im1 = mat[idx+1];

            idx = 2*j;
            re2 = vec[idx];
            im2 = vec[idx+1];

            sumre += (re1*re2) - (im1*im2);
            sumim += (re1*im2) + (re2*im1);
        }
            int idx_out = 2 * i; //matrix_get_complex_real_index(ncols,i,j);
            out[idx_out] = sumre; 
            out[idx_out+1] = sumim;
    }    

    return out;
}

void* matTranspose(  void* mDev, int ncols, int nrows ) {
//    uint i = get_global_id(0);
//	uint j = get_global_id(1);
//
//	if((i < ncols) && (j < nrows))
//	{
//		uint idx = j * ncols + i;
//		block[get_local_id(1)*(BLOCK_DIM+1)+get_local_id(0)] = m[idx];
//	}
//
//	barrier(CLK_LOCAL_MEM_FENCE);
//	i = get_group_id(1) * BLOCK_DIM + get_local_id(0);
//	j = get_group_id(0) * BLOCK_DIM + get_local_id(1);
//	if((i < nrows) && (j < ncols))
//    {
//		unsigned int index_out = j * nrows + i;
//		out[index_out] = block[get_local_id(0)*(BLOCK_DIM+1)+get_local_id(1)];
//	}
    
    return (void *)NULL;
}

double sumVector( double* v1, int len ) {

    double out = 0;
    for (int i=0; i<len; i++) {
        out += v1[i];
    }

    return out;
    
}

double normVector( void* vDev, int len ) {
//    unsigned int tid = get_local_id(0);
//    unsigned int i = get_group_id(0)*(get_local_size(0)*2) + get_local_id(0);
//
//    sdata[tid] = (i < n) ? m[i]*m[i] : 0;
//    if ((i + get_local_size(0)) < n) {
//        sdata[tid] += m[i+get_local_size(0)]*m[i+get_local_size(0)];
//    }  
//
//    barrier(CLK_LOCAL_MEM_FENCE);
//
//    for(unsigned int s=get_local_size(0)/2; s>0; s>>=1) 
//    {
//        if (tid < s) 
//        {
//            sdata[tid] += sdata[tid + s];
//        }
//        barrier(CLK_LOCAL_MEM_FENCE);
//    }
//    if (tid == 0) out[get_group_id(0)] = sdata[0];
    
    return 0.0;
}


double dotVector(void* v1Dev, void* v2Dev, int len ) {
    
    double sum = 0;

    double * v1 = (double *) v1Dev;
    double * v2 = (double *) v2Dev;
    for (int i = 0; i < len; i++) {
        // printf("%d v1=%f v2=%f\n", i, v1[i], v2[i]);
        sum += v1[i] * v2[i];
    }    

    return sum;
}

void dotVectorComplex( double * out_re, double * out_im,  void* v1Dev, void* v2Dev, int len ) {
//    unsigned int tid = get_local_id(0);
//    unsigned int i = get_group_id(0)*(get_local_size(0)*2) + get_local_id(0);
//
//    sdata_re[tid] = (i < n) ? m1[2*i]*m2[2*i] -  m1[2*i+1]*m2[2*i+1]: 0;
//    sdata_im[tid] = (i < n) ? m1[2*i]*m2[2*i+1] + m1[2*i+1]*m2[2*i] : 0;
//    if ((i + get_local_size(0)) < n) {
//        sdata_re[tid] += m1[2*(i+get_local_size(0))]*m2[2*(i+get_local_size(0))] - m1[2*(i+get_local_size(0))+1]*m2[2*(i+get_local_size(0))+1];
//        sdata_im[tid] += m1[2*(i+get_local_size(0))+1]*m2[2*(i+get_local_size(0))] + m1[2*(i+get_local_size(0))]*m2[2*(i+get_local_size(0))+1];
//        
//    }
//    barrier(CLK_LOCAL_MEM_FENCE);
//    for(unsigned int s=get_local_size(0)/2; s>0; s>>=1)
//    {
//        if (tid < s)
//        {
//            sdata_im[tid] += sdata_im[tid + s];
//            sdata_re[tid] += sdata_re[tid + s];
//        }
//        barrier(CLK_LOCAL_MEM_FENCE);
//    }
//    if (tid == 0){ out_re[get_group_id(0)] = sdata_re[0];out_im[get_group_id(0)] = sdata_im[0]; }
    
    return (void *)NULL;
}

    
#endif
