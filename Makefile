NAME = cub3D
NAME_BONUS = cub3D_bonus
CC = cc
MLX_DIR = mlx
MLX = $(MLX_DIR)/libmlx.a
CPPFLAGS += -I$(MLX_DIR)
MLXFLAGS = $(MLX) -framework OpenGL -framework AppKit
CFLAGS += -Wall -Werror -Wextra

SRC =	mandatory/parse/parsing.c mandatory/parse/check_file.c mandatory/parse/check_map.c mandatory/parse/exit.c mandatory/parse/free.c  mandatory/parse/p_utils.c\
		mandatory/parse/get_next_line.c mandatory/parse/rgb.c mandatory/parse/main.c mandatory/parse/check_map3.c mandatory/parse/check_map2.c \
		mandatory/raycasting/init_mlx.c mandatory/raycasting/init_raycasting.c mandatory/raycasting/player.c mandatory/raycasting/update_3d_value.c  \
		mandatory/raycasting/render_ray_utils.c mandatory/raycasting/render_ray.c mandatory/raycasting/texture.c mandatory/raycasting/utils.c mandatory/parse/coma.c mandatory/parse/check_file_utils.c \

SRC_BONUS =		bonus/parse_bonus/parsing_bonus.c bonus/parse_bonus/check_file_bonus.c bonus/parse_bonus/check_map_bonus.c bonus/parse_bonus/exit_bonus.c \
				bonus/parse_bonus/free_bonus.c bonus/parse_bonus/get_next_line_bonus.c bonus/parse_bonus/rgb_bonus.c  bonus/raycasting_bonus/mini_map_bonus.c\
				bonus/parse_bonus/main.c bonus/parse_bonus/check_map3_bonus.c bonus/parse_bonus/check_map2_bonus.c bonus/parse_bonus/check_file_utils_bonus.c \
				bonus/raycasting_bonus/init_mlx_bonus.c  bonus/raycasting_bonus/player_bonus.c  bonus/raycasting_bonus/init_raycasting_bonus.c bonus/parse_bonus/coma_bonus.c\
				bonus/raycasting_bonus/update_3d_value_bonus.c bonus/raycasting_bonus/render_ray_utils_bonus.c bonus/raycasting_bonus/utils_bonus.c \
				bonus/raycasting_bonus/render_ray_bonus.c bonus/raycasting_bonus/texture_bonus.c  bonus/raycasting_bonus/render_player_bonus.c bonus/parse_bonus/p_utils_bonus.c\


SRC_BONUS += bonus/blockyard/world.c bonus/blockyard/render.c \
	bonus/blockyard/material.c bonus/blockyard/characters.c \
	bonus/blockyard/hud.c bonus/blockyard/input.c \
	bonus/blockyard/build.c bonus/blockyard/adventure_hud.c

LIB=./libft/libft.a
OBJS = ${SRC:.c=.o}

OBJB = ${SRC_BONUS:.c=.o}



all : ${NAME}

${NAME} : $(LIB) $(MLX) ${OBJS}
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIB) $(MLXFLAGS)

bonus :	${NAME_BONUS}
${NAME_BONUS} : $(LIB) $(MLX) ${OBJB}
	$(CC) $(CFLAGS) $(OBJB) -o $(NAME_BONUS) $(LIB) $(MLXFLAGS)

$(MLX):
	$(MAKE) -C $(MLX_DIR)

$(LIB):
	cd libft && make
clean:
	rm -rf ${OBJS} ${OBJB}
	cd ./libft && make clean

fclean: clean
	rm -f ${NAME} ${NAME_BONUS}
	cd ./libft && make fclean

re: fclean all

.PHONY: all bonus clean fclean re run-blockyard check-blockyard

$(OBJB): bonus/includes/cub3d_bonus.h bonus/includes/blockyard.h
$(OBJB): CFLAGS += -O2

run-blockyard: bonus
	./$(NAME_BONUS) maps/island.cub

check-blockyard: $(NAME_BONUS)
	$(CC) $(CFLAGS) $(CPPFLAGS) tests/blockyard_test.c $(filter-out bonus/parse_bonus/main.o,$(OBJB)) $(LIB) $(MLXFLAGS) -o /tmp/cub3dd-blockyard-test
	/tmp/cub3dd-blockyard-test
	python3 tests/blockyard_maps.py

.PHONY: check-parser-safety
check-parser-safety: $(LIB) $(MLX) $(OBJS) $(OBJB)
	$(CC) $(CFLAGS) $(CPPFLAGS) -DTEST_MANDATORY tests/parser_safety.c $(filter-out mandatory/parse/main.o,$(OBJS)) $(LIB) $(MLXFLAGS) -o /tmp/cub3dd-parser-test
	/tmp/cub3dd-parser-test
	$(CC) $(CFLAGS) $(CPPFLAGS) tests/parser_safety.c $(filter-out bonus/parse_bonus/main.o,$(OBJB)) $(LIB) $(MLXFLAGS) -o /tmp/cub3dd-bonus-parser-test
	/tmp/cub3dd-bonus-parser-test
