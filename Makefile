ifdef WIN
  export CC=~/mxe/usr/bin/i686-pc-mingw32-gcc
  SDLCONFIG=~/mxe/usr/i686-pc-mingw32/bin/sdl-config
  PREFIX=~/mxe/usr/i686-pc-mingw32/
  CFLAGS=`$(SDLCONFIG) --cflags` -I. -I$(PREFIX)/include -O3 -DRELEASE
  LDFLAGS=-static
  LIBS=-lSDL_ttf -lfreetype -lSDL_gfx -lSDL_image -ljpeg -lpng -lSDL_mixer -lvorbisfile -lvorbis -logg -lmodplug -lsmpeg -lbz2 -lz -lstdc++ `$(SDLCONFIG) --libs` -mconsole
  # LIBS=-lSDL_ttf -lfreetype -lSDL_gfx -lSDL_image -ljpeg -lpng -lSDL_mixer -lvorbisfile -lvorbis -logg -lmikmod -lmodplug -lsmpeg -lbz2 -lz -lstdc++ `$(SDLCONFIG) --libs` -mconsole
  OUTPUT=nvz.exe
else
  CC=gcc
  LFDLAGS=-static 
  CFLAGS=`sdl-config --cflags` -I. -ggdb #-pg
  OUTPUT=nvz
ifdef MAC
  CFLAGS += -IHardHeaded.app/Contents/Frameworks/SDL.framework/Headers -force_cpusubtype_ALL -mmacosx-version-min=10.6
  LIBS=-IHardHeaded.app/Contents/Frameworks/SDL.framework/Headers /opt/local/lib/libSDL_ttf.a /opt/local/lib/libfreetype.a /opt/local/lib/libSDL_gfx.a /opt/local/lib/libSDL_image.a /opt/local/lib/libjpeg.a /opt/local/lib/libpng.a /opt/local/lib/libSDL_mixer.a /opt/local/lib/libvorbisfile.a /opt/local/lib/libvorbis.a /opt/local/lib/libogg.a /opt/local/lib/libmikmod.a /opt/local/lib/libflac.a /opt/local/lib/libsmpeg.a /opt/local/lib/libbz2.a /opt/local/lib/libz.a -lstdc++ SDLmain.m -FHardHeaded.app/Contents/Frameworks -framework SDL -framework Cocoa -Wl,-framework,ApplicationServices -Wl,-framework,Carbon -Wl,-framework,AudioToolbox -Wl,-framework,AudioUnit -Wl,-framework,IOKit
else # linux
  LIBS=-L/opt/local/lib -lSDL_image -lSDL_ttf -lSDL_mixer -lSDL_gfx -lSDL_ttf `sdl-config --libs` -lm # -pg
endif
endif

OBJS=main.o sprite.o body.o ninja.o zombie.o missile.o map.o

INCS=\
data/icon.h \
data/ninja.h \
data/zombie.h \
data/missile1.h \
data/missile2.h \

.PHONY: all clean depend mac win

all: $(INCS) $(OUTPUT)

clean:
	rm -fv nvz nvz.exe $(OBJS) .depend gmon.out NinjaVsZombies.app/Contents/MacOS/NinjaVsZombies
	make -C data clean

$(INCS):
	make -C data

deps: .depend

.depend: $(patsubst %.o,%.c,$(OBJS)) | $(INCS)
	$(CC) $(CFLAGS) -MM $^ > $@

ifneq ($(MAKECMDGOALS),clean)
-include .depend
endif

$(OUTPUT): $(OBJS) | $(INCS)
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

win:
	echo aa $(TARGET) $(CC)
	make WIN=1
	cd ../ && zip -r9 ninjavszombies/ninjavszombies-windows.zip ninjavszombies/nvz.exe

# http://www.libsdl.org/download-1.2.php#SDL-1.2.15-OSX10.4.dmg 

mac:
	make MAC=1
	mkdir -p NinjaVsZombies.app/Contents/MacOS/
	cp -fv $(OUTPUT) NinjaVsZombies.app/Contents/MacOS/NinjaVsZombies
	# mkdir -p NinjaVsZombies.app/Contents/Frameworks/
	# cp -Rv /Library/Frameworks/SDL.framework NinjaVsZombies.app/Contents/Frameworks/
	install_name_tool -change @rpath/SDL.framework/Versions/A/SDL @executable_path/../Frameworks/SDL.framework/Versions/A/SDL NinjaVsZombies.app/Contents/MacOS/NinjaVsZombies
	cd ../ && zip -r9 ninjavszombies/ninjavszombies-macosx.zip ninjavszombies/NinjaVsZombies.app

