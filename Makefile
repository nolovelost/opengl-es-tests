INCDIR=-I./Common
LIBS=-lGLESv2 -lEGL -lm -lX11

COMMONSRC=./Common/esShader.c    \
          ./Common/esTransform.c \
          ./Common/esShapes.c    \
          ./Common/esUtil.c
COMMONHRD=esUtil.h

CH02SRC=./src/ogles.h		\
	./src/ogles.cpp		\
	./src/main.cpp

default: all

all: ./build/run

clean:
	find . -name "run" | xargs rm -f

./build/run: ${COMMONSRC} ${COMMONHDR} ${CH02SRC}
	g++ ${COMMONSRC} ${CH02SRC} -o $@ ${INCDIR} ${LIBS}
