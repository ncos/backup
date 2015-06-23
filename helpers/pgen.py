###Main###
from optparse import OptionParser
from optparse import OptionGroup
import sys
import os
import time
import datetime
import re


known_extensions = [".mp3"]


def Main():
    current_path = os.path.abspath(os.path.dirname(__file__))
    print "Processing files in " + current_path
    onlyfiles = [ f for f in os.listdir(current_path) if os.path.isfile(os.path.join(current_path,f)) ]
    valid_files = []    
    print "Found:"
    for f in onlyfiles:
        for ext in known_extensions:
            if ext in f:
                print f    
                valid_files.append(f)
    if len(valid_files) == 0:
        print "... No valid files. Terminating"
        sys.exit(0)
    print "Found", len(valid_files), "valid files"

    filestr = "<?wpl version=\"1.0\"?>\n"+\
              "<smil>\n"+\
              "  <head>\n"+\
              "    <meta name=\"Generator\" content=\"Microsoft Windows Media Player -- 12.0.7601.18741\"/>\n"+\
              "    <meta name=\"ItemCount\" content=\""+str(len(valid_files))+"\"/>\n"+\
              "    <title>"+options.playlist_file_name.split(".")[0]+"</title>\n"+\
              "  </head>\n"+\
              "  <body>\n"+\
              "    <seq>\n"
    for f in valid_files:
        filestr +="      <media src=\"" + f + "\"/>\n"
    filestr +="    </seq>\n"+\
              "  </body>\n"+\
              "</smil>\n"

    f = open(options.playlist_file_name, 'w')
    f.write(filestr)
    f.close()
    print "Written to " + os.path.join(current_path, options.playlist_file_name)

if __name__ == '__main__':
    class MyParser(OptionParser):
        def format_epilog(self, formatter):
            return self.epilog
    description =  ("This python script will generate .wpl playlist for all music files in current directory\n"+
                    "Known extensions are: " + str(known_extensions) + "\n")
    parser = MyParser(usage="Usage: pgen.py [options]", epilog=description)
    parser.add_option('-n', '--name', dest='playlist_file_name',
        help='Specify the name of the generated playlist (default = Playlist.wpl)', default="Playlist.wpl") 
    (options, args) = parser.parse_args()
    Main()
