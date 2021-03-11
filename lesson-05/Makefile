CPPFLAGS+=	-O3 -std=c++20
CPPFLAGS+=	-Wall -Wextra -Werror
CPPFLAGS+=	$(shell pkg-config --cflags sdl2)
LDLIBS	+=	$(shell pkg-config --libs   sdl2)
LDLIBS	+=	-lstdc++

SRCS	=	$(shell echo *.cpp)
OBJS	=	$(subst .cpp,.o,$(SRCS))
EXES	=	$(subst .cpp,,$(SRCS))

$(EXES):	$(OBJS)

clean:
	$(RM) $(EXES)
	$(RM) $(OBJS)
