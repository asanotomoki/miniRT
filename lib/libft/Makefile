NAME	=	libft.a
COMPILER  = cc
CFLAGS    = -Wall -Wextra -Werror
INCLUDE   = -I ./includes
SRCDIR    = ./
AR = ar
ARFLAFS = -rcs
RM = rm -rf
SOURCES	= ft_atoi.c \
ft_atof.c \
ft_strtol.c \
ft_bzero.c \
ft_calloc.c \
ft_isalnum.c \
ft_isalpha.c \
ft_isascii.c \
ft_isdigit.c \
ft_isprint.c \
ft_isspace.c \
ft_itoa.c \
ft_memchr.c \
ft_memcmp.c \
ft_memcpy.c \
ft_memmove.c \
ft_memset.c \
ft_putchar_fd.c \
ft_putendl_fd.c \
ft_putnbr_fd.c \
ft_putstr_fd.c \
ft_split.c \
ft_split_space.c \
ft_strchr.c \
ft_strdup.c \
ft_striteri.c \
ft_strjoin.c \
ft_strlcat.c \
ft_strlcpy.c \
ft_strlen.c \
ft_strmapi.c \
ft_strncmp.c \
ft_strnstr.c \
ft_strrchr.c \
ft_strtrim.c \
ft_substr.c \
ft_tolower.c \
ft_toupper.c \
ft_lstnew.c \
ft_lstadd_front.c \
ft_lstsize.c \
ft_lstlast.c \
ft_lstadd_back.c \
ft_lstdelone.c \
ft_lstclear.c \
ft_lstiter.c \
ft_lstmap.c \
ft_get_digit.c \
gnl/get_next_line.c \
gnl/get_next_line_utils.c

OBJDIR    = ./obj
OBJECTS   = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.c=.o)))
DEPENDS   = $(OBJECTS:.o=.d)

$(NAME): $(OBJECTS) 
	@$(AR) $(ARFLAFS) $(NAME) $(OBJECTS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@-mkdir -p $(OBJDIR)
	@$(COMPILER) $(CFLAGS) $(INCLUDE) -o $@ -c $<

all: $(NAME)

clean:
	@$(RM) $(OBJECTS)
	@$(RM) $(OBJDIR)


fclean:	clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all bonus fclean clean re
