#!/usr/bin/python
# -*- coding: utf-8 -*-


import urllib2
from lxml import html

response = urllib2.urlopen('file:///home/ncos/Desktop/backup/runsity-2016/list.html')
content = response.read()

tree = html.fromstring(content)

links = []
names = []
strts = []
for tr in tree.xpath("//tr//td//a"):
    print tr.xpath("@href")[0], unicode(tr.xpath("text()")[0]).encode('utf8')
    links.append(tr.xpath("@href")[0])
    names.append(unicode(tr.xpath("text()")[0]).encode('utf8'))

print "\n\nFound " + str(len(links)) + " streets\n\n"


for i, link in enumerate(links):
    print "["+str(i+1) + "/" + str(len(links)) + "] Processing " + names[i] + " (" + link + ")"
    response = urllib2.urlopen(link)
    content = response.read()
    tree = html.fromstring(content)

    info = tree.xpath("//ul[@class='information']//li")
    strts.append("-")
    
    for li in info:
        if ("Номера домов" in unicode(li.xpath("b/text()")[0]).encode('utf8')):
            strts[i] = (li.xpath("span/text()")[0])
            break

print "\n\nOutput:\n"
for i in xrange(len(names)):
    print str(names[i]) + ";\t" + unicode(strts[i]).encode('utf8')

