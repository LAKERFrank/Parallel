#include "PPintrin.h"

// implementation of absSerial(), but it is vectorized using PP intrinsics
void absVector(float *values, float *output, int N)
{
  __pp_vec_float x;
  __pp_vec_float result;
  __pp_vec_float zero = _pp_vset_float(0.f);
  __pp_mask maskAll, maskIsNegative, maskIsNotNegative;

  //  Note: Take a careful look at this loop indexing.  This example
  //  code is not guaranteed to work when (N % VECTOR_WIDTH) != 0.
  //  Why is that the case?
  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {

    // All ones
    maskAll = _pp_init_ones();

    // All zeros
    maskIsNegative = _pp_init_ones(0);

    // Load vector of values from contiguous memory addresses
    _pp_vload_float(x, values + i, maskAll); // x = values[i];

    // Set mask according to predicate
    _pp_vlt_float(maskIsNegative, x, zero, maskAll); // if (x < 0) {

    // Execute instruction using mask ("if" clause)
    _pp_vsub_float(result, zero, x, maskIsNegative); //   output[i] = -x;

    // Inverse maskIsNegative to generate "else" mask
    maskIsNotNegative = _pp_mask_not(maskIsNegative); // } else {

    // Execute instruction ("else" clause)
    _pp_vload_float(result, values + i, maskIsNotNegative); //   output[i] = x; }

    // Write results back to memory
    _pp_vstore_float(output + i, result, maskAll);
  }
}

void clampedExpVector(float *values, int *exponents, float *output, int N)
{
  //
  // PP STUDENTS TODO: Implement your vectorized version of
  // clampedExpSerial() here.
  //
  // Your solution should work for any value of
  // N and VECTOR_WIDTH, not just when VECTOR_WIDTH divides N
  //

  __pp_vec_float x;
  __pp_vec_float result= _pp_vset_float(0.f);
  __pp_vec_float float_vec_nine = _pp_vset_float(9.999999f);
  __pp_vec_int y;
  __pp_vec_int count;
  __pp_vec_int int_vec_zero = _pp_vset_int(0);
  __pp_vec_int int_vec_one = _pp_vset_int(1);
  __pp_mask allMask;
  __pp_mask zeroMask;
  __pp_mask oneMask;
  __pp_mask countCompareMask;
  __pp_mask resultCompareMask;
  float nine = 9.999999f;

  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {
    zeroMask=_pp_init_ones(0);
    countCompareMask=_pp_init_ones(0);
    resultCompareMask=_pp_init_ones(0);

    if(i + VECTOR_WIDTH > N){
      allMask = _pp_init_ones(N-i);
    }
    else{
      allMask = _pp_init_ones();
    }
    _pp_vload_float(x, values + i, allMask);
    _pp_vload_int(y, exponents + i, allMask);

    _pp_veq_int(zeroMask,y,int_vec_zero,allMask);
    _pp_vset_float(result,1.f,zeroMask);

    oneMask = _pp_mask_not(zeroMask);

    _pp_vmove_float(result,x,oneMask);

    _pp_vsub_int(count, y ,int_vec_one, oneMask);

    _pp_vgt_int(countCompareMask,count,int_vec_zero,oneMask);

    while(_pp_cntbits(countCompareMask) > 0){
      _pp_vmult_float(result,result, x, countCompareMask);
      _pp_vsub_int(count, count, int_vec_one, countCompareMask);
      _pp_vgt_int(countCompareMask,count,int_vec_zero,countCompareMask);
    }

    _pp_vgt_float(resultCompareMask,result,float_vec_nine,allMask);
    _pp_vset_float(result,nine,resultCompareMask);
    _pp_vstore_float(output+i,result,allMask);

  }
}

// returns the sum of all elements in values
// You can assume N is a multiple of VECTOR_WIDTH
// You can assume VECTOR_WIDTH is a power of 2
float arraySumVector(float *values, int N)
{

  //
  // PP STUDENTS TODO: Implement your vectorized version of arraySumSerial here
  //
  __pp_vec_float x;
  float sum=0.f; 
  float temp[VECTOR_WIDTH];
  __pp_mask allMask;
  
  for (int i = 0; i < N; i += VECTOR_WIDTH)
  {
    int len = VECTOR_WIDTH;

    if(i + VECTOR_WIDTH > N){
      allMask = _pp_init_ones(N-i);
    }
    else{
      allMask = _pp_init_ones();
    }
    _pp_vload_float(x,values + i,allMask);//x=values[i] 1234
    while(len!=1){
      _pp_hadd_float(x,x);
      _pp_interleave_float(x,x);
      len>>=1;
    }
    _pp_vstore_float(temp,x,allMask);
    sum += temp[0];
  }

  return sum;
}