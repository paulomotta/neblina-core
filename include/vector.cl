#if defined(cl_amd_fp64)

#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#define DOUBLE_SUPPORT_AVAILABLE

#elif defined(cl_khr_fp64)  

#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#define DOUBLE_SUPPORT_AVAILABLE

#endif


#if defined(DOUBLE_SUPPORT_AVAILABLE)
typedef double real;
#else
typedef float real;
#endif


//#pragma OPENCL EXTENSION cl_intel_printf : enable
__kernel void addVector( __global real * out, __global const real * v1, __global const real * v2 ) {
    int i = get_global_id(0);
    out[i] = v1[i] + v2[i];
}

__kernel void addVectorComplex( __global real * out, __global const real * v1, __global const real * v2 ) {
    int i = get_global_id(0);
    out[2*i] = v1[2*i] + v2[2*i];
    out[2*i+1] = v1[2*i+1] + v2[2*i+1];
}

__kernel void addVectorFloatComplex( __global real * out, __global const real * v1, __global const real * v2 ) {
    int i = get_global_id(0);
    out[2*i] = v1[i] + v2[2*i];
    out[2*i+1] = v2[2*i+1];
}

__kernel void subVector( __global real * out, __global const real * v1, __global const real * v2 ) {
    int i = get_global_id(0); 
    out[i] = v1[i] - v2[i];
}

__kernel void subVectorComplex( __global real * out, __global const real * v1, __global const real * v2 ) {
    int i = get_global_id(0);
    out[2*i] = v1[2*i] - v2[2*i];
    out[2*i+1] = v1[2*i+1] - v2[2*i+1];
}

__kernel void vecConjugate( __global real * out, __global const real * v ) {
    int i = get_global_id(0); 
    out[2*i] = v[2*i];
    out[2*i+1] = -v[2*i+1];
}
__kernel void mulScalarVector( __global real * out, __global real * v, real value ) {
    int i = get_global_id(0); 
    out[i] = value * v[i];
}

__kernel void mulScalarMatRow( __global real * inout, real value, int ncol, int line ) {
    int i = get_global_id(0); 
    inout[line*ncol + i] = value * inout[line*ncol + i];
}

__kernel void mulScalarMatCol( __global real * inout, real value, int ncol, int col  ) {
    int i = get_global_id(0); 
    inout[ncol*i + col] = value * inout[ncol*i + col];
}

__kernel void mulComplexScalarFloatVet( __global real * out, __global real * v, real re, real im ) {
    int i = get_global_id(0); 
    out[2*i] = re * v[i];
    out[2*i+1] = im; //there was no original imaginary part
}

__kernel void mulComplexScalarComplexVet( __global real * out, __global real * v, real re, real im ) {
    int i = get_global_id(0); 
    out[2*i] = re * v[2*i];
    out[2*i+1] = im * v[2*i+1];
}
 
__kernel void mulFloatScalarComplexVet( __global real * out, __global real * v, real re) {
    int i = get_global_id(0); 
    out[2*i] = re * v[2*i];
    out[2*i+1] = v[2*i+1];
}
 
#define  BLOCK_DIM 16
__kernel void matTranspose(__global real *out, __global real * m, int ncols, int nrows, __local real * block) {    
	uint i = get_global_id(0);
	uint j = get_global_id(1);

	if((i < ncols) && (j < nrows))
	{
		uint idx = j * ncols + i;
		block[get_local_id(1)*(BLOCK_DIM+1)+get_local_id(0)] = m[idx];
	}

	barrier(CLK_LOCAL_MEM_FENCE);
	i = get_group_id(1) * BLOCK_DIM + get_local_id(0);
	j = get_group_id(0) * BLOCK_DIM + get_local_id(1);
	if((i < nrows) && (j < ncols))
    {
		unsigned int index_out = j * nrows + i;
		out[index_out] = block[get_local_id(0)*(BLOCK_DIM+1)+get_local_id(1)];
	}
}
__kernel void matVecMul3(__global real * out, const __global real * m, const __global real * v, int ncols, int nrows, __local real * pDot ) {
    int i = get_group_id(0);
    int n = get_num_groups(0);
    uint k, t;
    for (; i < nrows; i +=n ) {
        const __global real * row = m + i * ncols;
        real sum = 0;
        for ( k = get_local_id(0); k < ncols; k += get_local_size(0))
            sum += row[k] * v[k];
        pDot[get_local_id(0)] = sum;
        barrier(CLK_LOCAL_MEM_FENCE);
        for (t=1;t<get_local_size(0); t*=2) {
            barrier(CLK_LOCAL_MEM_FENCE);
            uint idx = 2 * t * get_local_id(0);
            if (idx < get_local_size(0))
                pDot[idx] += pDot[idx+t];
        }
        if (get_local_id(0) == 0)
            out[i] = pDot[0];
        barrier(CLK_LOCAL_MEM_FENCE);
	} 
}

__kernel void matVecMul3Complex(__global real * out, const __global real * m, const __global real * v, int ncols, int nrows, __local real * pDotR, __local real * pDotC ) {
    int i = get_group_id(0);
    int n = get_num_groups(0);
    uint k, t;
    for (; i < nrows; i +=n ) {
        const __global real * row = m + i * ncols * 2;
        real sumR = 0;
        real sumC = 0;
        for ( k = get_local_id(0); k < ncols; k += get_local_size(0)) {
            sumR += row[2*k] * v[2*k] - row[2*k+1] * v[2*k+1];
            sumC += row[2*k] * v[2*k+1] + row[2*k+1] * v[2*k];
        }
        pDotR[get_local_id(0)] = sumR;
        pDotC[get_local_id(0)] = sumC;
        barrier(CLK_LOCAL_MEM_FENCE);
        for (t=1;t<get_local_size(0); t*=2) {
            barrier(CLK_LOCAL_MEM_FENCE);
            uint idx = 2 * t * get_local_id(0);
            if (idx < get_local_size(0)) {
                pDotR[idx] += pDotR[idx+t];
                pDotC[idx] += pDotC[idx+t];
            }    
        }
        if (get_local_id(0) == 0) {
            out[2*i] = pDotR[0];
            out[2*i+1] = pDotC[0];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
	} 
}
__kernel void sumVector(__global real *m, __global real *out, uint n,  __global real * result, __local real * sdata)
{
    unsigned int tid = get_local_id(0);
    unsigned int i = get_group_id(0)*(get_local_size(0)*2) + get_local_id(0);

    sdata[tid] = (i < n) ? m[i] : 0;
    if (i + get_local_size(0) < n) 
        sdata[tid] += m[i+get_local_size(0)];  

    barrier(CLK_LOCAL_MEM_FENCE);

    for(unsigned int s=get_local_size(0)/2; s>0; s>>=1) 
    {
        if (tid < s) 
        {
            sdata[tid] += sdata[tid + s];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    if (tid == 0) out[get_group_id(0)] = sdata[0];
    // Passar essa parte para o 'host'
    /*
    barrier(CLK_GLOBAL_MEM_FENCE);
    if( get_global_id(0) == 0 ) {
        int k=0, n = get_global_size(0)/get_local_size(0);      
        *result = 0.0;  
        for(k=0;k<n;k++)
            *result +=  out[k];       
    }
    */
}
__kernel void normVector(__global real *m, __global real *out, uint n,  __global real * result, __local real * sdata)
{
    unsigned int tid = get_local_id(0);
    unsigned int i = get_group_id(0)*(get_local_size(0)*2) + get_local_id(0);

    sdata[tid] = (i < n) ? m[i]*m[i] : 0;
    if ((i + get_local_size(0)) < n) {
        sdata[tid] += m[i+get_local_size(0)]*m[i+get_local_size(0)];
    }  

    barrier(CLK_LOCAL_MEM_FENCE);

    for(unsigned int s=get_local_size(0)/2; s>0; s>>=1) 
    {
        if (tid < s) 
        {
            sdata[tid] += sdata[tid + s];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    if (tid == 0) out[get_group_id(0)] = sdata[0];
    // Passar essa parte para o 'host'
    /*
    barrier(CLK_GLOBAL_MEM_FENCE);
    if( get_global_id(0) == 0 ) {
        int k=0, n = get_global_size(0)/get_local_size(0);      
        *result = 0.0;  
        for(k=0;k<n;k++)
            *result +=  out[k];       
    }
    */
}
__kernel void dotVector(__global real *m1, __global real *m2, __global real *out, uint n,  __global real * result, __local real * sdata)
{
    unsigned int tid = get_local_id(0);
    unsigned int i = get_group_id(0)*(get_local_size(0)*2) + get_local_id(0);

    sdata[tid] = (i < n) ? m1[i]*m2[i] : 0;
    if ((i + get_local_size(0)) < n) {
        sdata[tid] += m1[i+get_local_size(0)]*m2[i+get_local_size(0)];
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    for(unsigned int s=get_local_size(0)/2; s>0; s>>=1)
    {
        if (tid < s)
        {
            sdata[tid] += sdata[tid + s];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    if (tid == 0) out[get_group_id(0)] = sdata[0];
}
__kernel void dotVectorComplex(__global real *m1, 
                               __global real *m2, 
                               __global real *out_re, 
                               __global real *out_im, 
                               uint n,  
                               __local real * sdata_re,
                               __local real * sdata_im )
{
    unsigned int tid = get_local_id(0);
    unsigned int i = get_group_id(0)*(get_local_size(0)*2) + get_local_id(0);

    sdata_re[tid] = (i < n) ? m1[2*i]*m2[2*i] -  m1[2*i+1]*m2[2*i+1]: 0;
    sdata_im[tid] = (i < n) ? m1[2*i]*m2[2*i+1] + m1[2*i+1]*m2[2*i] : 0;
    if ((i + get_local_size(0)) < n) {
        sdata_re[tid] += m1[2*(i+get_local_size(0))]*m2[2*(i+get_local_size(0))] - m1[2*(i+get_local_size(0))+1]*m2[2*(i+get_local_size(0))+1];
        sdata_im[tid] += m1[2*(i+get_local_size(0))+1]*m2[2*(i+get_local_size(0))] + m1[2*(i+get_local_size(0))]*m2[2*(i+get_local_size(0))+1];
        
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    for(unsigned int s=get_local_size(0)/2; s>0; s>>=1)
    {
        if (tid < s)
        {
            sdata_im[tid] += sdata_im[tid + s];
            sdata_re[tid] += sdata_re[tid + s];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    if (tid == 0){ out_re[get_group_id(0)] = sdata_re[0];out_im[get_group_id(0)] = sdata_im[0]; }
}


__kernel void matMul1(__global real * out, __global const real * m1, __global const real * m2, int ncol_m1) {
    int i = get_global_id(0);
    int j = get_global_id(1);
    int nrow = get_global_size(0);
    int ncol = get_global_size(1);
    out[i*ncol+j] = 0;
    int k;
    real sum = 0;
    real v1;
    real v2;
    for(k=0; k < ncol_m1; k++) {
        v1 = m1[i*ncol_m1+k];
        v2 = m2[k*ncol+j];
        sum += v1* v2;
    }
    out[i*ncol+j] = sum;    
}

int matrix_get_complex_real_index(int ncol, int i, int j){
    return 2 * (i * ncol + j);
}

__kernel void matMulComplex(__global real * out, __global const real * m1, __global const real * m2, int ncol_m1) {
    int i = get_global_id(0);
    int j = get_global_id(1);
    int nrow = get_global_size(0);
    int ncol = get_global_size(1);

    int k;
    real sumre = 0, sumim = 0;
    real re1, im1, re2, im2;
    for(k=0; k < ncol_m1; k++) {
        int idx = matrix_get_complex_real_index(ncol,i,k);
        re1 = m1[idx];
        im1 = m1[idx+1];
        
        idx = matrix_get_complex_real_index(ncol,k,j);
        re2 = m2[idx];
        im2 = m2[idx+1];

        sumre += re1*re2 - im1*im2;
        sumim += re1*im2 + re2*im1;
    }
    out[2*(i*ncol+j)] = sumre; 
    out[2*(i*ncol+j)+1] = sumim; 
}

__kernel void matMulFloatComplex(__global real * out, __global const real * m1, __global const real * m2, int ncol_m1) {
    int i = get_global_id(0);
    int j = get_global_id(1);
    int nrow = get_global_size(0);
    int ncol = get_global_size(1);
    
    int k;
    real sumre = 0, sumim = 0;
    real re1, re2, im2;
    for(k=0; k < ncol_m1; k++) {
        re1 = m1[i*ncol_m1+k];
        
        re2 = m2[2*(k*ncol+j)];
        im2 = m2[2*(k*ncol+j)+1];
        sumre += re1*re2;
        sumim += re1*im2;
    }
    out[2*(i*ncol+j)] = sumre;
    out[2*(i*ncol+j)+1] = sumim;   
}
#define BLOCK_SIZE 16

__kernel void
matMul2(              __global       real * Result,
                      __global       real * M1,
                      __global       real * M2,
                            int  q)
{
   int i = get_group_id(0);
   int j = get_group_id(1);
   int idX = get_local_id(0);
   int idY = get_local_id(1);
   int p = get_global_size(0);
   int r = get_global_size(1);
   int qq = q;
   int numSubMat = qq/BLOCK_SIZE;

   real resp = 0;
   __local real A[BLOCK_SIZE][BLOCK_SIZE];
   __local real B[BLOCK_SIZE][BLOCK_SIZE];

   for (int k=0; k<numSubMat; k++)
   {
       int idx1 = BLOCK_SIZE*i + idX + p*(BLOCK_SIZE*k+idY);
       int idx2 = BLOCK_SIZE*k + idX + qq*(BLOCK_SIZE*j+idY);
       A[idX][idY] = (idx1 < q*q) ? M1[idx1] : 0;
       B[idX][idY] = (idx2 < q*q) ? M2[idx2] : 0;
       barrier(CLK_LOCAL_MEM_FENCE);

       for (int k2 = 0; k2 < BLOCK_SIZE; k2++)
       {
            real temp1 = A[idX][k2];
            real temp2 = B[k2][idY];
            resp += temp1 * temp2;
       }
       barrier(CLK_LOCAL_MEM_FENCE);
   }
   if( (BLOCK_SIZE*i + idX + p*(BLOCK_SIZE*j+idY)) < q*q )
       Result[BLOCK_SIZE*i + idX + p*(BLOCK_SIZE*j+idY)] = resp;
}
__kernel void sparseMatMulVec(  __global real * vec_out, 
                                __global real * m,  
                                __global int * col_idx,
                                __constant real * vec_in, 
                                const int nrows, 
                                const int maxcols ) {
    const int idx = get_global_id(0);
    if( idx >= nrows )
        return;
        
    real sum = 0;
    int row = idx;
    int midx = idx * maxcols;
    int i;
    for (i = 0; i < maxcols; i++) {
        int col = col_idx[midx + i];
        sum += ( col != -1 ) ? m[midx + i] * vec_in[col] : 0;             
    }
    vec_out[row] = sum;
}/*
#define SMM 256
__kernel void sparseMatMulVec(  __global real * vec_out, 
                                __global real * m,  
                                __global int * col_idx,
                                __constant real * vec_in, 
                                const int nrows, 
                                const int maxcols ) {
    const int init = get_global_id(0)*SMM;
    const int final = ((init + SMM - 1) < nrows) ? (init + SMM - 1) : (nrows - 1);    
        
    real sum = 0;
    real value;
    int row,i;

    for(row=init; row <= final; row++ ) {
        sum = 0;        
        for (i = 0; i < maxcols; i++) {
            int col = col_idx[(row * maxcols) + i];
            if( col == -1 )
                break;
            value = m[(row * maxcols) + i];
            sum += value * vec_in[col]; 
        }
        vec_out[row] = sum;
    }
    
}*/

__kernel void sparseComplexMatMulVec(   __global real * vec_out, 
                                        __global real * m,  
                                        __global int * col_idx,
                                        __global real * vec_in, 
                                        const int nrows, 
                                        const int maxcols ) {
    const int idx = get_global_id(0);
    if( idx >= nrows )
        return;
        
    real sum_re = 0,sum_im = 0, re_m, im_m, re_v, im_v;
    int row = idx,i, col, idxt;
    for (i = 0; i < maxcols; i++) {
        idxt = (row * maxcols) + i;         
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

__kernel void luDecomp(__global real * A, int n ) {
    int j = get_global_id(0) + 1, i, k;
    for( i = 0; i < n; i++ ) {
        if( j > i )           
            A[j*n+i] /= A[i*n+i]; 
        barrier(CLK_LOCAL_MEM_FENCE);    
        if( j > i )        
        for(k=i+1; k < n; k++ )
            A[j*n+k] -= A[i*n+k]*A[j*n+i];
    }
}

__kernel void prodVector( __global real * out, __global const real * v1, __global const real * v2 ) {
    int i = get_global_id(0);
    out[i] = v1[i] * v2[i];
}

__kernel void prodComplexVector( __global real * out, __global const real * v1, __global const real * v2 ) {
    int i = get_global_id(0);
    int idx_re = 2*i;
    int idx_im = 2*i+1;
    out[idx_re]  = v1[idx_re] * v2[idx_re] - v1[idx_im] * v2[idx_im];
    out[idx_im]  = v1[idx_re] * v2[idx_im] + v1[idx_im] * v2[idx_re];
}

__kernel void vecConj( __global real * out, __global const real * v ) {
    int i = get_global_id(0);
    int idx_re = 2*i, idx_im = 2*i+1;
    out[i]  = v[idx_re] * v[idx_re] + v[idx_im] * v[idx_im];   
}

__kernel void vec_add_off2( __global real * out, __global const real * v, int off) {
    int i = get_global_id(0);
    out[i]  = v[i] + v[i+off];   
}

__kernel void vec_add_off( __global real * out, __global const real * v, int offset, int parts) {
    int j = get_global_id(0), l;
    real s = 0;
    for(l=0; l < parts; l++ )
        s += v[j+l*offset];   
    out[j] = s;
}
/*
void GetSemaphor(__global int * semaphor) {
   int occupied = atom_xchg(semaphor, 1);
   while(occupied > 0)
   {
     occupied = atom_xchg(semaphor, 1);
   }
}

void ReleaseSemaphor(__global int * semaphor)
{
   int prevVal = atom_xchg(semaphor, 0);
}
*/


__kernel void sparseMatMatMul(  __global real * qOutLin,
                                __global int  * qOutLinIdx,   
                                const int       qOutLinMax,
                                
                                __global real * qOutCol, 
                                __global int  * qOutColIdx,
                                const int       qOutColMax, 
                                
                                __global real * qInLin,
                                __global int  * qInLinIdx,
                                const int       qInLinMax,
                                
                                __global real * qInCol,
                                __global  int * qInColIdx,
                                const int       qInColMax,
                                
                                __global  int * qOutCount,
                                
                                __global  int * sem,
                                
                                
                                const int N ) {
                                
    int i = get_global_id(0), j, idxLin, idxCol, iLin, iCol, count = 0;
    //printf("%d BEGIN\n", i );
    /*
    for(j=0; j < N; j++ )
    for(s=0; s < qInLinMax; s++ ) {
        printf("i=%d col = %d [%f %f]\n", j, qInLinIdx[j*qInLinMax+s], qInLin[2*(j*qInLinMax+s)], qInLin[2*(j*qInLinMax+s)+1] ); 
        //printf("i=%d col = %d [%f %f]\n", s, j, qInCol[2*(j*qInLinMax+s)], qInCol[2*(j*qInLinMax+s)+1] ); 
    
    }*/
    real re1, re2, im1, im2, sum_re, sum_im;
        
    for(j=0; j < N; j++ ) {
        
        iLin = 0;
        iCol = 0;
        sum_re =  0;
        sum_im = 0;
        //count = 0;
    
        //printf("Processing  (%d, %d)\n", i, j );
        while( iLin < qInLinMax && iCol < qInColMax && 
               qInLinIdx[i*qInLinMax + iLin] != -1 && 
               qInColIdx[j*qInColMax + iCol] != -1) {
                
            idxLin = qInLinIdx[i*qInLinMax + iLin];
            idxCol = qInColIdx[j*qInColMax + iCol];
            //printf("[%d, %d] COL = %d LIN =%d\n", iLin, iCol, idxLin, idxCol );   
            if( idxLin > idxCol )
                iCol++;
            else if( idxLin < idxCol )
                iLin++;
            else {
                re1 = qInLin[2*(i*qInLinMax + iLin)];
                im1 = qInLin[2*(i*qInLinMax + iLin)+1];
                
                
                re2 = qInCol[2*(j*qInColMax + iCol)];
                im2 = qInCol[2*(j*qInColMax + iCol)+1]; 
                
                //printf("(%d,%d) * (%d, %d) ==== re1 = %f  im1 = %f re2 = %f im2 = %f\n", i, idxLin, idxCol, j, re1, im1, re2, im2 );
                
                sum_re += re1*re2 - im1*im2;
                sum_im += re1*im2 + im1*re2; 
                iLin++;
                iCol++;
            }
        }
        
        if( sum_re != 0 || sum_im != 0 ) {
            //printf("(%d, %d) -> %f %f [%d]\n", i, j, sum_re, sum_im, count  ); 
            
            qOutLin[2*(i*qOutLinMax + count)] = sum_re;
            qOutLin[2*(i*qOutLinMax + count)+1] = sum_im;
            qOutLinIdx[i*qOutLinMax + count] = j;
            
            count++;
            
            //GetSemaphor(&sem[j]);
                qOutCol[2*(j*qOutColMax + qOutCount[j])] = sum_re;
                qOutCol[2*(j*qOutColMax + qOutCount[j])+1] = sum_im;
                qOutColIdx[j*qOutColMax + qOutCount[j]] = i;
                qOutCount[j]++;                            // Danger Zone
            //ReleaseSemaphor(&sem[j]);
           
           
        }
                
    }
    qOutLinIdx[i*qOutLinMax + count] = -1;                               
    //printf("%d END\n", i );
}
