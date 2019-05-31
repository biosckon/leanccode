P=avro_play
CC=gcc
CFLAGS=`pkg-config --cflags avro-c jansson glib-2.0 readline libuv libcurl` -std=gnu11 -pedantic -Wall -O0 -g
# json-c
OBJECTS=
LDLIBS=`pkg-config --libs   avro-c jansson glib-2.0 readline libuv libcurl`
# json-c
$(P): $(OBJECTS)

