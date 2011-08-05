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

#ifndef DIRECTORY_READER_H
#define DIRECTORY_READER_H

#include <errno.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <set>
#include <iterator>
#include <iostream>
#include <algorithm>

class Directory {
  public:
    Directory() { }

    Directory(const char *dir)
        {
            read(".");
        }
    
    void addExtensionFilter(std::string ext)
        {
            filters.insert(ext);
        }
    void read(const char *dir=".")       
        {
            DIR *pdir;
            struct dirent *pent;
            
            pdir=opendir(dir);
            if(!pdir) {
                std::cerr << "opendir() failure. terminating." << std::endl;
                exit(1);
            }
            errno=0;
            while((pent=readdir(pdir))) {
                std::string file = pent->d_name;
                
                if(filters.size() == 0)
                    dirlist.push_back(file);
                else {                        
                    for(f_itr filter=filters.begin(); filter!=filters.end(); ++filter)
                        if(file.find(*filter) != std::string::npos)
                            dirlist.push_back(file);
                }                    
            }
            if(errno)
                std::cerr << "readdir() failure. terminating." << std::endl;

            closedir(pdir);
        }
    std::vector<std::string> contents() const 
        {
            return dirlist;
        }
    void printDirectory() const 
        {
            std::vector<std::string>::const_iterator file;
            for(file=dirlist.begin(); file!=dirlist.end(); ++file)
                std::cout << *file << std::endl;
        }

    void sort()
        {
            std::sort(dirlist.begin(), dirlist.end());
        }

  protected:
    std::vector<std::string> dirlist;
    std::set<std::string>    filters;
    
    typedef std::set<std::string>::const_iterator f_itr;
};

#endif
