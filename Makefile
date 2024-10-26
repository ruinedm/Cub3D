CC = cc
NAME = cub3D
NAME_BONUS = cub3D_bonus
INCLUDE = mandatory/cub3D.h
INCLUDE_BONUS = bonus/cub3D.h
FLAGS = -Wunreachable-code -Ofast -Wall -Wextra -Werror
MLX_LIB = libmlx42.a -ldl /Users/$(shell whoami)/.brew/lib/libglfw3.a -pthread -lm  -framework Cocoa -framework IOKit -framework CoreFoundation

SRC = mandatory/cub3D.c \
      mandatory/parsing/parser.c mandatory/parsing/parsing_helper.c mandatory/parsing/colors.c mandatory/parsing/door_helper.c mandatory/parsing/inner.c mandatory/parsing/get_map.c mandatory/parsing/helper.c \
      mandatory/get_next_line/get_next_line.c \
	  mandatory/raycasting/raycasting.c mandatory/raycasting/ho_inter.c mandatory/raycasting/ve_inter.c \
	  mandatory/hooks/key_hooks.c mandatory/hooks/loop_hook.c \
      mandatory/utils/printing_utils.c mandatory/utils/string_utils.c mandatory/utils/string_utils2.c mandatory/utils/ft_atoi.c mandatory/utils/ll_utils.c mandatory/utils/math_utils.c  mandatory/utils/other_utils.c mandatory/utils/collisions.c mandatory/utils/error_management.c

SRC_BONUS = bonus/cub3D.c \
      bonus/parsing/parser.c  bonus/parsing/parsing_helper.c bonus/parsing/doors.c bonus/parsing/colors.c bonus/parsing/door_helper.c bonus/parsing/inner.c bonus/parsing/get_map.c bonus/parsing/helper.c\
      bonus/get_next_line/get_next_line.c \
	  bonus/raycasting/raycasting.c bonus/raycasting/ho_inter.c bonus/raycasting/ve_inter.c\
	  bonus/doors_minimap_sprite/dooors.c bonus/doors_minimap_sprite/minimap.c bonus/doors_minimap_sprite/sprite.c\
	  bonus/hooks/key_hooks.c bonus/hooks/mouse_hooks.c\
      bonus/utils/printing_utils.c bonus/utils/string_utils.c bonus/utils/string_utils2.c bonus/utils/ft_atoi.c bonus/utils/ll_utils.c bonus/utils/math_utils.c  bonus/utils/other_utils.c bonus/utils/collisions.c bonus/utils/error_management.c

OBJ = $(SRC:.c=.o)
OBJ_BONUS = $(SRC_BONUS:.c=.o)

all: $(NAME)

bonus: $(NAME_BONUS)

mandatory/%.o: mandatory/%.c $(INCLUDE)
	@($(CC) $(FLAGS) -c $< -o $@)
	@echo "Compiled $<"

bonus/%.o: bonus/%.c $(INCLUDE_BONUS)
	@$(CC) $(FLAGS) -c $< -o $@
	@echo "Compiled $<"

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) -g $(OBJ) $(MLX_LIB) -o $(NAME)
	@echo "Mandatory executable linked successfully!"

$(NAME_BONUS): $(OBJ_BONUS)
	@$(CC) $(FLAGS) $(OBJ_BONUS) $(MLX_LIB) -o $(NAME_BONUS)
	@echo "Bonus executable linked successfully!"

clean:
	@rm -f $(OBJ) $(OBJ_BONUS)
	@echo "Cleaned object files"

fclean: clean
	@rm -f $(NAME) $(NAME_BONUS)
	@echo "Cleaned executables"

re: fclean all
