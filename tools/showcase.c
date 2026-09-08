#include "../bonus/includes/blockyard.h"

static void scene(int index)
{
	ft_memset(vx()->keys,0,sizeof(vx()->keys));
	vx()->pitch = 0;
	vx()->help = 0;
	vx()->map_on = 1;
	vx()->build_mode = 0;
	vx()->attack = 0;
	vx()->message_until = 0;
	if (index == 0) { vx()->x = 5.5; vx()->y = 15.5; vx()->angle = -0.15; }
	if (index == 1) { vx()->x = 6.5; vx()->y = 6.3; vx()->angle = -HALF_PI; vx()->weapon = 1; }
	if (index == 2) { vx()->x = 12.5; vx()->y = 14.7; vx()->angle = -HALF_PI; }
	if (index == 3) { vx()->x = 23.1; vx()->y = 15.5; vx()->angle = 0; vx()->build_mode = 1; vx()->block = 2; }
	if (index == 4) { vx()->map_on = 2; }
}

int main(int argc, char **argv)
{
	t_cub3d *cub;
	FILE *out;
	int frame, i, step, part;
	unsigned int pixels[VX_W*VX_H];
	const char *titles[] = {"01 / EXPLORE THE VILLAGE", "02 / MINE CRYSTAL ORE",
		"03 / OPEN DOORS", "04 / BUILD YOUR CROSSING", "05 / FIND THE SIX SHARDS"};
	if (argc != 2) return (1);
	cub = ft_calloc(1,sizeof(*cub));
	init_f_c(cub);
	if (parsing(cub,"maps/island.cub") || final_parsing(cub)) return (1);
	main_init(cub);
	vx_init(cub);
	out = fopen(argv[1],"wb");
	if (!out) return (1);
	for (frame = 0; frame < 600; frame++)
	{
		part = frame/120; step = frame%120;
		if (!step) scene(part);
		if (part == 0) vx()->angle = -0.15+0.30*step/120;
		if (part == 1 && step >= 20 && step < 55) vx()->keys[49] = 1;
		else vx()->keys[49] = 0;
		if (part == 2 && step == 25) vx_interact();
		if (part == 2) vx()->keys[KEY_W] = step > 60 && step < 83;
		if (part == 3)
		{
			if (step == 15 || step == 35) vx_place();
			vx()->keys[KEY_W] = step > 60 && step < 103;
		}
		vx_update(1.0/30);
		vx_render();
		vx_rect(12,60,260,22,0x21352f);
		vx_text(20,68,titles[part],0xf3dfb2,1);
		for (i = 0; i < VX_W*VX_H; i++) pixels[i] = (unsigned int)vx()->frame[i]|0xff000000u;
		if (fwrite(pixels,sizeof(pixels),1,out) != 1) return (1);
		if (frame == 390) vx_capture("/tmp/blockyard-video-poster.ppm");
	}
	fclose(out);
	return (0);
}
