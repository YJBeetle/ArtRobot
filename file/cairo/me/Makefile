APPS    = ibm_cairo_logo

CFLAGS  = -Wall -g
#LDFLAGS = -lm
LDFLAGS =

#CFLAGS  += `pkg-config gtk+-2.0 --cflags`
#LDFLAGS += `pkg-config gtk+-2.0 --libs`
CFLAGS += -I/opt/local/include/cairo
LDFLAGS += -L/opt/local/lib -lcairo

all: $(APPS)

clean:
	rm -f $(APPS)
