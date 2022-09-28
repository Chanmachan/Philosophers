NAME = philo

SRCS_DIR = ./srcs

OBJS_DIR = ./objs

SRCS_FILE = main.c

SRCS = $(addprefix $(SRCS_DIR)/,$(SRCS_FILE))

INCLUDES = ../includes/philo.h

OBJS = $(patsubst $(SRCS_DIR)/%, $(OBJS_DIR)/%, $(SRCS:.c=.o))

CFLAGS = -Wall -Wextra -Werror
#-MP -MMD

all: $(NAME)

$(NAME) : $(OBJS)
		$(CC) $(CFLAGS) $(OBJS)-o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
		mkdir -p $(OBJS_DIR)
		$(CC) $(CFLAGS) -c $< -o $@

clean:
		$(RM) -r $(OBJS_DIR)

fclean: clean
		$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re