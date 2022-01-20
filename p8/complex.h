#pragma once // prevents the symbols in this file from being multiply defined

/*
 * complex.h
 * Header file for a simple complex number datatype.
 */

// we need these in order to prototype our output operator (<<)
#include<iostream>
using std::ostream;
using std::istream;

//our datatype to represent complex numbers:
class complex
{
public:
	// constructors:
	complex(); // sets the value to (0,0)
	complex(double re, double im); // sets to (re,im)
	complex(const complex& z); // makes this a copy of z.

	// other member functions:
	// this function returns the norm (the length as a vector in R^2)
	double norm();

	// this function returns the complex conjugate, i.e., the reflection over
	// the real axis
	complex conj();

	// data members: the information we need to represent a complex number
	double real;
	double imag;
};

/////////////////operator overloads////////////////////
// Here, we define all the usual operators for complex numbers.
complex operator+(const complex& w, const complex& z); // addition
complex operator-(const complex& w, const complex& z); // subtraction
complex operator-(const complex& w); //negation (returns -w)
complex operator*(const complex& w, const complex& z); // multiplication
complex operator/(complex& w, complex& z); // division
complex operator^(const complex& w, int a); // exponentiation (integer exponent)
ostream& operator<<(ostream& o, const complex& z); // write to stdout
istream& operator>>(istream& i, complex& z); // read from stdin
/* 
 * Note: we could have made many of the above functions member functions,
 * but they would look a little funny, since the first parameter is
 * implicit:  "w+z"  would translate into "w.operator+(z)", so the prototype
 * would become:  complex complex::operator+(const complex& z); which lacks
 * the symmetry you would expect.
 */
///////////////////////////////////////////////////////
