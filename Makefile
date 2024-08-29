CC = cc
NAME = cub3D
INCLUDE = cub3D.h
FLAGS = -Wunreachable-code -Ofast
MLX_LIB = libmlx42.a -ldl /Users/$(shell whoami)/.brew/lib/libglfw3.a -pthread -lm  -framework Cocoa -framework IOKit -framework CoreFoundation
HEADERS = -I ./MLX42/include
OBJ_DIR = obj

SRC = cub3D.c raycasting.c \
      parsing/parser.c parsing/parsing_errors.c  parsing/parsing_helper.c\
      get_next_line/get_next_line.c \
      utils/printing_utils.c utils/string_utils.c utils/string_utils2.c utils/ft_atoi.c utils/ll_utils.c utils/math_utils.c utils/draw_utils.c utils/other_utils.c

OBJ = $(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))
LINK_OBJ = $(addprefix $(OBJ_DIR)/,$(notdir $(SRC:.c=.o)))
all: $(NAME)

$(OBJ_DIR)/%.o: %.c $(INCLUDE) | $(OBJ_DIR)
	@$(CC) $(FLAGS) $(HEADERS) -c $< -o $(OBJ_DIR)/$(notdir $@)
	@echo "Compiled $<"

$(NAME): $(OBJ)
	@$(CC) $(LINK_OBJ) -g $(HEADERS) $(MLX_LIB) -o $(NAME)
	@echo "Mandatory executable linked successfully!"

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	@rm -f $(OBJ)
	@echo "Cleaned object files"

fclean: clean
	@rm -f $(NAME)
	@echo "Cleaned executable"

re: fclean all

.PHONY: all clean fclean re

