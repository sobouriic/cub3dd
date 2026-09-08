/* Shared regression checks for mandatory and bonus color parsers. */
#ifdef TEST_MANDATORY
# include "../mandatory/includes/cub3d.h"
#else
# include "../bonus/includes/cub3d_bonus.h"
#endif
#include <assert.h>
#include <string.h>

int main(void)
{
	const char *bad[] = {"F ,,1", "F 1,,", "F ,1,", "F 1, ,2",
		"F 1,2,", "F 256,0,0", "F -1,2,3", "F 1,2,3,4", "F 1,2,x",
		"F 999999999999999999999999999999999999999999,1,2", NULL};
	int i, color;
	char *line;
	t_parse parse;
	for (i = 0; bad[i]; i++)
	{
		line = ft_strdup(bad[i]); color = 42;
		assert(color_parsing(&color,line) == ERROR);
		assert(color == 42);
		free(line);
	}
	line = ft_strdup("F 0, 255, 001");
	assert(color_parsing(&color,line) == SUCCESS && color == 0x00ff01);
	free(line);
	assert(color_parsing(&color,NULL) == ERROR);
	assert(check_rgb(&color,NULL) == ERROR);
	memset(&parse,0,sizeof(parse));
	line = ft_strdup("C 1,2,3");
	assert(is_color(CIEL,&parse,line) == SUCCESS);
	assert(is_color(CIEL,&parse,line) == ERROR);
	free(line);
	puts("PASS: malformed RGB, overflow, unchanged output on failure, null input, duplicate ceiling");
	return (0);
}
