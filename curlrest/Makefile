P=curlrest
CC=gcc
CFLAGS=`pkg-config --cflags json-c glib-2.0 readline libuv libcurl` -std=gnu11 -pedantic -Wall -O0 -g
OBJECTS=
LDLIBS=`pkg-config --libs   json-c glib-2.0 readline libuv libcurl`
$(P): $(OBJECTS)

