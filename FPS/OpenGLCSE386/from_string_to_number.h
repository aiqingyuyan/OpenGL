#ifndef _FROM_STRING_TO_NUMBER_H_
#define _FROM_STRING_TO_NUMBER_H_

	#include <string>
	#include <sstream>
	#include <iostream>
	/*
	* @author: Yan Yu
	* This function helps to convert a string to number (int, float, double)
	* It is type safe
	*/

	// the third parameter of from_string() should be 
	// one of std::hex, std::dec or std::oct
	// T& t: target type you want to convert to, i.e. float, double
	template <class T>
	bool from_string_to_number(T& t, std::string& s, std::ios_base& (*f)(std::ios_base&))
	{
		std::istringstream iss(s);
		return !(iss>> f >> t).fail();
	}

#endif