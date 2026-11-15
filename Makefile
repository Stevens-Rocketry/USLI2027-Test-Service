# By George K Oliynyk @ 2026

PROG=	tsservice
OBJS=	tsservice.o notify.o
CFLAGS=		-Wall -Werror -Wextra -Wformat=2 -Wjump-misses-init -Wlogical-op -Wpedantic -Wshadow -g
LDFLAGS= 	-lsystemd
VPATH=		src

all: ${PROG}

-include .depend

depend:
	${CC} -M ${CFLAGS} src/*.c > .depend

${PROG}: ${OBJS}
	@echo $@ depends on $?
	${CC} ${OBJS} -o ${PROG} ${LDFLAGS}

clean:
	rm -f ${PROG} ${OBJS}