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

#include "caveapp.h"

//const std::string Student::typeName = "student"; // the name for this class
//const std::string Met::typeName = "met";

int main(int argc, char *argv[]) {

  try {
    char **appDefaults = 0;
    NetworkApplication app(argc, argv, appDefaults);
    app.run();
  } 
  catch(std::runtime_error err) { }
  
  return 0;
}
