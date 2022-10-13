NAME = philo

SRCS_DIR = ./srcs

OBJS_DIR = ./objs

SRCS_FILE = main.c utils.c init.c valid_args.c attitude.c monitor.c

SRCS = $(addprefix $(SRCS_DIR)/,$(SRCS_FILE))

INCLUDES = ../includes/philo.h

OBJS = $(patsubst $(SRCS_DIR)/%, $(OBJS_DIR)/%, $(SRCS:.c=.o))

CFLAGS = -Wall -Wextra -Werror -pthread -fsanitize=thread -g
#CFLAGS = -Wall -Wextra -Werror -pthread -fsanitize=thread -g
#-MP -MMD -pthread -fsanitize=thread

all: $(NAME)

$(NAME) : $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
		mkdir -p $(OBJS_DIR)
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		$(RM) -r $(OBJS_DIR)

fclean: clean
		$(RM) $(NAME)

re: fclean all

norm:
		norminette $(SRCS_DIR)

.PHONY: all clean fclean re