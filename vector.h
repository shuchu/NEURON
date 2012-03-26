#ifndef __VECTOR_H__
#define __VECTOR_H__

template <class T>
class vec3 {
public:
    typedef T value_type;
    int size() const { return 3;}

    ////////////////////////////////////////////////////////
    //
    //  Constructors
    //
    ////////////////////////////////////////////////////////

    // Default/scalar constructor
    vec3(const T & t = T()) {
        for(int i = 0; i < size(); i++) _array[i] = t;
    }

    // Construct from array
    vec3(const T * tp) {
        for(int i = 0; i < size(); i++) _array[i] = tp[i];
    }

    // Construct from explicit values
    vec3( const T v0, const T v1, const T v2) {
        x = v0;
        y = v1;
        z = v2;
    }

    
    const T * get_value() const {
        return _array;
    }
    
    vec3<T> & set_value( const T * rhs ) {
        for(int i = 0; i < size(); i++) _array[i] = rhs[i];
        return *this;
    }
    
    // indexing operators
    T & operator [] ( int i ) {
        return _array[i];
    }
    
    const T & operator [] ( int i ) const {
        return _array[i];
    }

    // type-cast operators
    operator T * () {
        return _array;
    }

    operator const T * () const {
        return _array;
    }

    ////////////////////////////////////////////////////////
    //
    //  Math operators
    //
    ////////////////////////////////////////////////////////

    // scalar multiply assign
    friend vec3<T> & operator *= ( vec3<T> &lhs, T d ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] *= d;
        return lhs;
    }

    // component-wise vector multiply assign
    friend vec3<T> & operator *= ( vec3<T> &lhs, const vec3<T> &rhs ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] *= rhs[i];
        return lhs;
    }

    // scalar divide assign
    friend vec3<T> & operator /= ( vec3<T> &lhs, T d ) {
        if(d == 0) return lhs;
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] /= d;
        return lhs;
    }

    // component-wise vector divide assign
    friend vec3<T> & operator /= ( vec3<T> &lhs, const vec3<T> & rhs ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] /= rhs._array[i];
        return lhs;
    }

    // component-wise vector add assign
    friend vec3<T> & operator += ( vec3<T> &lhs, const vec3<T> & rhs ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] += rhs._array[i];
        return lhs;
    }

    // component-wise vector subtract assign
    friend vec3<T> & operator -= ( vec3<T> &lhs, const vec3<T> & rhs ) {
        for(int i = 0; i < lhs.size(); i++) lhs._array[i] -= rhs._array[i];
        return lhs;
    }

    // unary negate
    friend vec3<T> operator - ( const vec3<T> &rhs) {
        vec3<T> rv;
        for(int i = 0; i < rhs.size(); i++) rv._array[i] = -rhs._array[i];
        return rv;
    }

    // vector add
    friend vec3<T> operator + ( const vec3<T> & lhs, const vec3<T> & rhs) {
        vec3<T> rt(lhs);
        return rt += rhs;
    }

    // vector subtract 
    friend vec3<T> operator - ( const vec3<T> & lhs, const vec3<T> & rhs) {
        vec3<T> rt(lhs);
        return rt -= rhs;
    }

    // scalar multiply
    friend vec3<T> operator * ( const vec3<T> & lhs, T rhs) {
        vec3<T> rt(lhs);
        return rt *= rhs;
    }

    // scalar multiply
    friend vec3<T> operator * ( T lhs, const vec3<T> & rhs) {
        vec3<T> rt(lhs);
        return rt *= rhs;
    }

    // vector component-wise multiply
    friend vec3<T> operator * ( const vec3<T> & lhs, const vec3<T> & rhs){
        vec3<T> rt(lhs);
        return rt *= rhs;
    }

    // scalar multiply
    friend vec3<T> operator / ( const vec3<T> & lhs, T rhs) {
        vec3<T> rt(lhs);
        return rt /= rhs;
    }

    // vector component-wise multiply
    friend vec3<T> operator / ( const vec3<T> & lhs, const vec3<T> & rhs){
        vec3<T> rt(lhs);
        return rt /= rhs;
    }

    ////////////////////////////////////////////////////////
    //
    //  Comparison operators
    //
    ////////////////////////////////////////////////////////

    // equality
    friend bool operator == ( const vec3<T> &lhs, const vec3<T> &rhs ) {
        bool r = true;
        for (int i = 0; i < lhs.size(); i++)
            r &= lhs._array[i] == rhs._array[i];
        return r;
    }

    // inequality
    friend bool operator != ( const vec3<T> &lhs, const vec3<T> &rhs ) {
        bool r = true;
        for (int i = 0; i < lhs.size(); i++)
            r &= lhs._array[i] != rhs._array[i];
        return r;
    }

    ////////////////////////////////////////////////////////////////////////////////
    //
    // dimension specific operations
    //
    ////////////////////////////////////////////////////////////////////////////////

    // cross product
    friend vec3<T> cross( const vec3<T> & lhs, const vec3<T> & rhs) {
        vec3<T> r;

        r.x = lhs.y * rhs.z - lhs.z * rhs.y;
        r.y = lhs.z * rhs.x - lhs.x * rhs.z;
        r.z = lhs.x * rhs.y - lhs.y * rhs.x;

        return r;
    }
   
    union {
        struct {
            T x, y, z;          // standard names for components
        };
        struct {
            T s, t, r;          // standard names for components
        };
        T _array[3];     // array access
    };
};

#endif