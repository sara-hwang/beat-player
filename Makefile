# Makefile for building embedded application.
# by Brian Fraser

# Edit this file to compile extra C files into their own programs.
TARGET= beatbox

SOURCES= wave_player.c audioMixer_template.c joystick.c files.c main.c accelerometer.c udp.c

PUBDIR = $(HOME)/cmpt433/public/myApps
OUTDIR = $(PUBDIR)
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

# Asound process:
# get alibsound2 lib on target:
# 	# apt-get install libasound2
# Copy target's /usr/lib/arm-linux-gnueabihf/libasound.so.2.0.0 
#      to host  ~/public/asound_lib_BBB/libasound.so
# Copy to just base library:

LFLAGS = -L$(HOME)/cmpt433/public/asound_lib_BBB

# -pg for supporting gprof profiling.
#CFLAGS += -pg

all: wav node 
	$(CC_C) $(CFLAGS) $(SOURCES) -o $(OUTDIR)/$(TARGET)  $(LFLAGS) -lpthread -lasound

# Copy wave files to the shared folder
wav:
	mkdir -p $(PUBDIR)/beatbox-wav-files/
	cp -R beatbox-wave-files/* $(PUBDIR)/beatbox-wav-files/ 

node:
	mkdir -p $(PUBDIR)/beatbox-server-copy/
	cp -R as3-server/* $(PUBDIR)/beatbox-server-copy/
	cd $(PUBDIR)/beatbox-server-copy/ && npm install

clean:
	rm -f $(OUTDIR)/$(TARGET)
	rm -f -r $(OUTDIR)/beatbox-server-copy  
	rm -f -r $(OUTDIR)/beatbox-wav-files
