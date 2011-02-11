import pyfreetype

print "FreeType version is:"
print pyfreetype.version()

font = pyfreetype.open_font("c:\\Windows\\Fonts\\calibri.ttf")

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

