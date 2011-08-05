#!/usr/bin/python

import os
import sys


class ActorData:
    def __init__(self, max):

        file = open('NDactors.net')

        file.readline()
        file.readline()

        self.movieList = []
        self.actorList = []
        self.edgeList  = []

        count = 1

        while(count < max):

            count += 1

            actors = file.readline().split(' ')

            self.movieList.append(actors[0])

            for id in range(1,len(actors)):

                if int(actors[id]) not in self.actorList:
                    self.actorList.append(int(actors[id]))

                edge = []
                edge.append(actors[0])
                edge.append(actors[id])

                self.edgeList.append(edge)

    def toXML(self):

        print "<network>"

        print "\t<nodes>"

        self.movieList.sort()
        for movie in self.movieList:
            print "\t\t<node type=\"movie\" id=\"%i\"/>"%int(movie)

        self.actorList.sort()
        for actor in self.actorList:
            print "\t\t<node type=\"actor\" id=\"%i\"/>"%actor

        print "\t</nodes>"

        print "\t<edges>"
            
        for edge in self.edgeList:
            print "\t\t<edge directed=\"false\" from=\"%i\" to=\"%i\"/>"%(int(edge[0]),int(edge[1]))

        print "\t</edges>"

        print "</network>"


if __name__=='__main__':
    data = ActorData(int(sys.argv[1]))
    data.toXML()
