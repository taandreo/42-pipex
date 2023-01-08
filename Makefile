.PHONY: all clean fclean re log bonus libft

CC = cc
NAME = pipex
LIBFT = ./lib/libft.a
LIBFT_DIR = ./libft
CFLAGS = -Wall -Wextra -Werror
UNAME := $(shell uname)
LIBS := -lft

ifeq ($(UNAME), Darwin)
# CFLAGS = -Wall -Wextra -Werror -arch x86_64
	CFLAGS = -Werror -arch x86_64
	LIBS = -lmlx -lft -framework OpenGL -framework AppKit
endif

SRCS = pipex.c utils.c path.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I libft/include -I include -c $< -o $@ 

$(NAME): $(OBJS) | libft
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS) -L $(LIBFT_DIR)

libft:
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME) $(LIBFT)
	make fclean -C $(LIBFT_DIR)

re: fclean all

git_libft:
	git clone https://github.com/taandreo/42-libft.git libft