#!/usr/bin/python

import os
import sys


class NetworkData:
    def __init__(self, filename):

        file = open(filename)

        self.nodeList = []
        self.actorList = []
        self.edgeList  = []

        for line in file.readlines():

            node1 = int(line.split(' ')[1])
            node2 = int(line.split(' ')[2])
                        
            if node1 not in self.nodeList:
                self.nodeList.append(node1)

            if node2 not in self.nodeList:
                self.nodeList.append(node2)
            
            edge = []
            edge.append(node1)
            edge.append(node2)
                
            self.edgeList.append(edge)

    def toXML(self):

        print "<network>"

        print "\t<nodes>"

        self.nodeList.sort()
        for node in self.nodeList:
            print "\t\t<node id=\"%i\"/>"%node

        print "\t</nodes>"

        print "\t<edges>"
            
        for edge in self.edgeList:
            print "\t\t<edge directed=\"true\" from=\"%i\" to=\"%i\"/>"%(edge[0],edge[1])

        print "\t</edges>"

        print "</network>"


if __name__=='__main__':
    data = NetworkData(sys.argv[1])
    data.toXML()
