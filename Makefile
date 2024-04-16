NAME = cub3D
NAME_BONUS = cub3D_bonus
CC = cc
MLXFLAGS = -lmlx -framework OpenGl -framework AppKit
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


LIB=./libft/libft.a
OBJS = ${SRC:.c=.o}

OBJB = ${SRC_BONUS:.c=.o}



all : ${NAME}

${NAME} : $(LIB) ${OBJS}
	$(CC) $(CFLAGS) $(MLXFLAGS) $(SRC) -o $(NAME) $(LIB)

bonus :	${NAME_BONUS}
${NAME_BONUS} : $(LIB) ${OBJB}
	$(CC) $(CFLAGS) $(MLXFLAGS) $(SRC_BONUS) -o $(NAME_BONUS) $(LIB)

$(LIB):
	cd libft && make
clean:
	rm -rf ${OBJS} ${OBJB}
	cd ./libft && make clean

fclean: clean
	rm -f ${NAME} ${NAME_BONUS}
	cd ./libft && make fclean

re: fclean all