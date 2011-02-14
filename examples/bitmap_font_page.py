import pyfreetype
import PIL
import PIL.Image
import math
import json

def build(fontfile, size, charset, max_texture_width=256, spacing=4):
	font = pyfreetype.open_font(fontfile)
	font.set_pixel_size(size, size)
	
	bitmaps = {}
	
	max_width = 0
	max_height = 0
	
	for ch in charset:
		bitmap = font.get_char_bitmap(ord(unicode(ch)))
		bitmaps[ch] = bitmap
		
		max_width = max(max_width, bitmap.width)
		max_height = max(max_height, bitmap.height)
	
	# adjust for spacing
	max_width += spacing * 2
	max_height += spacing * 2
	
	tex_width = min(max_width * len(charset), max_texture_width)
	tex_height = int(len(charset) / math.floor(tex_width / float(max_width)) * max_height)
	
	tex = PIL.Image.new("L", (tex_width, tex_height))
	
	pen_x = 0
	pen_y = 0
	
	metrics_dict = {}
	
	for ch in charset:
		bitmap = font.get_char_bitmap(ord(unicode(ch)))
		metrics = font.get_char_metrics(ord(unicode(ch)))
		
		metrics_dict[ch] = {
			'width':				metrics.width,
			'horiBearingX':			metrics.horiBearingX,
			'horiBearingY':			metrics.horiBearingY,
			'horiAdvance':			metrics.horiAdvance,
			'vertBearingX':			metrics.vertBearingX,
			'vertBearingY':			metrics.vertBearingY,
			'vertAdvance':			metrics.vertAdvance,
			'linearHoriAdvance':	metrics.linearHoriAdvance,
			'linearVertAdvance':	metrics.linearVertAdvance,
			'advanceX':				metrics.advanceX,
			'advanceY':				metrics.advanceY,
			'bitmapLeft':			pen_x+spacing,
			'bitmapTop':			pen_y+spacing,
			'bitmapWidth':			bitmap.width,
			'bitmapHeight':			bitmap.height,
		}
		
		image = PIL.Image.new("L", (bitmap.width, bitmap.height))
		image.putdata(bitmap.pixels)
		
		tex.paste(image, (pen_x+spacing, pen_y+spacing))
		
		del image
		
		pen_x += max_width
		
		if pen_x + max_width >= tex.size[0]:
			pen_x = 0
			pen_y += max_height
	
	kerning_pairs = []
	
	for left in charset:
		for right in charset:
			x, y = font.get_kerning(ord(unicode(left)), ord(unicode(right)))
			
			if x != 0 or y != 0:
				kerning_pairs.append({
					'left': left,
					'right': right,
					'x': x,
					'y': y,
				})
	
	metrics_dict['kerning_pairs'] = kerning_pairs
	
	del font
	
	return tex, json.dumps(metrics_dict, indent=1)

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
	
	default_charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!$%.,:?'\"()/~"
	
	tex, data = build(options.font, options.size, default_charset)
	tex.save("%s.%s" % (options.output, options.ext))
	
	data_out = open("%s.json" % options.output, "wt")
	data_out.write(data)
	data_out.close()

	

