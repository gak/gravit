

#ifdef _MSC_VER
// microsoft SSE -- not tested yet
#include <xmmintrin.h>
#define __v128 __m128
#else
// gcc SSE 
#include <x86intrin.h>
#define ALIGNED __attribute__((aligned (16))) 

typedef float __v128 __attribute__(( vector_size(4*sizeof(float)) ,aligned(16)  ));

#define _mm_init1_ps(f) {f, f, f, f}

#endif



// aux. constants
static const __v128 _half4 = { .5f, .5f, .5f, .5f}; 
static const __v128 _three4= { 3.f, 3.f, 3.f, 3.f}; 
static const __v128 _two4  = { 2.f, 2.f, 2.f, 2.f}; 


// SSE fast inverse square root
// accuracy: approx. 22bit 
static inline __v128 __attribute__((__gnu_inline__, __always_inline__, __artificial__, hot)) 
newtonrapson_rsqrt4( const __v128 v ) 
{
  //  x = rsqrt_approx(v);
  //  x' = 0.5 * x * (3 - v*x*x);

  __v128 approx = _mm_rsqrt_ps( v );
  // one newton-rapson step to improve accuracy
  return _mm_mul_ps(_mm_mul_ps(_half4, approx), 
		               _mm_sub_ps(_three4, 
				          _mm_mul_ps(_mm_mul_ps(v, approx), approx) ) );
}


// SSE fast inverse

static inline __v128 __attribute__((__gnu_inline__, __always_inline__, __artificial__, hot)) 
newtonrapson_rcp(const __v128 a)
{
  //  x = reciprocal_approx(c);
  //  x' = x * (2 - x * a); --> x' = 2x - x( x*a) = (x+x) - x (x*a)

  __v128 r = _mm_rcp_ps(a);
  return _mm_sub_ps(_mm_add_ps(r, r), _mm_mul_ps(_mm_mul_ps(r, a), r));
}



// SSE: sum of all four elements of a vector

static inline float  __attribute__((__gnu_inline__, __always_inline__, __artificial__, hot))
_vector4_sum(const __v128 b)
{ 
  __v128 a;

  a = _mm_shuffle_ps(b,b, 0b00110001);                                       // 1->0, 3->2; 
  a = _mm_add_ps(a, b);                                                      // 0+1, 1+0, 2+3, 3+2
  return(_mm_cvtss_f32(_mm_add_ps(a, _mm_shuffle_ps(a,a, 0b00000010))));     // 2->0
                                                                             // 0 = 1 + 0 + 3 + 2
}


