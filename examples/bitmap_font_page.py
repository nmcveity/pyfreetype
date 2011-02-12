import pyfreetype
import PIL
import PIL.Image
import math

def build(fontfile, size, charset, max_texture_width=256, spacing=4):
	font = pyfreetype.open_font(fontfile)
	font.set_pixel_size(size, size)
	
	bitmaps = {}
	
	max_width = 0
	max_height = 0
	
	for ch in charset:
		bitmap = font.get_char_bitmap(unicode(ch))
		bitmaps[ch] = bitmap
		
		max_width = max(max_width, bitmap.width)
		max_height = max(max_height, bitmap.height)
	
	# adjust for spacing
	max_width += spacing * 2
	max_height += spacing * 2
	
	tex_width = min(max_width * len(charset), max_texture_width)
	tex_height = int(len(charset) / math.floor(tex_width / float(max_width)) * max_height)
	
	print tex_width, tex_height
	print max_width, max_height
	
	tex = PIL.Image.new("L", (tex_width, tex_height))
	pen_x = 0
	pen_y = 0
	
	for ch in charset:
		bitmap = font.get_char_bitmap(unicode(ch))
		metrics = font.get_char_metrics(unicode(ch))
		
		image = PIL.Image.new("L", (bitmap.width, bitmap.height))
		image.putdata(bitmap.pixels)
		
		print pen_x, pen_y, bitmap.width, bitmap.height
		
		tex.paste(image, (pen_x+spacing, pen_y+spacing))
		
		del image
		
		pen_x += max_width
		
		if pen_x + max_width >= tex.size[0]:
			pen_x = 0
			pen_y += max_height
	
	return tex

if __name__ == "__main__":
	from optparse import OptionParser
	
	parser = OptionParser()
	parser.add_option("-f", "--font", dest="font", help="The filename of the font to use", action="store")
	parser.add_option("-s", "--pixelsize", dest="size", help="The size of the character in pixels", type="int", action="store")
	parser.add_option("-o", "--output", dest="output", help="The base name for output files.  Will generate FILE.EXT and FILE.xml", action="store")
	parser.add_option("-e", "--extension", dest="ext", help="The format for the output image", default="png", action="store")

	(options, args) = parser.parse_args()
	
	if options.font is None:
		parser.error("You need to specify a --font option")

	if options.size is None:
		parser.error("You need to specify a --size option")

	if options.output is None:
		parser.error("You need to specify an --output option")
	
	default_charset = "ABCDEFGHIJKLMNOPQRSTUXYZabcdefghijklmnopqrstuvwxyz1234567890!$%.,:?'\"()/~"
	
	tex = build(options.font, options.size, default_charset)
	tex.save("%s.%s" % (options.output, options.ext))

	

