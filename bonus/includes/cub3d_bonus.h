/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 17:24:52 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/29 15:11:07 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_BONUS_H
# define CUB3D_BONUS_H

# include <fcntl.h>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <math.h>
# include <unistd.h>
# include "../../libft/libft.h"
# include "mlx.h"
# include <stdio.h>

# define TRUE       1
# define FALSE      0
# define SUCCESS    0
# define ERROR      1
# define KEY_ESC    53
# define KEY_A      0
# define KEY_W      13
# define KEY_S      1
# define KEY_D      2
# define KEY_LEFT   123
# define KEY_UP    126
# define KEY_RIGHT  124
# define KEY_DOWN   125
# define COLOR      0x222200
# define PI         3.14159265358979323846264338327950288
# define DOUBLE_PI  6.28318530717958647692528676655900576
# define FOV_ANGLE  1.047197551
# define HALF_PI    1.57079632679489661923132169163975144
# define W_H        20
# define W_W        32
# define WALL_STRIP 1
# define TEX_COUNT  4
# define W_SPEED    2
# define T_SPEED    0.02617993878
# define ERR		-1
# define NORTH		0
# define SOUTH 		1
# define WEST		2
# define EAST		3
# define FLOOR		4
# define CIEL		5
# define MAP		6
# define COUNT		4
# define SIZE_W		35
# define MAP_SCALE  0.1
# define PLAYER_POINT 2
# define RANGE_RAY	FOV_ANGLE
# define LEFTUP_MAP 	1
# define LEFTDOWN_MAP 	2
# define RIGHTUP_MAP 	3
# define RIGHTDOWN_MAP	4
# define LOCATION RIGHTDOWN_MAP

typedef struct s_keyboard
{
	int	left;
	int	right;
	int	down;
	int	up;
	int	updown_sight;
	int	right_rotat;
	int	left_rotat;
}	t_keyboard;

typedef struct s_text
{
	int		*texture;
	char	*path;
	double	w;
	double	h;
}	t_text;

typedef struct s_line_size_node
{
	struct s_line_size_node	*next;
	size_t					size;
}	t_line_size_node;

typedef struct s_do_parsing
{
	int					fd;
	int					parse;
	char				*line;
	t_line_size_node	*line_sizes;
	int					get_value;
}	t_do_parsing;

typedef struct s_parse
{
	int				d;
	int				f;
	int				floor;
	int				ceiling;
	int				row;
	int				column;
	int				c;
	char			**map;
	char			**tm_map;
	char			*temp_map;
	int				var;
	t_text			texture[COUNT];
	t_do_parsing	pa;
	int				flag;
}	t_parse;

typedef struct s_calcule_ray
{
	int		x;
	int		y;
	double	r_x;
	double	r_y;
	double	maximum;
}	t_calcule_ray;

typedef struct s_get_line
{
	char	*buff;
	char	*tmp;
	ssize_t	ofset;
	ssize_t	i;
}	t_get_line;

typedef struct s_map2d
{
	char	**map;
	int		map_col;
	int		map_row;
	int		win_width;
	int		win_height;
	int		r_count;
	int		size_tile;
}	t_map2d;

typedef struct s_mlx_image
{
	int		*data;
	int		line_size;
	int		img_h;
	int		img_w;
	void	*ptr;
	int		endian;
	int		bpp;
}	t_mlx_image;

typedef struct s_point
{
	double	x;
	double	y;
}	t_point;

typedef struct s_init_var
{
	double	t_dir;
	double	w_dir;
	double	n_x;
	double	n_y;
	double	m_side;
	int		m_step;
}	t_init_var;

typedef struct s_play
{
	double	x;
	double	y;
	double	r_angle;
	double	w_sped;
	double	t_sped;
	double	up_si;
	int		point;
}	t_player;

typedef struct s_data_ray
{
	int		f_w_hit;
	double	x_inter;
	double	y_inter;
	double	x_step;
	double	y_step;
	double	w_hitx;
	double	w_hity;
	double	distance;
}	t_data_ray;

typedef struct s_ray
{
	int		wal_pap;
	int		faci_down;
	int		faci_up;
	int		faci_right;
	int		faci_left;
	double	ray_ang;
}	t_ray;

typedef struct s_treed
{
	int		bottom;
	int		corr_bott;
	int		eight;
	int		width;
	double	corr_distance;
	double	dis_p;
	int		p_hei;
	int		top;
	int		corr_top;
}	t_treed;

typedef struct s_rrray
{
	int		hit_ver;
	double	w_hitx;
	double	wall_hity;
	double	distance;
	double	angle;
}	t_rrray;

typedef struct s_init_edge
{
	int	x;
	int	y;
	int	x1;
	int	y1;
}	t_init_edge;

typedef struct s_parsed_line
{
	struct s_parsed_line	*next;
	int						type;
}	t_line;

typedef struct s_norm_init
{
	int		count;
	t_line	*parsed_lines;
	t_line	*current_line;
}	t_norm_init;

typedef struct s_cub3d
{
	t_parse		parse;
	t_keyboard	key;
	t_player	player;
	t_map2d		map;
	t_mlx_image	img;
	t_ray		ray;
	t_rrray		*rray;
	t_text		texture;
	t_init_var	init_var;
	void		*mlx;
	void		*win;
}	t_cub3d;

void				ft_free_str(char *str);
int					ft_destroy(t_cub3d *cub, int ret);
void				ft_error(char *err);
int					exit_game(t_cub3d *cub, int ret, const char *err);
int					get_next_line(int fd, char **line);
int					check_ext(const char *file);
int					texture_type(char *line);
int					check_map_char(char *str);
int					empty_line(char *line);
int					texture_path(t_parse *parse, int texture, char *line);
void				ft_free_str(char *str);
void				ft_free(void **arr, int i);
void				arr_free(void *arr);
int					arr_free_return(void *arr, int value);
int					check_rgb(int *x, char **palet);
int					color_parsing(int *x, char *line);
int					is_color(int texture, t_parse *parse, char *line);
int					check_map(t_parse *parse, int get_value,
						int texture, char *line);
int					sky_ground(t_parse *parse, int texture, char *line);
char				*path_parsing(char *line);
int					parsing(t_cub3d *cub, const char *file);
char				*map_update(char *str1, char *str2);
int					final_map(t_parse *parse);
int					parse_lines(t_cub3d *cub, t_do_parsing *parse);
int					open_file(t_cub3d *cub, const char *file);
int					handle_parsing(t_parse *parse, int get_value, char *line);
void				init_f_c(t_cub3d *cub);
int					the_wall(t_cub3d *cub, double x, double y);
int					check_map2(t_cub3d *cub);
int					check_edge(t_cub3d *cub, const t_point *first,
						const t_point *last);
int					final_parsing(t_cub3d *cub);
int					check_map3(t_cub3d *cub);
int					check_map_dimensions(t_cub3d *cub);
double				distance_entre_points(double x1, double x2,
						double y1, double y2);
int					conditional_operator(int p, int true, int false);
int					conditional_operator2(double p, double true, double false);
void				init_threed(t_cub3d *cub, t_treed *t, int i);
int					set_color_wall(t_cub3d *cub, t_treed *m, int re, int i);
void				walls_3d_projection(t_cub3d *cub, int i);
void				calcule_ray(t_cub3d *cub, t_data_ray *hv, int a, int b);
void				main_init(t_cub3d *cub);
void				init_mlx(t_cub3d *cub);
void				apload_texture(t_cub3d *cub);
void				main_update(t_cub3d *cub);
void				update_3d_value(t_cub3d *cub);
void				calcule_horiz_rays(t_cub3d *cub, t_data_ray *hor);
void				init_rays(double rayAng, t_ray *ray);
void				update_rays(t_cub3d *cub, double ang, int i);
void				update_multiple_rays(t_cub3d *cub);
void				init_mlx_loop(t_cub3d *cub);
void				init_screen(t_cub3d *cub);
void				walls_3d_projection_render(t_cub3d *cub);
void				text_free(t_cub3d *cub);
void				map_rendering(t_cub3d *cub);
void				player_render(t_cub3d *cub);
void				map_init(t_cub3d *cub);
void				mini_map_location(t_parse *parse, int *x,
						int *y, t_point *point);
void				put_squares(t_cub3d *cub, int x, int y, int collor);
void				put_color(t_cub3d *cub, int row, int colum);
void				map_rendering(t_cub3d *cub);
void				determine_point(t_point *point, double x, double y);
void				map_rendering(t_cub3d *cub);
void				render_ray_mini(t_cub3d *cub);
char				*trim_line(char *line);
int					game_loop(t_cub3d *cub);
int					check_spaces(t_cub3d *cub, int row, int colum);
int					keyrel(int keycode, t_keyboard *key);
int					key_press(int keycode, t_keyboard *key);
int					ft_exit(void);
int					process_parsed_lines(t_line *parsed_lines);
void				init_player(t_player *player);
int					add_line_size_node(t_line_size_node **head, size_t size);
void				free_parsed_lines(t_line *parsed_lines);
int					handle_parsing(t_parse *parse, int get_value, char *line);
int					parsing(t_cub3d *cub, const char *file);
int					add_line_size_node(t_line_size_node **head, size_t size);
void				free_line_size_list(t_line_size_node *head);
t_line_size_node	*create_line_size_node(size_t size);

#endif