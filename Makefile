#####################################################################
#       (c) Copyright 2014, Hogeschool voor de Kunsten Utrecht
#                       Hilversum, the Netherlands
#####################################################################
#
# Description   : Makefile for osc2midi and midi2osc
#
# Author        : Marc_G
# E-mail        : marcg@dinkum.nl
#
####################################################################/

CC = g++ -g
CFLAGS = -Wall
LDFLAGS=  -lpthread -lm -llo -L/usr/local/lib -lportmidi

CLIENT_OBJ= midi2osc.o osc.o midi_io.o
SERVER_OBJ= osc2midi.o osc.o midi_io.o

all: midi2osc osc2midi

midi2osc: $(CLIENT_OBJ)
	$(CC) -o $@ $(CFLAGS) $(CLIENT_OBJ) $(LDFLAGS)

osc2midi: $(SERVER_OBJ)
	$(CC) -o $@ $(CFLAGS) $(SERVER_OBJ) $(LDFLAGS)


.cpp.o:
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o
	rm -f `find . -perm +111 -type f`

