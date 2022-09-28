CFLAGS = -Wall -g
LDLIBS = -lm
OBJS = pgmnega.o pgmrotacao.o pgmlimiar.o pgmmedia.o pgmmediana.o pgmlbp.o

all: pgmnega pgmlimiar pgmmedia pgmmediana pgmlbp pgmrotacao

pgmnega: nega.c pgmfilters.c pgmfilters.h pgmimage.c pgmimage.h
	gcc $(CFLAGS) nega.c pgmfilters.c pgmimage.c $(LDLIBS) -o pgmnega

pgmrotacao: rotacao.c pgmfilters.c pgmfilters.h pgmimage.c pgmimage.h
	gcc $(CFLAGS) rotacao.c pgmfilters.c pgmimage.c $(LDLIBS) -o pgmrotacao

pgmlimiar: limiar.c pgmfilters.c pgmfilters.h pgmimage.c pgmimage.h
	gcc $(CFLAGS) limiar.c pgmfilters.c pgmimage.c $(LDLIBS) -o pgmlimiar

pgmmediana: mediana.c pgmfilters.c pgmfilters.h pgmimage.c pgmimage.h
	gcc $(CFLAGS) mediana.c pgmfilters.c pgmimage.c $(LDLIBS) -o pgmmediana

pgmmedia: media.c pgmfilters.c pgmfilters.h pgmimage.c pgmimage.h
	gcc $(CFLAGS) media.c pgmfilters.c pgmimage.c $(LDLIBS) -o pgmmedia

pgmlbp: lbp.c pgmfilters.c pgmfilters.h pgmimage.c pgmimage.h
	gcc $(CFLAGS) lbp.c pgmfilters.c pgmimage.c $(LDLIBS) -o pgmlbp

clean:
	-rm -f $(OBJS) *~

purge: clean
	-rm -f pgmnega pgmlimiar pgmmedia pgmmediana pgmlbp pgmrotacao