/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
 *
 * All rights reserved. Holger Seelig <holger.seelig@yahoo.de>.
 *
 * THIS IS UNPUBLISHED SOURCE CODE OF create3000.
 *
 * The copyright notice above does not evidence any actual of intended
 * publication of such source code, and is an unpublished work by create3000.
 * This material contains CONFIDENTIAL INFORMATION that is the property of
 * create3000.
 *
 * No permission is granted to copy, distribute, or create derivative works from
 * the contents of this software, in whole or in part, without the prior written
 * permission of create3000.
 *
 * NON-MILITARY USE ONLY
 *
 * All create3000 software are effectively free software with a non-military use
 * restriction. It is free. Well commented source is provided. You may reuse the
 * source in any way you please with the exception anything that uses it must be
 * marked to indicate is contains 'non-military use only' components.
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
 *
 * This file is part of the Titania Project.
 *
 * Titania is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 3 only, as published by the
 * Free Software Foundation.
 *
 * Titania is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
 * details (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version 3
 * along with Titania.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
 * copy of the GPLv3 License.
 *
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#ifndef __TITANIA_MATH_NUMBERS_ROTATION4_H__
#define __TITANIA_MATH_NUMBERS_ROTATION4_H__

#include <cmath>
#include <istream>
#include <ostream>
#include <stdexcept>

#include "../../Utility/MemberValue.h"
#include "../Functional.h"
#include "Quaternion.h"
#include "Vector3.h"

namespace titania {
namespace math {

/**
 *  Template to represent an arbitary rotation about an axis.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of axis and angle.
 */
template <class Type>
class rotation4
{
public:

	///  Size typedef.  Used for size and indices.
	typedef size_t size_type;

	///  Value typedef.
	typedef Type value_type;

	///  Quaternion typedef.
	typedef quaternion <Type> quaternion_type;

	///  Vector3 typedef.
	typedef vector3 <Type> vector3_type;

	///  Member value typedef.
	typedef basic::member_value <Type, rotation4 <Type>> member_value_type;

	///  @name Constructors

	///  Default constructor.
	///  The rotation will be set to its default value 0 0 1  0.
	constexpr
	rotation4 () :
		value (Type (), Type (), Type (), Type (1)) { }

	///  Copy constructor.
	template <class T>
	constexpr
	rotation4 (const rotation4 <T> & rotation) :
		value (rotation .quat ()) { }

	///  Construct a rotation from normalized @a quaternion.
	template <class T>
	explicit
	constexpr
	rotation4 (const quaternion <T> & quaternion) :
		value (quaternion) { }

	///  Components constructor. Construct a rotation from @a x, @a y, @a z and @a angle.
	rotation4 (const Type & x, const Type & y, const Type & z, const Type & angle);

	///  Construct a rotation from @a axis and @a angle.
	template <class T>
	rotation4 (const vector3 <T> & axis, const Type & angle);

	///  Construct a rotation from @a fromVector and @a toVector.
	///  The vectors are normalized and the rotation value that would rotate
	///  from the fromVector to the toVector is stored in the object.
	template <class T>
	rotation4 (const vector3 <T> & fromVector, const vector3 <T> & toVector);

	///  @name Assignment operator

	///  Assign @a rotation to this rotation.
	template <class T>
	rotation4 &
	operator = (const rotation4 <T> &);

	///  @name Element access

	///  Set x axis of this rotation.
	void
	x (const Type &);

	///  Returns x axis of this rotation.
	Type
	x () const { return axis () .x (); }

	///  Set y axis of this rotation.
	void
	y (const Type &);

	///  Returns y axis of this rotation.
	Type
	y () const { return axis () .y (); }

	///  Set z axis of this rotation.
	void
	z (const Type &);

	///  Returns z axis of this rotation.
	Type
	z () const { return axis () .z (); }

	///  Set @a axis of this rotation from vector.
	template <class T>
	void
	axis (const vector3 <T> & value) { *this = rotation4 (value, angle ()); }

	///  Returns axis of this rotation.
	vector3_type
	axis () const;

	///  Set angle of this rotation.
	void
	angle (const Type &);

	///  Returns angle of this rotation.
	Type
	angle () const;

	member_value_type
	operator [ ] (const size_type);

	Type
	operator [ ] (const size_type) const;

	///  Get @a x, @a y, @a z and @a angle componentwise.
	template <class T>
	void
	get (T &, T &, T &, T &) const;

	///  Set quaternion of this rotation.
	template <class T>
	void
	quat (const quaternion <T> & q) { value = q; }

	///  Returns quaternion of this rotation.
	const quaternion <Type> &
	quat () const { return value; }

	///  @name Arithmetic operations

	///  Invert this quaternion in place.
	rotation4 &
	inverse ();

	///  Multiply this rotations angle by @a value.
	rotation4 &
	operator *= (const Type &);

	///  Multiply this rotation by @a rotation.
	template <class T>
	rotation4 &
	operator *= (const rotation4 <T> &);

	///  Divide this rotations angle by @a value.
	rotation4 &
	operator /= (const Type &);


private:

	quaternion <Type> value;

};

template <class Type>
rotation4 <Type>::rotation4 (const Type & x, const Type & y, const Type & z, const Type & angle)
{
	Type scale = std::sqrt (x * x + y * y + z * z);

	if (scale == 0)
	{
		value = quaternion <Type> (0, 0, 0, 1);
		return;
	}

	// Calculate quaternion

	Type halfTheta = angle / 2;
	scale = std::sin (halfTheta) / scale;

	value = quaternion <Type> (x * scale,
	                           y * scale,
	                           z * scale,
	                           std::cos (halfTheta));
}

template <class Type>
template <class T>
inline
rotation4 <Type>::rotation4 (const vector3 <T> & axis, const Type & angle)
{
	*this = rotation4 (axis .x (), axis .y (), axis .z (), angle);
}

template <class Type>
template <class T>
rotation4 <Type>::rotation4 (const vector3 <T> & fromVector, const vector3 <T> & toVector)
{
	Type cos_theta = dot (normalize (fromVector), normalize (toVector));

	if (cos_theta > -1)
	{
		Type angle = std::acos (cos_theta);
		*this = rotation4 (cross (fromVector, toVector), angle);
	}
	else
	{
		vector3 <Type> normal = normalize (toVector - fromVector);

		if (normal .x ())
		{
			Type x = -normal .y () / normal .x ();
			*this = rotation4 (x, 1, 0, M_PI);
		}
		else if (normal .y ())
		{
			Type y = -normal .z () / normal .y ();
			*this = rotation4 (0, y, 1, M_PI);
		}
		else if (normal .z ())
		{
			Type z = -normal .x () / normal .z ();
			*this = rotation4 (1, 0, z, M_PI);
		}
		else
			*this = rotation4 (0, 0, 1, 0);
	}
}

template <class Type>
template <class T>
inline
rotation4 <Type> &
rotation4 <Type>::operator = (const rotation4 <T> & rotation)
{
	value = rotation .quat ();
	return *this;
}

template <class Type>
void
rotation4 <Type>::x (const Type & value)
{
	Type x, y, z, angle;

	get (x, y, z, angle);

	*this = rotation4 (value, y, z, angle);
}

template <class Type>
void
rotation4 <Type>::y (const Type & value)
{
	Type x, y, z, angle;

	get (x, y, z, angle);

	*this = rotation4 (x, value, z, angle);
}

template <class Type>
void
rotation4 <Type>::z (const Type & value)
{
	Type x, y, z, angle;

	get (x, y, z, angle);

	*this = rotation4 (x, y, value, angle);
}

template <class Type>
vector3 <Type>
rotation4 <Type>::axis () const
{
	if (std::abs (value .w ()) == 1)
		return vector3_type (0, 0, 1);

	return normalize (imag (value));
}

template <class Type>
inline
void
rotation4 <Type>::angle (const Type & value)
{
	*this = rotation4 (axis (), value);
}

template <class Type>
Type
rotation4 <Type>::angle () const
{
	if (std::abs (value .w () == 1))
		return 0;

	return 2 * std::acos (value .w ());
}

///  Access components by @a index.
template <class Type>
typename rotation4 <Type>::member_value_type
rotation4 <Type>::operator [ ] (const size_type index)
{
	switch (index)
	{
		case 0: return member_value_type (&rotation4::x, &rotation4::x, this);
		case 1: return member_value_type (&rotation4::y, &rotation4::y, this);
		case 2: return member_value_type (&rotation4::z, &rotation4::z, this);
		case 3: return member_value_type (&rotation4::angle, &rotation4::angle, this);
	}

	throw std::out_of_range ("index out of range");
}

///  Access components by @a index.
template <class Type>
Type
rotation4 <Type>::operator [ ] (const size_type index) const
{
	switch (index)
	{
		case 0: return x ();
		case 1: return y ();
		case 2: return z ();
		case 3: return angle ();
	}

	throw std::out_of_range ("index out of range");
}

template <class Type>
template <class T>
void
rotation4 <Type>::get (T & x, T & y, T & z, T & angle) const
{
	if (std::abs (value .w ()) == 1)
	{
		x     = 0;
		y     = 0;
		z     = 1;
		angle = 0;
		return;
	}

	vector3_type vector = normalize (imag (value));

	x     = vector .x ();
	y     = vector .y ();
	z     = vector .z ();
	angle = 2 * std::acos (value .w ());
}

template <class Type>
inline
rotation4 <Type> &
rotation4 <Type>::inverse ()
{
	value .inverse ();
	return *this;
}

template <class Type>
rotation4 <Type> &
rotation4 <Type>::operator *= (const Type & t)
{
	angle (angle () * t);
	return *this;
}

template <class Type>
template <class T>
inline
rotation4 <Type> &
rotation4 <Type>::operator *= (const rotation4 <T> & rotation)
{
	value .multLeft (rotation .quat ());
	return *this;
}

template <class Type>
rotation4 <Type> &
rotation4 <Type>::operator /= (const Type & t)
{
	angle (angle () / t);
	return *this;
}

///  @relates rotation4
///  @name Comparision operations

///  Compares two rotation numbers.
///  Returns true if @a a is equal to @a b.
template <class Type>
inline
constexpr quaternion <Type>
quat (const rotation4 <Type> & rotation)
{
	return rotation .quat ();
}

///  Compares two rotation numbers.
///  Returns true if @a a is equal to @a b.
template <class Type>
inline
constexpr bool
operator == (const rotation4 <Type> & lhs, const rotation4 <Type> & rhs)
{
	return lhs .quat () == rhs .quat ();
}

///  Compares two rotation numbers.
///  Returns false if @a a is not equal to @a b.
template <class Type>
inline
constexpr bool
operator not_eq (const rotation4 <Type> & lhs, const rotation4 <Type> & rhs)
{
	return lhs .quat () not_eq rhs .quat ();
}

///  @relates rotation4
///  @name Arithmetic operations

///  Returns the inverse rotation for @a rotation.
template <class Type>
inline
rotation4 <Type>
operator ~ (const rotation4 <Type> & rotation)
{
	return rotation4 <Type> (rotation) .inverse ();
}

///  Multiply the angle of @a rotation by @a t.
template <class Type>
inline
rotation4 <Type>
operator * (const rotation4 <Type> & rotation, const Type & t)
{
	return rotation4 <Type> (rotation) *= t;
}

///  Multiply the angle of @a rotation by @a t.
template <class Type>
inline
rotation4 <Type>
operator * (const Type & t, const rotation4 <Type> & rotation)
{
	return rotation4 <Type> (rotation) *= t;
}

///  Left multiply @a lhs by @a rhs.
template <class Type>
inline
rotation4 <Type>
operator * (const rotation4 <Type> & lhs, const rotation4 <Type> & rhs)
{
	return rotation4 <Type> (rhs .quat () * lhs .quat ());
}

///  Returns the value of @a vector left multiplied by the quaternion corresponding to this object's rotation.
template <class Type>
inline
vector3 <Type>
operator * (const rotation4 <Type> & rotation, const vector3 <Type> & vector)
{
	return rotation .quat () * vector;
}

///  Divide the angle of @a rotation by @a t.
template <class Type>
inline
rotation4 <Type>
operator / (const rotation4 <Type> & rotation, const Type & t)
{
	return rotation4 <Type> (rotation) /= t;
}

///  Return a rotation where the angle is @a t / angle.
template <class Type>
inline
rotation4 <Type>
operator / (const Type & t, const rotation4 <Type> & rotation)
{
	rotation4 <Type> r = rotation4 <Type> (rotation);
	r .angle (t / r .angle ());
	return r;
}

///  Spherical linear interpolate between @a source quaternion and @a destination quaternion by an amout of @a t.
template <class Type, class T>
inline
rotation4 <Type>
slerp (const rotation4 <Type> & source,
       const rotation4 <Type> & destination,
       const T & t)
{
	return rotation4 <Type> (math::slerp (source .quat (), destination .quat (), t));
}

///  @relates rotation4
///  @name Input/Output operations

///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, rotation4 <Type> & rotation)
{
	Type x, y, z, angle;

	istream >> x >> y >> z >> angle;

	if (istream)
		rotation = rotation4 <Type> (x, y, z, angle);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const rotation4 <Type> & rotation)
{
	Type x, y, z, angle;

	rotation .get (x, y, z, angle);

	return ostream
	       << x << ' '
	       << y << ' '
	       << z << ' '
	       << angle;
}

extern template class rotation4 <float>;
extern template class rotation4 <double>;
extern template class rotation4 <long double>;

//
extern template std::istream & operator >> (std::istream &, rotation4 <float> &);
extern template std::istream & operator >> (std::istream &, rotation4 <double> &);
extern template std::istream & operator >> (std::istream &, rotation4 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const rotation4 <float> &);
extern template std::ostream & operator << (std::ostream &, const rotation4 <double> &);
extern template std::ostream & operator << (std::ostream &, const rotation4 <long double> &);

} // math
} // titania

#endif
