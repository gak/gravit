

#ifndef __GNUC__
/* ******************************************************************* */
// no GCC
#include <xmmintrin.h>
#define __v128 __m128
#define _mm_init1_ps(f) {f, f, f, f}

#ifdef _MSC_VER
// microsoft specific

//#if (defined(_M_IX86_FP) && (_M_IX86_FP >= 1)) || defined(_M_X64) || defined(_M_AMD64)
//#define __SSE__
//#define __SSE2__
//#endif

#define ALWAYS_INLINE(ret_type) static __forceinline ret_type 

#else
#define ALWAYS_INLINE(ret_type) static inline ret_type 
#endif

#else
/* ******************************************************************* */
// use gcc extensions for SSE 
#include <x86intrin.h>
#include <mm_malloc.h>
#define ALIGNED __attribute__((aligned (16))) 
#define ALWAYS_INLINE(ret_type) static inline ret_type __attribute__((__gnu_inline__, __always_inline__, __artificial__)) 

typedef float __v128 __attribute__(( vector_size(4*sizeof(float)) ,aligned(16)  ));
#define _mm_init1_ps(f) {f, f, f, f}
#endif


/* ******************************************************************* */
// Number of Element an SSE operation processes
#define VECT_SIZE 4


/* ******************************************************************* */
// macros to make the source easier to read

#if 0 && defined(__GNUC__)
// let GCC do it - slower (unaligned mem. access ??)
#define LOAD_V4(arr, ind)         ( *((__v128 *) (arr + ind)) )
#define STORE_V4(arr, ind, value) { *((__v128 *) (arr + ind)) = value ;}
#define ADD_V4(arr, ind, value) { *((__v128 *) (arr + ind)) += (value) ;}
#define SUB_V4(arr, ind, value) { *((__v128 *) (arr + ind)) -= (value) ;}

#else
// use SSE macros
#define LOAD_V4(arr, ind)          _mm_load_ps((arr + ind))
#define STORE_V4(arr, ind, value) _mm_store_ps((arr + ind), (value))
#define ADD_V4(arr, ind, value)   _mm_store_ps((arr + ind), _mm_add_ps( _mm_load_ps((arr + ind)),  (value)))
#define SUB_V4(arr, ind, value)   _mm_store_ps((arr + ind), _mm_sub_ps( _mm_load_ps((arr + ind)),  (value)))
#endif


/* ******************************************************************* */

#if 0 && defined(__GNUC__)
// let GCC do it -- slower ..
#define V_ADD(a, b) ( (a) + (b))
#define V_SUB(a, b) ( (a) - (b))
#define V_MUL(a, b) ( (a) * (b))
#define V_DIV(a, b) ( (a) / (b))
#define V_INCR(a, b) {a += (b);} 
#define V_DECR(a, b) {a -= (b);} 

#else
// use SSE macros .. somehow faster ...
#define V_ADD(a, b) _mm_add_ps((a), (b))
#define V_SUB(a, b) _mm_sub_ps((a), (b))
#define V_MUL(a, b) _mm_mul_ps((a), (b))
#define V_DIV(a, b) _mm_div_ps((a), (b))
#define V_INCR(a, b) {a = _mm_add_ps( (a), (b));} 
#define V_DECR(a, b) {a = _mm_sub_ps( (a), (b));} 
#endif


/* ******************************************************************* */
// aux. constants
static const __v128 _half4 = _mm_init1_ps(.5f); 
static const __v128 _three4= _mm_init1_ps(3.f); 
static const __v128 _two4  = _mm_init1_ps(2.f);



/* ******************************************************************* */
/* SSE fast inverse square root                                        */
/*                            ret = 1/sqrt(v)  accuracy: approx. 22bit */
/* ******************************************************************* */

ALWAYS_INLINE(__v128) newtonrapson_rsqrt4( const __v128 v ) 
{
  //  x = rsqrt_approx(v);
  //  x' = 0.5 * x * (3 - v*x*x);

  __v128 approx = _mm_rsqrt_ps( v );
  // one newton-rapson step to improve accuracy
  return V_MUL( V_MUL(_half4, approx), 
		               V_SUB(_three4, 
				          V_MUL( V_MUL(v, approx), approx) ) );
}


/* ******************************************************************* */
/* SSE fast inverse                                                    */
/*                            ret = 1/v        accuracy: approx. 22bit */
/* ******************************************************************* */

ALWAYS_INLINE(__v128) newtonrapson_rcp(const __v128 v)
{
  //  x = reciprocal_approx(v);
  //  x' = x * (2 - x * v); --> x' = 2x - x( x*v) = (x+x) - x (x*v)

  __v128 r = _mm_rcp_ps(v);
  return (V_SUB( V_ADD(r, r), V_MUL( r, V_MUL(r, v))));
}



/* ******************************************************************* */
/* SSE: sum of all four elements of a vector                           */
/*                           ret = v[0] + v[1] + v[2] + v[3]           */
/* ******************************************************************* */

ALWAYS_INLINE(float) _vector4_sum(const __v128 v)
{ 
  __v128 a;

  a = _mm_shuffle_ps(v,v, _MM_SHUFFLE(0,3,0,1));                             // 1->0, 3->2; 
  a = V_ADD(a, v);                                                           // 0+1, 1+0, 2+3, 3+2
  return(_mm_cvtss_f32(V_ADD(a, _mm_shuffle_ps(a,a, _MM_SHUFFLE(0,0,0,2)))));// 2->0
                                                                             // 0 = 1 + 0 + 3 + 2
}


