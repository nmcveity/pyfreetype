import pyfreetype
import PIL
import PIL.Image

def generate_page(fontfile, str="The quick brown fox jumped over the lazy dog", sizes=[72,60,48,44,36,32,30,28,24,22,21,20,18,16,14,12,11,10,9,8]):
	font = pyfreetype.open_font(fontfile)
	
	image = PIL.Image.new("L", (1024,1024))
	
	pen_x = 0
	pen_y = 0
	
	for size in sizes:
		font.set_char_size(size, size, 72, 72)
		
		# move to the baseline
		pen_x = 0
		pen_y += int(font.size.height)
		
		for ch in str:
			bitmap = font.get_char_bitmap(ord(ch))
			metrics = font.get_char_metrics(ord(ch))
			
			ch_bitmap = PIL.Image.new("L", (bitmap.width, bitmap.height))
			ch_bitmap.putdata(bitmap.pixels)
			
			if pen_x + bitmap.left + bitmap.width > 1024:
				pen_x = 0
				pen_y += int(font.size.height)
	
			image.paste(ch_bitmap, (pen_x+bitmap.left, pen_y-bitmap.top), ch_bitmap)
			
			pen_x += int(metrics.advanceX)
			pen_y += int(metrics.advanceY)
			
	return image

#img = generate_page("examples/ubuntu/Ubuntu-I.ttf")
#img.save("examples/Preview-Ubuntu-Italic.png")

if __name__ == "__main__":
	from optparse import OptionParser
	
	parser = OptionParser()
	parser.add_option("-f", "--font", dest="font", help="The filename of the font to use", action="store", default="examples/ubuntu/Ubuntu-I.ttf")
	parser.add_option("-o", "--output", dest="output", help="The output file name", action="store", default="examples/Preview-Ubuntu-Italic.png")

	(options, args) = parser.parse_args()
	
	img = generate_page(options.font)
	img.save(options.output)
	
	print "%s written." % options.output
