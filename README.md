pyfreetype - A freetype2 binding for Python
===========================================

[FreeType](http://freetype.org/)

[Python](http://www.python.org/)

Introduction
------------

This module provides a wrapper around the excellent FreeType2 font manipulation
library to allow the user to extract glyph bitmap and metric data for further
processing.

To use this library you should be familiar with FreeType and concepts relating 
to font rendering.  The best place for this information is the FreeType 
website:

http://freetype.sourceforge.net/freetype2/documentation.html

In particular, check out the tutorial, the FAQ and the Glyph Convensions guide.

Installation
------------

Coming ...

Examples
--------

Some example scripts are provided in the examples directory:

bitmap_font_page.py: Generates a texture page with all characters for a font 
and size packed onto it AND a JSON data file with font metrics and kerning 
information, as well as the location of each glyph in the bitmap.  This is
useful in multimedia applications, such as games, where text is draw using
hardware acceleration (a quad per character).

**compare.py**: Generates an image file containing an example string for a list
of fonts.  

**compare_all.py**: Generates an image file containing an example string for 
all fonts in a directory.

**demo_page.py**: Generates an image containing a sample of text rendered in a
specific font at various sizes.

API
---

Import the module as:

    import pyfreetype

The module has two methods:

	font = pyfreetype.open_font(filename)
	major, minor, release = pyfreetype.ft2_version()

**pyfreetype.ft2_version()** - Returns a three value tuple with the major, minor
and release number of the FreeType library that pyfreetype is linked against.

**pyfreetype.open_font(name)** - Loads the font 'name' (should be a file path to
a supported font file type).  Returns a font object which you interact with
for all other operations.

See [here](http://freetype.org/freetype2/docs/ft2faq.html#general-what) for a list of
file formats supported by FreeType.

### Font Objects

A font object supports the following methods:

	font.set_pixel_size(32, 32)
	font.set_char_size(48, 48, 72, 72)
	bitmap = font.get_char_bitmap(charcode)
	metrics = font.get_char_metrics(charcode)
	x, y = font.get_kerning(left_charcode, right_charcode)
	
	num_charmaps
	family_name			# something like 'Arial'
	style_name			# something like 'Regular'

These members test the font flags as documented [here](http://freetype.sourceforge.net/freetype2/docs/reference/ft2-base_interface.html#FT_FACE_FLAG_XXX)

	is_flag_scalable
	is_flag_fixed_sizes
	is_flag_fixed_width
	is_flag_sfnt
	is_flag_horizontal
	is_flag_vertical
	is_flag_kerning
	is_flag_fast_glyphs
	is_flag_multiple_masters
	is_flag_names
	is_flag_external_stream
	is_flag_hinter
	is_flag_cid_keyed
	is_flag_tricky

These members test the style flags as documented [here](http://freetype.sourceforge.net/freetype2/docs/reference/ft2-base_interface.html#FT_STYLE_FLAG_XXX):

	is_bold
	is_italic

These members expose the members of the same name in FT_FaceRec, documented [here](http://freetype.sourceforge.net/freetype2/docs/reference/ft2-base_interface.html#FT_FaceRec):

	units_per_EM
	ascender
	descender
	height
	max_advance_width
	max_advance_height
	underline_position
	underline_thickness
	bbox

This member is only set after a size has been selected and contains metrics 
information scaled to the size.  Documentation for the size object is below.

	size

**font.set_pixel_size(width, height)** - Before performing many of the operations
on a font you must first select the size.  This function allows you to set 
the size in pixels.  

**font.set_char_size(width, height, horz_dpi, vert_dpi)** - Before performing many
of the operations on a font you must first select the size.  This function 
allows you to set the size in points, which FreeType converts to pixels using
the DPI of the output device.

**font.get_char_bitmap(charcode)** - Returns a bitmap object containing a glyph
rendered at the current size.  The parameter 'charcode' is the CODE POINT for
the character in the [Universal Charactet Set](http://en.wikipedia.org/wiki/Universal_Character_Set).

You would use this method in python like such:

	bitmap = font.get_char_bitmap(ord('a'))

The fields in the returned bitmap object are described below.

**font.get_char_metrics(charcode)** - Returns a metrics object containing the
metrics for a glyph rendered at the current font size.  As with get_char_bitmap
the parameter must be the CODE POINT for the character.

The fields in the returned metrics object are described below.

**font.get_kerning(left, right)** - Returns the kerning vector between two 
characters at the current font size.

### Bitmap Objects

	bitmap.width 		# the width of the bitmap
	bitmap.height		# the height of the bitmap
	bitmap.left			# empty space in pixels to the left of the bitmap
	bitmap.top			# empty space in pixels to the top of the bitmap
	bitmap.pixels		# a bytearray with 1 byte per pixel

	bitmap.pixels[0]	# first pixel
	bitmap.pixels[1]	# pixel to the right of the first pixel

To save a bitmap, use PIL, like this:

	from PIL import Image
	
	image = Image.new("L", (bitmap.width, bitmap.height))
	image.putdata(bitmap.pixels)
	image.save("output.png")

Note that the bitmap is cropped to fit the glyph exactly, so when drawing the 
bitmap you need to offset it using 'left' and 'top' (this is covered in the
FreeType tutorial).

### Metrics Objects

	metrics.width
	metrics.height
	metrics.horiBearingX
	metrics.horiBearingY
	metrics.horiAdvance
	metrics.vertBearingX
	metrics.vertBearingY
	metrics.vertAdvance
	metrics.linearHoriAdvance
	metrics.linearVertAdvance

These all correspond to the members in the FT_Glyph_Metrics struct documented
[here](http://freetype.sourceforge.net/freetype2/docs/reference/ft2-base_interface.html#FT_Glyph_Metrics)

	metrics.advanceX
	metrics.advanceY

These two values correspond to the 'advance' field in the FT_GlyphSlotRec 
struct documented [here](http://freetype.sourceforge.net/freetype2/docs/reference/ft2-base_interface.html#FT_GlyphSlotRec)

### Size Objects

These members all correspond to the members in the FT_Size_Metrics structure,
documented [here](http://freetype.sourceforge.net/freetype2/docs/reference/ft2-base_interface.html#FT_Size_Metrics)
	
	font.size.x_ppem
	font.size.y_ppem
	font.size.x_scale
	font.size.y_scale
	font.size.ascender
	font.size.descender
	font.size.height
	font.size.max_advance
