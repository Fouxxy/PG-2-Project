// PG Tools, (c)2021-2022 Tomas Fabian
// 
// May only be used for non-commercial educational purposes at VSB-TUO, FEECS,
// Dept. of Computer Science and may not be shared with any third parties in any form.

#ifndef VECTOR2_H_
#define VECTOR2_H_

#include <array>

#include "vector3.h"

/*! \struct Vector2
\brief Dvourozm�rn� (2D) vektor.

Implementace dvouslo�kov�ho re�ln�ho vektoru podporuj�c� z�kladn� matematick� operace.

\note
Vektor se pova�uje za sloupcov�, p�esto�e je v koment���ch pro jednoduchost uv�d�n jako ��dkov�.

\code{.cpp}
Vector2 v = Vector2( 2.0f, -7.8f );
v.Normalize();
\endcode

\author Tom� Fabi�n
\version 0.95
\date 2007-2021
*/
class Vector2
{
public:
	union	// anonymn� unie
	{
		struct
		{
			float x; /*!< Prvn� slo�ka vektoru. */
			float y; /*!< Druh� slo�ka vektoru. */			
		};

		struct
		{
			float u; /*!< Prvn� slo�ka vektoru. */
			float v; /*!< Druh� slo�ka vektoru. */
		};

		std::array<float, 2> data; /*!< Pole slo�ek vektoru. */		
	};

	//! V�choz� konstruktor.
	/*!
	Inicializuje v�echny slo�ky vektoru na hodnotu nula,
	\f$\mathbf{v}=\mathbf{0}\f$.
	*/
	Vector2() : x( 0 ), y( 0 ) { }	

	//! Obecn� konstruktor.
	/*!
	Inicializuje slo�ky vektoru podle zadan�ch hodnot parametr�,
	\f$\mathbf{v}=(x,y,z)\f$.

	\param x prvn� slo�ka vektoru.
	\param y druh� slo�ka vektoru.	
	*/
	Vector2( const float x, const float y ) : x( x ), y( y ) { }

	//! Konstruktor z pole.
	/*!
	Inicializuje slo�ky vektoru podle zadan�ch hodnot pole,

	\param v ukazatel na prvn� slo�ka vektoru.	
	*/
	Vector2( const float * v );

	//! L2-norma vektoru.
	/*!
	\return x Hodnotu \f$\mathbf{||v||}=\sqrt{x^2+y^2+z^2}\f$.
	*/
	float L2Norm() const;

	//! Druh� mocnina L2-normy vektoru.
	/*!
	\return Hodnotu \f$\mathbf{||v||^2}=x^2+y^2+z^2\f$.
	*/
	float SqrL2Norm() const;

	//! Normalizace vektoru.
	/*!
	Po proveden� operace bude m�t vektor jednotkovou d�lku.
	*/
	float Normalize();

	//! Vektorov� sou�in.
	/*!
	\param v vektor \f$\mathbf{v}\f$.

	\return Vektor \f$(\mathbf{u}_x \mathbf{v}_z - \mathbf{u}_z \mathbf{v}_y,
	\mathbf{u}_z \mathbf{v}_x - \mathbf{u}_x \mathbf{v}_z,
	\mathbf{u}_x \mathbf{v}_y - \mathbf{u}_y \mathbf{v}_x)\f$.
	*/
	Vector3 CrossProduct( const Vector2 & v ) const;	

	/* projects vector v orhogonally onto the line spanned by this vector */
	Vector2 Project( const Vector2 & v ) const;

	Vector2 Abs() const;

	Vector2 Max( const float a = 0 ) const;

	float max_value() const;

	float min_value() const;

	//! Skal�rn� sou�in.
	/*!		
	\return Hodnotu \f$\mathbf{u}_x \mathbf{v}_x + \mathbf{u}_y \mathbf{v}_y + \mathbf{u}_z \mathbf{v}_z)\f$.
	*/
	float DotProduct( const Vector2 & v ) const;

	//! Rotace.
	/*!		
	\return Vektor orotovan� kolem po��tku o \f$\phi\f$ radi�n� v kladn�m smyslu.
	*/
	//Vector2 Rotate( const float phi );

	//! Index nejv�t�� slo�ky vektoru.
	/*!
	\param absolute_value index bude ur�en podle absolutn� hodnoty slo�ky

	\return Index nejv�t�� slo�ky vektoru.
	*/
	char LargestComponent( const bool absolute_value = false );	

	void Print() const;

	// --- oper�tory ------

	friend Vector2 operator-( const Vector2 & v );

	friend Vector2 operator+( const Vector2 & u, const Vector2 & v );
	friend Vector2 operator-( const Vector2 & u, const Vector2 & v );

	friend Vector2 operator*( const Vector2 & v, const float a );	
	friend Vector2 operator*( const float a, const Vector2 & v );
	friend Vector2 operator*( const Vector2 & u, const Vector2 & v );

	friend Vector2 operator/( const Vector2 & v, const float a );

	friend void operator+=( Vector2 & u, const Vector2 & v );
	friend void operator-=( Vector2 & u, const Vector2 & v );
	friend void operator*=( Vector2 & v, const float a );
	friend void operator/=( Vector2 & v, const float a );		
	friend bool operator==( const Vector2 & u, const Vector2 & v );
};

static_assert( sizeof( Vector2 ) == 2 * sizeof( float ), "Vector3 size is not correct" );

#endif // VECTOR2_H_
