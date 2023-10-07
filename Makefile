.SILENT:

NAME    = philo

INCS	= ./includes
SRCS_DIR= ./srcs
OBJS_DIR= ./obj

SRCF    = philosophers check_error utils		

OBJS    = ${SRCS:.c=.o}


SRCS= $(addprefix ${SRCS_DIR}/, $(addsuffix .c, ${SRCF}))
OBJS= $(addprefix ${OBJS_DIR}/, $(addsuffix .o, ${SRCF}))


CFLAGS	= #-Wall -Wextra -Werror
CC = cc
RM = rm -rf

all: ${NAME}
	printf "Compilation \033[0;32m [OK]\n\033[0m"

${NAME}:	${OBJS}
	${CC}	${CFLAGS} -o $@ $^ -Iincludes/

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.c | ${OBJS_DIR}
	${CC} ${CFLAGS} -I${INCS} -c $< -o $@

${OBJS_DIR}:
	mkdir -p ${OBJS_DIR}

clean:
	${RM} ${OBJS_DIR}
	printf "clean\033[0;32m  [OK]\n\033[0m"

fclean:	clean
	${RM}	${NAME}
	printf "fclean\033[0;32m [OK]\n\033[0m"

re: fclean all

author:
		@printf "rboia-pe\n"

.PHONY: clean fclean norm all re author