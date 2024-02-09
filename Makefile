.SILENT:

NAME = philo

INCLUDE = philo.h

INCS = ./includes
SRCS_DIR = ./srcs
OBJS_DIR = ./objs

SRC = main philo utils watcher

SRCS = $(addprefix $(SRCS_DIR)/, $(addsuffix .c, $(SRC)))
OBJS = $(addprefix $(OBJS_DIR)/, $(addsuffix .o, $(SRC)))

CFLAGS = -Wall -Wextra -Werror -pthread -fsanitize=thread

BLUE=\033[0;34m

ORANG=\033[0;33m

RESET=\033[0m

all: $(NAME)
	@echo "\n"
	@echo "             $(ORANG)****************************"
	@echo "             $(BLUE)*           ^-^            *"
	@echo "             $(ORANG)*      PHILO IS READY      *"
	@echo "             $(BLUE)*           ^-^            *"
	@echo "             $(ORANG)****************************"
	@echo "\n"
%.o : %.c
		$(CC) -c $< -o $@

CC = cc

$(NAME): $(OBJS)
				@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

${OBJS_DIR}/%.o: ${SRCS_DIR}/%.c | ${OBJS_DIR}
	${CC} ${CFLAGS} -I${INCS} -c $< -o $@

${OBJS_DIR}:
	mkdir -p ${OBJS_DIR}

clean:
	@rm -rf $(OBJS) $(OBJS_DIR)

fclean: clean
	@rm -rf $(NAME)

re: clean fclean all

author:
	@echo Intra : rboia-pe;
	@echo Github: Nebbur;
