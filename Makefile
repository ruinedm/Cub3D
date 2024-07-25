CC = cc
NAME = cub3D
INCLUDE = cub3D.h
# Flag = FLAGS = -Wall -Wextra -Werror

OBJ_DIR = obj
PARSING_DIR = parsing
GNL_DIR = get_next_line
UTILS_DIR = utils


PARSING_SRC = parser.c parsing_errors.c
GNL_SRC = get_next_line.c
UTILS_SRC = printing_utils.c string_utils.c string_utils2.c ft_atoi.c ll_utils.c
MAIN_SRC = cub3D.c

PARSING_FILES = $(addprefix $(PARSING_DIR)/,$(PARSING_SRC))
GNL_FILES = $(addprefix $(GNL_DIR)/,$(GNL_SRC))
UTILS_FILES = $(addprefix $(UTILS_DIR)/,$(UTILS_SRC))

PARSING_OBJ = $(addprefix $(OBJ_DIR)/$(PARSING_DIR)/,$(PARSING_SRC:.c=.o))
GNL_OBJ = $(addprefix $(OBJ_DIR)/$(GNL_DIR)/,$(GNL_SRC:.c=.o))
UTILS_OBJ = $(addprefix $(OBJ_DIR)/$(UTILS_DIR)/,$(UTILS_SRC:.c=.o))
MAIN_OBJ = $(OBJ_DIR)/$(MAIN_SRC:.c=.o)

OBJ = $(PARSING_OBJ) $(GNL_OBJ) $(UTILS_OBJ) $(MAIN_OBJ)

all: $(NAME)

$(OBJ_DIR)/$(PARSING_DIR)/%.o: $(PARSING_DIR)/%.c $(INCLUDE) | $(OBJ_DIR)/$(PARSING_DIR)
	@($(CC) $(FLAGS) -c $< -o $@)
	@echo "Compiled $<"

$(OBJ_DIR)/$(GNL_DIR)/%.o: $(GNL_DIR)/%.c $(INCLUDE) | $(OBJ_DIR)/$(GNL_DIR)
	@($(CC) $(FLAGS) -c $< -o $@)
	@echo "Compiled $<"

$(OBJ_DIR)/$(UTILS_DIR)/%.o: $(UTILS_DIR)/%.c $(INCLUDE) | $(OBJ_DIR)/$(UTILS_DIR)
	@($(CC) $(FLAGS) -c $< -o $@)
	@echo "Compiled $<"

$(OBJ_DIR)/$(MAIN_SRC:.c=.o): $(MAIN_SRC) $(INCLUDE) | $(OBJ_DIR)
	@($(CC) $(FLAGS) -c $< -o $@)
	@echo "Compiled $<"

$(NAME): $(OBJ)
	@($(CC) $(OBJ) -lmlx -framework OpenGL -framework AppKit -o $(NAME))
	@echo "Mandatory executable linked successfully!"

$(OBJ_DIR)/$(PARSING_DIR):
	mkdir -p $@

$(OBJ_DIR)/$(GNL_DIR):
	mkdir -p $@

$(OBJ_DIR)/$(UTILS_DIR):
	mkdir -p $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	@(rm -f $(OBJ))
	@echo "Cleaned object files"

fclean: clean
	@(rm -f $(NAME))
	@echo "Cleaned executable"

re: fclean all

.PHONY: all clean fclean re



# BONUS_NAME = cub3d_bonus
# BONUS_SRC = 
# BONUS_OBJ = $(BONUS_SRC:.c=.o)
# BONUS_INCLUDE = bonus/cub3d_bonus.h

# $(BONUS_OBJ): $(BONUS_SRC)

# bonus: $(BONUS_NAME)

# $(BONUS_NAME): $(BONUS_OBJ)
# 	@($(CC) $(BONUS_OBJ) -lmlx -framework OpenGL -framework AppKit -o $(BONUS_NAME))
# 	@echo "Bonus executable linked successfully!"