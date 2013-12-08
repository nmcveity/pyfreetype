# For MSVC users, change these version numbers to match your local version
# of FreeType 2 binaries.

FT2_MAJOR = 2
FT2_MINOR = 3
FT2_PATCH = 12

from distutils.core import setup, Extension

import os
import os.path
import subprocess
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

try:
	extra_compile_args = subprocess.check_output(["freetype-config", "--cflags"]).split()
except OSError as e:
	print "!! Unable to run freetype-config to automatically derive CFLAGS."
	print "!! This is normal on windows but on linux may indicate a missing freetype2-dev package."
	print "!! Either install (sudo apt-get install freetype2.dev) or manually configure paths on command line."
else:
	print ":) Found freetype-config for CFLAGS"
	print "     " + " ".join(extra_compile_args)

try:
	extra_link_args = subprocess.check_output(["freetype-config", "--libs"]).split()
except OSError as e:
	print "!! Unable to run freetype-config to automatically derive LIBS."
	print "!! This is normal on windows but on linux may indicate a missing freetype2-dev package."
	print "!! Either install (sudo apt-get install freetype2.dev) or manually configure paths on command line."
else:
	print ":) Found freetype-config for LIBS"
	print "     " + " ".join(extra_link_args)

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
	version="0.2",
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
			extra_compile_args=extra_compile_args,
			extra_link_args=extra_link_args,
			libraries=libraries,
			define_macros=cppdefines
		)
	]
)

