PROG=proj

SRCS = proj.c video_gr.c keyboard_standalone.c mouse.c timer3.c bitmap.c util.c highscores.c

CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
