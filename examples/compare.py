import pyfreetype
import PIL
import PIL.Image

def generate_comparison(fontfiles, str, size=28, width=1024):
	image_width = width
	image_height = 1024
	image = PIL.Image.new("L", (image_width, image_height))
	
	pen_x = 0
	pen_y = 0
	
	for file in fontfiles:
		font = pyfreetype.open_font(file)
		font.set_char_size(size, size, 72, 72)
		
		# move to the baseline
		pen_x = 0
		pen_y += int(font.size.height)
		
		for ch in "%s-%s: %s" % (font.family_name, font.style_name, str):
			bitmap = font.get_char_bitmap(ord(ch))
			metrics = font.get_char_metrics(ord(ch))
			
			ch_bitmap = PIL.Image.new("L", (bitmap.width, bitmap.height))
			ch_bitmap.putdata(bitmap.pixels)
			
			if pen_x + bitmap.left + bitmap.width > image_width:
				pen_x = 0
				pen_y += int(font.size.height)
	
			if pen_y-bitmap.top+bitmap.height >= image_height:
				image_height *= 2
				image_large = PIL.Image.new("L", (image_width, image_height))
				image_large.paste(image, (0, 0))
				image = image_large
	
			image.paste(ch_bitmap, (pen_x+bitmap.left, pen_y-bitmap.top), ch_bitmap)
			
			pen_x += int(metrics.advanceX)
			pen_y += int(metrics.advanceY)
		
		del font
	
	return image.crop((0, 0, image_width, pen_y+64))

if __name__ == "__main__":
	from optparse import OptionParser
	
	parser = OptionParser()
	parser.add_option("-f", "--font", dest="font", help="The filename of the font to use", action="append", default=["examples/ubuntu/Ubuntu-R.ttf", "examples/ubuntu/Ubuntu-I.ttf", "examples/ubuntu/Ubuntu-B.ttf"])
	parser.add_option("-o", "--output", dest="output", help="The output file name", action="store", default="examples/compare_output.png")
	parser.add_option("-w", "--width", dest="width", help="The output file width in pixels", action="store", default=1024, type="int")
	parser.add_option("-p", "--phrase", dest="phrase", help="Text to print after the font name in the output image", action="store", default="The quick brown fox jumped over the lazy dog")
	
	(options, args) = parser.parse_args()
	
	img = generate_comparison(options.font, options.phrase, width=options.width)
	img.save(options.output)
	
	print "%s written." % options.output
