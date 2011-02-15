# For MSVC users, change these version numbers to match your local version
# of FreeType 2 binaries.

FT2_MAJOR = 2
FT2_MINOR = 3
FT2_PATCH = 12

from distutils.core import setup, Extension

import os
import os.path
import distutils
import distutils.ccompiler

compiler = distutils.ccompiler.new_compiler()

sources = [
	os.path.join("source", "pyfreetype.c"),
	os.path.join("source", "pyfreetype_bitmapdata.c"),
	os.path.join("source", "pyfreetype_charmapiter.c"),
	os.path.join("source", "pyfreetype_font.c"),
	os.path.join("source", "pyfreetype_glyphmetrics.c"),
	os.path.join("source", "pyfreetype_helpers.c"),
	os.path.join("source", "pyfreetype_sizemetrics.c"),
]

if distutils.ccompiler.get_default_compiler() == "msvc":
	libraries = [
		"freetype%d%d%d" % (FT2_MAJOR, FT2_MINOR, FT2_PATCH)
	]
else:
	libraries = [
		"freetype"
	]

cppdefines = [
	("NDEBUG", None),
]

setup(
	name="pyfreetype",
	version="0.1",
	description="Python bindings for FreeType 2",
	long_description="Python bindings for FreeType 2",
	author="Nick McVeity",
	author_email="nmcveity@gmail.com",
	url="https://github.com/nmcveity/pyfreetype",
	platforms=["any"],
	classifiers=[
		"Development Status :: 3 - Alpha",
		"License :: OSI Approved :: BSD License",
		"Topic :: Text Processing :: Fonts",
	],
	license="License :: OSI Approved :: BSD License",
	ext_modules=[
		Extension('pyfreetype', sources, 
			libraries=libraries,
			define_macros=cppdefines
		)
	]
)

