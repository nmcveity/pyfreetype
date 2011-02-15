import os
import os.path

from compare import generate_comparison

from optparse import OptionParser

parser = OptionParser()
parser.add_option("-d", "--directory", dest="path", help="The directory to search for fonts", action="store", default="c:\\windows\\fonts")
parser.add_option("-o", "--output", dest="output", help="The output file name", action="store", default="examples/compare_all_output.png")
parser.add_option("-w", "--width", dest="width", help="The output file width in pixels", action="store", default=1024, type="int")
parser.add_option("-p", "--phrase", dest="phrase", help="Text to print after the font name in the output image", action="store", default="The quick brown fox jumped over the lazy dog")

(options, args) = parser.parse_args()

fonts = []

for file in os.listdir(options.path):
	if os.path.splitext(file)[1] == ".ttf":
		print "Adding: %s" % file
		fonts.append(os.path.join(options.path, file))

img = generate_comparison(fonts, options.phrase, width=options.width)
img.save(options.output)

print "%s written." % options.output
