# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tasano <tasano@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/04 15:38:14 by asanotomoki       #+#    #+#              #
#    Updated: 2023/04/21 14:18:02 by tasano           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=	miniRT
OBJ_DIR		:=	./obj
SRC_DIR		:=	./srcs
CC			:=	cc
CFLAGS		:=	-Wall -Werror -Wextra

HEADERS		:=  ./includes

LIBFT_DIR	:=	./lib/libft
LIBFT		:=	$(LIBFT_DIR)/libft.a
HEADERS		+=	$(LIBFT_DIR)/includes

MLX_DIR		:=	./lib/mlx
LIBMLX		:=	$(MLX_DIR)/libmlx.a
LXFLAGS		:=	-framework OpenGL -framework AppKit
HEADERS		+= $(MLX_DIR)

INCLUDES		:=	$(addprefix -I , $(HEADERS))

SOURCES :=	main.c
# create_map
SRC_FILE := create_map.c \
			set_ambient.c set_camera.c  set_light.c \
			set_sphere.c set_plane.c set_cylinder.c set_object_list.c \
			set_vec.c set_color.c
SOURCES += $(addprefix create_map/, $(SRC_FILE))

#draw
SRC_FILE := draw.c \
			get_dir_vec.c \
			get_t_val.c get_t_util_cylinder.c get_t_util_condition.c\
			diffusion.c  get_pixcel_color.c shadow.c specular.c \
			get_min.c \
			determin_normal_vec.c \
			get_vec.c
SOURCES += $(addprefix draw/, $(SRC_FILE))

#util 
SRC_FILE := color_util.c mlx_util.c vec_util.c vec_util2.c free_util.c delete_rt.c
SOURCES += $(addprefix util/, $(SRC_FILE))

OBJECTS	:= $(addprefix $(OBJ_DIR)/, $(SOURCES:.c=.o))
RM := rm -f
#Message
RED		:= \033[1;31m
GREEN	:= \033[1;32m
YELLOW	:= \033[1;33m
DEFAULT	:= \033[0m
NAME_MSG	:=	"$(GREEN) Compile $(NAME)$(DEFAULT)"
CLEAN_MSG	:=	"$(YELLOW) Delete $(OBJ_DIR)$(DEFAULT)"
FCLEAN_MSG	:=	"$(RED) Delete $(NAME)$(DEFAULT)"

.PHONY: all fclean clean re libft

$(NAME):   $(LIBFT) $(LIBMLX) $(OBJECTS)
	@$(CC) $(CFLAGS) -o $(NAME) $(LIBFT) $(LIBMLX) $(LXFLAGS) $(OBJECTS)
	@echo $(NAME_MSG)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(LIBMLX) : 
	@make -C $(MLX_DIR)

all: $(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@echo $(CLEAN_MSG)
	@make clean -C $(LIBFT_DIR)
	@make clean -C $(MLX_DIR)

fclean:clean
	@make fclean -C $(LIBFT_DIR)
	@make fclean -C $(MLX_DIR)
	@$(RM) $(NAME)
	@echo $(FCLEAN_MSG)

re: fclean all

norm:
	norminette -v
	norminette $(SRC_DIR) $(LIBFT_DIR) ./includes 
