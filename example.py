import pyfreetype
import PIL
import PIL.Image

print "FreeType version is:"
print pyfreetype.version()


font_name = "calibri"
font = pyfreetype.open_font("c:\\Windows\\Fonts\\%s.ttf" % font_name)

print str(font)
print font.num_charmaps

print "Charmaps: %s" % str(list(font.charmaps()))
print "Family: %s" % font.family_name
print "Style: %s" % font.style_name
print "Flags:"

print "\tis_flag_scalable: " + str(font.is_flag_scalable)
print "\tis_flag_fixed_sizes: " + str(font.is_flag_fixed_sizes)
print "\tis_flag_fixed_width: " + str(font.is_flag_fixed_width)
print "\tis_flag_sfnt: " + str(font.is_flag_sfnt)
print "\tis_flag_horizontal: " + str(font.is_flag_horizontal)
print "\tis_flag_vertical: " + str(font.is_flag_vertical)
print "\tis_flag_kerning: " + str(font.is_flag_kerning)
print "\tis_flag_fast_glyphs: " + str(font.is_flag_fast_glyphs)
print "\tis_flag_multiple_masters: " + str(font.is_flag_multiple_masters)
print "\tis_flag_names: " + str(font.is_flag_names)
print "\tis_flag_external_stream: " + str(font.is_flag_external_stream)
print "\tis_flag_hinter: " + str(font.is_flag_hinter)
print "\tis_flag_cid_keyed: " + str(font.is_flag_cid_keyed)
print "\tis_flag_tricky: " + str(font.is_flag_tricky)

print "Style:"
print "\tis_italic: " + str(font.is_italic)
print "\tis_bold: " + str(font.is_bold)

font_width = 32
font_height = 32
font.set_char_size(font_width, font_height, 300, 300)

print "A: ", font.get_char_bitmap(unicode("A"))
print "B: ", font.get_char_bitmap(unicode("B"))
print "C: ", font.get_char_bitmap(unicode("C"))
print "D: ", font.get_char_bitmap(unicode("D"))
print "E: ", font.get_char_bitmap(unicode("E"))
print "F: ", font.get_char_bitmap(unicode("F"))
print "1: ", font.get_char_bitmap(unicode("1"))
print "2: ", font.get_char_bitmap(unicode("2"))

test_set = unicode("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234567890_!@#$%^&*()") + unichr(169) + unichr(174) + unichr(9824) + unichr(9827) + unichr(9829) + unichr(9830)

for test in test_set:
	width, height, pixels = font.get_char_bitmap(unicode(test))
	
	image = PIL.Image.new("L", (width, height))
	image.putdata(pixels)
	image.save("test/%s/%d_%d-%s.png" % (font_name, font_width, font_height, ord(test)))

for test in test_set:
	metrics = font.get_char_metrics(unicode(test))
	
	print "A: %d,%d\t%d,%d,%d\t%d,%d,%d\t%d,%d\t%d,%d" % (metrics.width, metrics.height, metrics.horiBearingX, metrics.horiBearingY, metrics.horiAdvance, metrics.vertBearingX, metrics.vertBearingY, metrics.vertAdvance, metrics.advanceX, metrics.advanceY, metrics.linearHoriAdvance, metrics.linearVertAdvance)
	
