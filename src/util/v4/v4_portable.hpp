/* 
 * Written by:
 *   Kevin J. Bowers, Ph.D.
 *   Plasma Physics Group (X-1)
 *   Applied Physics Division
 *   Los Alamos National Lab
 * March/April 2004 - Imported from earlier V4PIC versions
 *
 */

// v4_portable is not intended to be efficient ... it is intended to
// give a reference implementation of the behavior of v4 to facilitate
// writing hardware accelerated versions.

#ifndef _v4_hpp_
#define _v4_hpp_

#include <ostream>
#include <cmath>

namespace v4 {
  
  class v4;
  class v4int;
  class v4float;

  /////////////////////////////////////////////////////////////////////////////

  ///////////////////
  // v4 base class //
  ///////////////////

  class v4 {

    /////////////////////////////
    // v4 manipulation friends //
    /////////////////////////////

    friend inline void transpose( v4 &a0, v4 &a1, v4 &a2, v4 &a3 );
    friend inline void cmov( const v4 &c, const v4 &a, v4 &b );
    friend inline void czero( const v4 &c, v4 &a );
    friend inline void notcmov( const v4 &c, const v4 &a, v4 &b );
    friend inline void notczero( const v4 &c, v4 &a );

    /////////////////////////////////
    // Memory manipulation friends //
    /////////////////////////////////
    
    friend inline void load( const void *p, v4 &a );
    friend inline void half_swizzle( const void *a0, const void *a1,
                                     const void *a2, const void *a3,
                                     v4 &a, v4 &b );
    friend inline void swizzle( const void *a0, const void *a1,
                                const void *a2, const void *a3,
                                v4 &a, v4 &b, v4 &c, v4 &d );

    friend inline void store( const v4 &a, void *p );
    friend inline void stream( const v4 &a, void *p );
    friend inline void half_deswizzle( const v4 &a, const v4 &b,
                                       void *a0, void *a1,
                                       void *a2, void *a3 );
    friend inline void deswizzle( const v4 &a, const v4 &b,
                                  const v4 &c, const v4 &d,
                                  void *a0, void *a1, void *a2, void *a3 );

  protected:
    union {
      int i[4];
      float f[4];
    };
    
  public:
    v4() {};
    v4(const v4 &a) {
      i[0]=a.i[0];
      i[1]=a.i[1];
      i[2]=a.i[2];
      i[3]=a.i[3];
    };
    ~v4() {};
  };
  
  ///////////////////////////////
  // v4 manipulation functions //
  ///////////////////////////////
  
# define sw(x,y) x^=y, y^=x, x^=y
  inline void transpose( v4 &a0, v4 &a1, v4 &a2, v4 &a3 ) {
    /**/ sw( a0.i[1],a1.i[0] ); sw( a0.i[2],a2.i[0] ); sw( a0.i[3],a3.i[0] );
    /**/                        sw( a1.i[2],a2.i[1] ); sw( a1.i[3],a3.i[1] );
    /**/                                               sw( a2.i[3],a3.i[2] );
  }
# undef sw

  inline void cmov( const v4 &c, const v4 &a, v4 &b ) {
    b.i[0] = (b.i[0] & ~c.i[0]) | (a.i[0] & c.i[0] );
    b.i[1] = (b.i[1] & ~c.i[1]) | (a.i[1] & c.i[1] );
    b.i[2] = (b.i[2] & ~c.i[2]) | (a.i[2] & c.i[2] );
    b.i[3] = (b.i[3] & ~c.i[3]) | (a.i[3] & c.i[3] );
  }

  inline void czero( const v4 &c, v4 &a ) {
    a.i[0] = a.i[0] & ~c.i[0];
    a.i[1] = a.i[1] & ~c.i[1];
    a.i[2] = a.i[2] & ~c.i[2];
    a.i[3] = a.i[3] & ~c.i[3];
  }

  inline void notcmov( const v4 &c, const v4 &a, v4 &b ) {
    b.i[0] = (b.i[0] & c.i[0]) | (a.i[0] & ~c.i[0] );
    b.i[1] = (b.i[1] & c.i[1]) | (a.i[1] & ~c.i[1] );
    b.i[2] = (b.i[2] & c.i[2]) | (a.i[2] & ~c.i[2] );
    b.i[3] = (b.i[3] & c.i[3]) | (a.i[3] & ~c.i[3] );
  }

  inline void notczero( const v4 &c, v4 &a ) {
    a.i[0] = a.i[0] & c.i[0];
    a.i[1] = a.i[1] & c.i[1];
    a.i[2] = a.i[2] & c.i[2];
    a.i[3] = a.i[3] & c.i[3];
  }

  ///////////////////////////////////
  // Memory manipulation functions //
  ///////////////////////////////////
  
  inline void load( const void *p, v4 &a ) {
    a.i[0] = ((const int *)p)[0];
    a.i[1] = ((const int *)p)[1];
    a.i[2] = ((const int *)p)[2];
    a.i[3] = ((const int *)p)[3];
  }
  
  inline void half_swizzle( const void *a0, const void *a1,
                            const void *a2, const void *a3, v4 &a, v4 &b ) {
    a.i[0] = ((const int *)a0)[0]; b.i[0] = ((const int*)a0)[1];
    a.i[1] = ((const int *)a1)[0]; b.i[1] = ((const int*)a1)[1];
    a.i[2] = ((const int *)a2)[0]; b.i[2] = ((const int*)a2)[1];
    a.i[3] = ((const int *)a3)[0]; b.i[3] = ((const int*)a3)[1];
  }
  
  inline void swizzle( const void *a0, const void *a1,
                       const void *a2, const void *a3,
                       v4 &a, v4 &b, v4 &c, v4 &d ) {
    a.i[0] = ((const int *)a0)[0]; b.i[0] = ((const int*)a0)[1];
    c.i[0] = ((const int *)a0)[2]; d.i[0] = ((const int*)a0)[3];
    
    a.i[1] = ((const int *)a1)[0]; b.i[1] = ((const int*)a1)[1];
    c.i[1] = ((const int *)a1)[2]; d.i[1] = ((const int*)a1)[3];
    
    a.i[2] = ((const int *)a2)[0]; b.i[2] = ((const int*)a2)[1];
    c.i[2] = ((const int *)a2)[2]; d.i[2] = ((const int*)a2)[3];
    
    a.i[3] = ((const int *)a3)[0]; b.i[3] = ((const int*)a3)[1];
    c.i[3] = ((const int *)a3)[2]; d.i[3] = ((const int*)a3)[3];
  }
  
  inline void store( const v4 &a, void *p ) {
    ((int *)p)[0] = a.i[0];
    ((int *)p)[1] = a.i[1];
    ((int *)p)[2] = a.i[2];
    ((int *)p)[3] = a.i[3];
  }
  
  inline void stream( const v4 &a, void *p ) {
    ((int *)p)[0] = a.i[0];
    ((int *)p)[1] = a.i[1];
    ((int *)p)[2] = a.i[2];
    ((int *)p)[3] = a.i[3];
  }
  
  inline void half_deswizzle( const v4 &a, const v4 &b,
                              void *a0, void *a1, void *a2, void *a3 ) {
    ((int *)a0)[0] = a.i[0]; ((int *)a0)[1] = b.i[0];
    ((int *)a1)[0] = a.i[1]; ((int *)a1)[1] = b.i[1];
    ((int *)a2)[0] = a.i[2]; ((int *)a2)[1] = b.i[2];
    ((int *)a3)[0] = a.i[3]; ((int *)a3)[1] = b.i[3];
  }
  
  inline void deswizzle( const v4 &a, const v4 &b, const v4 &c, const v4 &d,
                         void *a0, void *a1, void *a2, void *a3 ) {
    ((int *)a0)[0] = a.i[0]; ((int *)a0)[1] = b.i[0];
    ((int *)a0)[2] = c.i[0]; ((int *)a0)[3] = d.i[0];
    
    ((int *)a1)[0] = a.i[1]; ((int *)a1)[1] = b.i[1];
    ((int *)a1)[2] = c.i[1]; ((int *)a1)[3] = d.i[1];
    
    ((int *)a2)[0] = a.i[2]; ((int *)a2)[1] = b.i[2];
    ((int *)a2)[2] = c.i[2]; ((int *)a2)[3] = d.i[2];
    
    ((int *)a3)[0] = a.i[3]; ((int *)a3)[1] = b.i[3];
    ((int *)a3)[2] = c.i[3]; ((int *)a3)[3] = d.i[3];
  }

  /////////////////////////////////////////////////////////////////////////////

  /////////////////
  // v4int class //
  /////////////////

  class v4int : public v4 {
    
    ////////////////////
    // Friend classes //
    ////////////////////
    
    friend class v4float;
    
    //////////////////////////////////
    // Integer manipulation friends //
    //////////////////////////////////
    
    friend inline v4int abs( const v4int &a );
    
    //////////////////////////////////
    // Logical manipulation friends //
    //////////////////////////////////
    
    friend inline int any( const v4int &a );
    friend inline int all( const v4int &a );
    
    //////////////////////////////////
    // ostream manipulation friends //
    //////////////////////////////////
    
    friend inline std::ostream &operator <<( std::ostream &s, const v4int &a );
    
  public:
    
    ////////////////////////////////
    // Constructors / destructors //
    ////////////////////////////////
    
    v4int() {};                           // Default constructor
    v4int( const v4int &a ) {             // Copy constructor
      i[0] = a.i[0];
      i[1] = a.i[1];
      i[2] = a.i[2];
      i[3] = a.i[3];
    };
    v4int( const int a ) {                // Initialize from scalar
      i[0] = a;
      i[1] = a;
      i[2] = a;
      i[3] = a;
    };
    v4int( const int i0, const int i1,
           const int i2, const int i3 ) { // Initialize from scalars
      i[0] = i0;
      i[1] = i1;
      i[2] = i2;
      i[3] = i3;
    };
    ~v4int() {};                          // Destructor
    
    ////////////////////////////
    // Member access operator //
    ////////////////////////////
    
    int &operator()(const int n) {
      return i[n];
    };
    
    /////////////////////////////////////
    // Overload prefix unary operators //
    /////////////////////////////////////
    
#   define prefix_unary(op)                     \
    inline v4int operator op() {                \
      v4int b;                                  \
      b.i[0] = (op i[0]);                       \
      b.i[1] = (op i[1]);                       \
      b.i[2] = (op i[2]);                       \
      b.i[3] = (op i[3]);                       \
      return b;                                 \
    }
    prefix_unary(+);
    prefix_unary(-);
    inline v4int operator !() {
      v4int b;
      b.i[0] = i[0] ? 0 : -1;
      b.i[1] = i[1] ? 0 : -1;
      b.i[2] = i[2] ? 0 : -1;
      b.i[3] = i[3] ? 0 : -1;
      return b;
    }
    prefix_unary(~);
    prefix_unary(++);
    prefix_unary(--);
    // Note: Referencing (*) and dereferencing (&) apply to the whole vector
#   undef prefix_unary

    //////////////////////////////////////
    // Overload postfix unary operators //
    //////////////////////////////////////

#   define postfix_unary(op)                    \
    inline v4int operator op(int) {             \
      v4int b;                                  \
      b.i[0] = (i[0] op);                       \
      b.i[1] = (i[1] op);                       \
      b.i[2] = (i[2] op);                       \
      b.i[3] = (i[3] op);                       \
      return b;                                 \
    }
    postfix_unary(++);
    postfix_unary(--);
#   undef postfix_unary
  
    ///////////////////////////////
    // Overload binary operators //
    ///////////////////////////////
  
#   define binary(op)				\
    inline v4int operator op(const v4int &b) {	\
      v4int c;					\
      c.i[0] = i[0] op b.i[0];			\
      c.i[1] = i[1] op b.i[1];			\
      c.i[2] = i[2] op b.i[2];			\
      c.i[3] = i[3] op b.i[3];			\
      return c;					\
    }
    binary(+);
    binary(-);
    binary(*);
    binary(/);
    binary(%);
    binary(^);
    binary(&);
    binary(|);
    binary(<<);
    binary(>>);
#   undef binary
#   define binary_logical(op)			\
    inline v4int operator op(const v4int &b) {	\
      v4int c;					\
      c.i[0] = (i[0] op b.i[0]) ? -1 : 0;	\
      c.i[1] = (i[1] op b.i[1]) ? -1 : 0;	\
      c.i[2] = (i[2] op b.i[2]) ? -1 : 0;	\
      c.i[3] = (i[3] op b.i[3]) ? -1 : 0;	\
      return c;					\
    }
    binary_logical(<);
    binary_logical(>);
    binary_logical(==);
    binary_logical(!=);
    binary_logical(<=);
    binary_logical(>=);
    binary_logical(&&);
    binary_logical(||);
#   undef binary_logical
  
    ///////////////////////////////////
    // Overload assignment operators //
    ///////////////////////////////////
  
#   define assign(op)			        \
    inline v4int &operator op(const v4int &b) {	\
      i[0] op b.i[0];				\
      i[1] op b.i[1];				\
      i[2] op b.i[2];				\
      i[3] op b.i[3];				\
      return *this;				\
    }
    assign(=);
    assign(+=);
    assign(-=);
    assign(*=);
    assign(/=);
    assign(%=);
    assign(^=);
    assign(&=);
    assign(|=);
    assign(<<=);
    assign(>>=);
#   undef assign

  };

  ////////////////////////////////////
  // Integer manipulation functions //
  ////////////////////////////////////
  
  inline v4int abs( const v4int &a ) {
    v4int b;
    b.i[0] = (a.i[0]>=0) ? a.i[0] : -a.i[0];
    b.i[1] = (a.i[1]>=0) ? a.i[1] : -a.i[1];
    b.i[2] = (a.i[2]>=0) ? a.i[2] : -a.i[2];
    b.i[3] = (a.i[3]>=0) ? a.i[3] : -a.i[3];
    return b;
  }

  ////////////////////////////////////
  // Logical manipulation functions //
  ////////////////////////////////////

  inline int any( const v4int &a ) {
    return a.i[0] || a.i[1] || a.i[2] || a.i[3];
  }

  inline int all( const v4int &a ) {
    return a.i[0] && a.i[1] && a.i[2] && a.i[3];
  }

  ////////////////////////////////////
  // ostream manipulation functions //
  ////////////////////////////////////

  inline std::ostream &operator <<( std::ostream &s, const v4int &a ) {
    s << a.i[0] << " " << a.i[1] << " " << a.i[2] << " " << a.i[3];
    return s;
  }

  /////////////////////////////////////////////////////////////////////////////

  ///////////////////
  // v4float class //
  ///////////////////

  class v4float : public v4 {

    /////////////////////////////////////////
    // Floating point manipulation friends //
    /////////////////////////////////////////

#   define cmath_fr1(fn) friend inline v4float fn( const v4float &a )
#   define cmath_fr2(fn) friend inline v4float fn( const v4float &a,  \
                                                   const v4float &b )
    cmath_fr1(acos);  cmath_fr1(asin);  cmath_fr1(atan); cmath_fr2(atan2);
    cmath_fr1(ceil);  cmath_fr1(cos);   cmath_fr1(cosh); cmath_fr1(exp);
    cmath_fr1(fabs);  cmath_fr1(floor); cmath_fr2(fmod); cmath_fr1(log);
    cmath_fr1(log10); cmath_fr2(pow);   cmath_fr1(sin);  cmath_fr1(sinh);
    cmath_fr1(sqrt);  cmath_fr1(tan);   cmath_fr1(tanh);
    // Not implemented: frexp, ldexp, modf
#   if 0 // C99 library functions
    cmath_fr1(acosh);     cmath_fr1(asinh);      cmath_fr1(atanh);
    cmath_fr1(expm1);     cmath_fr1(log1p);      cmath_fr1(logb);     
    cmath_fr1(exp2);      cmath_fr1(log2);       cmath_fr2(hypot);    
    cmath_fr1(cbrt);      cmath_fr2(copysign);   cmath_fr1(erf);
    cmath_fr1(erfc);      cmath_fr1(lgamma);     cmath_fr1(tgamma);  
    cmath_fr1(rint);      cmath_fr2(nextafter);  cmath_fr2(nexttoward); 
    cmath_fr2(remainder); cmath_fr1(nearbyint);  cmath_fr1(round);
    cmath_fr1(trunc);     cmath_fr2(fdim);       cmath_fr2(fmax);
    cmath_fr2(fmin);
    // Not implemented: nan scalbn ilogb scalbln remquo lrint lround fma scalb
#   endif
#   undef cmath_fr1
#   undef cmath_fr2

    //////////////////////////////////
    // Logical manipulation friends //
    //////////////////////////////////

    friend inline int any( const v4float &a );
    friend inline int all( const v4float &a );

    //////////////////////////////////
    // ostream manipulation friends //
    //////////////////////////////////

    friend inline std::ostream &operator <<( std::ostream &s,
                                             const v4float &a );

    //////////////////////////
    // Other useful friends //
    //////////////////////////

    friend inline v4float rsqrt_approx( const v4float &a );
    friend inline v4float rsqrt( const v4float &a );
    friend inline v4float rcp_approx( const v4float &a );
    friend inline v4float rcp( const v4float &a );

  public:

    //////////////////
    // Constructors //
    //////////////////

    v4float() {};                               // Default constructor
    v4float( const v4float &a ) {               // Copy constructor
      f[0] = a.f[0];
      f[1] = a.f[1];
      f[2] = a.f[2];
      f[3] = a.f[3];
    };
    v4float( const float a ) {                  // Initialize from scalar
      f[0] = a;
      f[1] = a;
      f[2] = a;
      f[3] = a;
    };
    v4float( const float f0, const float f1,
             const float f2, const float f3 ) { // Initalize from scalars
      f[0] = f0;
      f[1] = f1;
      f[2] = f2;
      f[3] = f3;
    };
    v4float( const v4int &a ) {                 // int->float conversion
      f[0] = a.i[0];
      f[1] = a.i[1];
      f[2] = a.i[2];
      f[3] = a.i[3];
    };
    ~v4float() {};                              // Destructor
    
    ////////////////////////////
    // Member access operator //
    ////////////////////////////
    
    float &operator()(const int n) {
      return f[n];
    };
    
    /////////////////////////////////////
    // Overload prefix unary operators //
    /////////////////////////////////////

#   define prefix_unary(op)                    \
    inline v4float operator op() {             \
      v4float b;                               \
      b.f[0] = (op f[0]);                      \
      b.f[1] = (op f[1]);                      \
      b.f[2] = (op f[2]);                      \
      b.f[3] = (op f[3]);                      \
      return b;                                \
    }
    prefix_unary(+);
    prefix_unary(-);
    inline v4int operator !() {
      v4int b;
      b.i[0] = i[0] ? 0 : -1;
      b.i[1] = i[1] ? 0 : -1;
      b.i[2] = i[2] ? 0 : -1;
      b.i[3] = i[3] ? 0 : -1;
      return b;
    }
    prefix_unary(++);
    prefix_unary(--);
    // Note: Referencing (*) and deferencing (&) apply to the whole vector
#   undef prefix_unary

    //////////////////////////////////////
    // Overload postfix unary operators //
    //////////////////////////////////////

#   define postfix_unary(op)                    \
    inline v4float operator op(int) {           \
      v4float b;                                \
      b.f[0] = (f[0] op);                       \
      b.f[1] = (f[1] op);                       \
      b.f[2] = (f[2] op);                       \
      b.f[3] = (f[3] op);                       \
      return b;                                 \
    }
    postfix_unary(++);
    postfix_unary(--);
#   undef postfix_unary
    
    ///////////////////////////////
    // Overload binary operators //
    ///////////////////////////////
    
#   define binary(op)					\
    inline v4float operator op(const v4float &b) {	\
      v4float c;					\
      c.f[0] = f[0] op b.f[0];				\
      c.f[1] = f[1] op b.f[1];				\
      c.f[2] = f[2] op b.f[2];				\
      c.f[3] = f[3] op b.f[3];				\
      return c;						\
    }
    binary(+);
    binary(-);
    binary(*);
    binary(/);
#   undef binary
#   define binary_logical(op)				\
    inline v4int operator op(const v4float &b) {	\
      v4int c;						\
      c.i[0] = (i[0] op b.i[0]) ? -1 : 0;		\
      c.i[1] = (i[1] op b.i[1]) ? -1 : 0;		\
      c.i[2] = (i[2] op b.i[2]) ? -1 : 0;		\
      c.i[3] = (i[3] op b.i[3]) ? -1 : 0;		\
      return c;						\
    }
    binary_logical(<);
    binary_logical(>);
    binary_logical(==);
    binary_logical(!=);
    binary_logical(<=);
    binary_logical(>=);
    binary_logical(&&);
    binary_logical(||);
#   undef binary_logical

    ///////////////////////////////////
    // Overload assignment operators //
    ///////////////////////////////////

#   define assign(op)				        \
    inline v4float &operator op(const v4float &b) {	\
      f[0] op b.f[0];					\
      f[1] op b.f[1];					\
      f[2] op b.f[2];					\
      f[3] op b.f[3];					\
      return *this;					\
    }
    assign(=);
    assign(+=);
    assign(-=);
    assign(*=);
    assign(/=);
#   undef assign
    
  };

  ///////////////////////////////////////////
  // Floating point manipulation functions //
  ///////////////////////////////////////////
  
# define cmath_fr1(fn)                          \
  inline v4float fn( const v4float &a ) {       \
    v4float b;                                  \
    b.f[0] = std::fn(a.f[0]);                   \
    b.f[1] = std::fn(a.f[1]);                   \
    b.f[2] = std::fn(a.f[2]);                   \
    b.f[3] = std::fn(a.f[3]);                   \
    return b;                                   \
  }
# define cmath_fr2(fn)                                          \
  inline v4float fn( const v4float &a, const v4float &b ) {     \
    v4float c;                                                  \
    c.f[0] = std::fn(a.f[0],b.f[0]);                            \
    c.f[1] = std::fn(a.f[1],b.f[1]);                            \
    c.f[2] = std::fn(a.f[2],b.f[2]);                            \
    c.f[3] = std::fn(a.f[3],b.f[3]);                            \
    return c;                                                   \
  }
  cmath_fr1(acos);  cmath_fr1(asin);  cmath_fr1(atan); cmath_fr2(atan2);
  cmath_fr1(ceil);  cmath_fr1(cos);   cmath_fr1(cosh); cmath_fr1(exp);
  cmath_fr1(fabs);  cmath_fr1(floor); cmath_fr2(fmod); cmath_fr1(log);
  cmath_fr1(log10); cmath_fr2(pow);   cmath_fr1(sin);  cmath_fr1(sinh);
  cmath_fr1(sqrt);  cmath_fr1(tan);   cmath_fr1(tanh);
  // Not implemented: frexp, ldexp, modf
# if 0 // C99 library functions
  cmath_fr1(acosh);     cmath_fr1(asinh);      cmath_fr1(atanh);
  cmath_fr1(expm1);     cmath_fr1(log1p);      cmath_fr1(logb);     
  cmath_fr1(exp2);      cmath_fr1(log2);       cmath_fr2(hypot);    
  cmath_fr1(cbrt);      cmath_fr2(copysign);   cmath_fr1(erf);
  cmath_fr1(erfc);      cmath_fr1(lgamma);     cmath_fr1(tgamma);  
  cmath_fr1(rint);      cmath_fr2(nextafter);  cmath_fr2(nexttoward); 
  cmath_fr2(remainder); cmath_fr1(nearbyint);  cmath_fr1(round);
  cmath_fr1(trunc);     cmath_fr2(fdim);       cmath_fr2(fmax);
  cmath_fr2(fmin);
  // Not implemented: nan scalbn ilogb scalbln remquo lrint lround fma scalb
# endif
# undef cmath_fr1
# undef cmath_fr2

  ////////////////////////////////////
  // Logical manipulation functions //
  ////////////////////////////////////

  inline int any( const v4float &a ) {
    return a.f[0] || a.f[1] || a.f[2] || a.f[3];
  }
  
  inline int all( const v4float &a ) {
    return a.f[0] && a.f[1] && a.f[2] && a.f[3];
  }

  ////////////////////////////////////
  // ostream manipulation functions //
  ////////////////////////////////////
  
  inline std::ostream &operator <<( std::ostream &s, const v4float &a ) {
    s << a.f[0] << " " << a.f[1] << " " << a.f[2] << " " << a.f[3];
    return s;
  }
  
  ////////////////////////////
  // Other useful functions //
  ////////////////////////////
  
  inline v4float rsqrt_approx( const v4float &a ) {
    v4float b;
    b.f[0] = std::sqrt(1/a.f[0]);
    b.f[1] = std::sqrt(1/a.f[1]);
    b.f[2] = std::sqrt(1/a.f[2]);
    b.f[3] = std::sqrt(1/a.f[3]);
    return b;
  }
  
  inline v4float rsqrt( const v4float &a ) {
    v4float b;
    b.f[0] = std::sqrt(1/a.f[0]);
    b.f[1] = std::sqrt(1/a.f[1]);
    b.f[2] = std::sqrt(1/a.f[2]);
    b.f[3] = std::sqrt(1/a.f[3]);
    return b;
  }
  
  inline v4float rcp_approx( const v4float &a ) {
    v4float b;
    b.f[0] = 1/a.f[0];
    b.f[1] = 1/a.f[1];
    b.f[2] = 1/a.f[2];
    b.f[3] = 1/a.f[3];
    return b;
  }
  
  inline v4float rcp( const v4float &a ) {
    v4float b;
    b.f[0] = 1/a.f[0];
    b.f[1] = 1/a.f[1];
    b.f[2] = 1/a.f[2];
    b.f[3] = 1/a.f[3];
    return b;
  }

} // namespace v4

#endif
