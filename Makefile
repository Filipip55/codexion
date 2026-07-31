NAME    = codexion
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -pthread -g
INCDIR  = includes
SRCDIR  = srcs
OBJDIR  = objs

SRCS    = main.c parse.c init.c time_utils.c log.c dongle.c \
          heap.c heap_utils.c coder.c monitor.c cleanup.c

OBJS    = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/codexion.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
