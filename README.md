# Util #
----------------------------------------

## linux ##


DEFINES = \_PLATFORM\_UNIX\_

LIBS = -lpthread


## windows ##

DEFINES = \_PLATFORM\_WINDOWS\_

LIBS = -lws2_32 -lShlwapi
