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

#ifndef XML_OSTREAM_H_INCLUDED
#define XML_OSTREAM_H_INCLUDED

#include <XML/common.h>

#include <list>
#include <string>
#include <ostream>


namespace XML {


class ostream
{
	public:
	ostream(std::ostream& stdOutStream);
	virtual ~ostream();

	ostream& reset();

	ostream& put(const Token* xmlToken);
	ostream& flush();
	bool good();
	
	ostream& writeElementStart(const std::string& name, const AttributeMap& attributes)
			throw(Failure);
	ostream& writeCData(const std::string& cData) throw(Failure);
	ostream& writeElementEnd() throw(Failure);
	ostream& writeWhitespace(const std::string& whitespace) throw(Failure);

	// Accessors
	unsigned getDepth();
	std::string getParentName();
	bool outsideRoot();

	private:
	std::ostream& stdOutStream;
	std::list<std::string> ancestors;
	bool openTag;
	bool wroteRoot;
};


} // namespace XML

#endif
