SRCS	= badapple.c

OBJS	= ${SRCS:.c=.o}

NAME	= badapple

CC		= cc

CFLAGS	= -Wall -Wextra -Werror -O2

RM		= rm -f

all:	${NAME}

.c.o:
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}:	${OBJS}
	${CC} ${OBJS} -o ${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re:	fclean all

.PHONY:	all clean fclean re
