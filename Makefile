CC = cc
RM = rm -f

CFLAGS = -Wall -Wextra -Werror

all: cub3d

cub3d: cub3d.o
	$(CC) $(CFLAGS) -o cub3d cub3d.o

cub3d.o: cub3d.c
	$(CC) $(CFLAGS) -c -o cub3d.o cub3d.c

clean:
	$(RM) cub3d.o

fclean: clean
	$(RM) cub3d

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re
