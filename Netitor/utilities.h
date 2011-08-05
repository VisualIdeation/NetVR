/*******************************************************************
Copyright (c) 2006-2008 Braden Pellett and Jordan Van Aalsburg

This file is part of NetVR.

NetVR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

NetVR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with NetVR.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************/

#ifndef NETITOR_UTILITIES_H_INCLUDED
#define NETITOR_UTILITIES_H_INCLUDED

#include <string>
#include <sstream>
#include <map>
#include <vector>

namespace util {

template <class T>
inline std::string toString(T value)
{
	std::ostringstream ss;
	ss << value;
	return ss.str();
}

template <class T>
inline std::string toString(T value, std::ios_base::fmtflags flags)
{
	std::ostringstream ss;
	ss.flags(flags);
	ss << value;
	return ss.str();
}

template <class T>
inline std::string toString(T value, std::ios_base::fmtflags flags, std::streamsize width)
{
	std::ostringstream ss;
	ss.flags(flags);
	ss.width(width);
	ss << value;
	return ss.str();
}

template <>
inline std::string toString<bool>(bool value)
{
	if (value)
		return "true";
	else
		return "false";
}

template <class T>
inline T fromString(std::string valueString)
{
	T value;
	std::istringstream ss(valueString);
	ss >> value;
	return value;
}

template <>
inline bool fromString<bool>(std::string valueString)
{
	if (   valueString == "true"
	    || valueString == "True"
	    || valueString == "TRUE"
	    || valueString == "t"
	    || valueString == "T")
	{
		return true;
	} else
		return false;
}


class PointerToID
{
	typedef std::map<const void*,unsigned> PtrIDMap;

	public:
	PointerToID() : nextID(0) {}
	~PointerToID() {}

	const unsigned& operator[] (const void* const pointer) {
		std::pair<PtrIDMap::iterator,bool> result = ids.insert(std::make_pair(pointer, nextID));
		if (result.second) {
			++nextID;
		}
		return result.first->second;
	}

	private:
	unsigned nextID;
	PtrIDMap ids;
};

class IDToPointer
{
	public:
	IDToPointer() {}
	~IDToPointer() {}
	
	void*& operator[] (const unsigned& theID) {
		if (ptrs.size() > theID) {
			return ptrs[theID];
		} else {
			ptrs.resize(theID+1);
			return ptrs[theID];
		}
	}
	
	private:
	std::vector<void*> ptrs;
};

} // namespace util

#endif
