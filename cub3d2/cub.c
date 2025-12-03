/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 10:47:10 by mel-badd          #+#    #+#             */
/*   Updated: 2025/12/03 18:44:58 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int main(int ac, char **av)
{
	t_cub cub;

	init_cub(&cub);
	if (!pars_av(ac, av))
	{
		fprintf(stderr, "Error: Invalid arguments\n");
		return (EXIT_FAILURE);
	}
	if (!read_map(av[1], &cub))
	{
		fprintf(stderr, "Error: Failed to read map\n");
		return (EXIT_FAILURE);
	}
	if (!handle_colors_F(&cub))
	{
		fprintf(stderr, "Error: Invalid colors\n");
		return (EXIT_FAILURE);
	}
	if (!handle_colors_C(&cub))
	{
		fprintf(stderr, "Error: Invalid colors\n");
		return (EXIT_FAILURE);
	}
	if (!pars_map(&cub))
	{
		fprintf(stderr, "Error: Invalid map\n");
		return (EXIT_FAILURE);
	}
	change_space(&cub);
	cub.map_lines = ft_split(cub.map, '\n');
	if (!cub.map_lines)
	{
		fprintf(stderr, "Error: Failed to split map\n");
		return (EXIT_FAILURE);
	}
	find_p(&cub);
	if (check_maps(cub.map_lines) == 0)
	{
		fprintf(stderr, "Error: Invalid map\n");
		return (EXIT_FAILURE);
	}
	pad_map(cub.map_lines);
	// printf("%s\n", cub.map_lines[0]);
	int i = 0;
	cub.map = strdup("");
	while (cub.map_lines[i])
	{
		cub.map = ft_strjoin(cub.map, cub.map_lines[i]);
		cub.map = ft_strjoin(cub.map, "\n");
		i++;
	}
	printf("%s\n",cub.map);

	// raycast=====================
	init_player_raycasting(&cub);
	// cub.nopath = "./imgs/49_Italian-stone-wall_DIFF.xpm";
    // cub.sopath = "./imgs/49_Italian-stone-wall_DISPL.xpm";
    // cub.espath = "./imgs/49_Italian-stone-wall_NORM.xpm";
    // cub.wepath = "./imgs/49_Italian-stone-wall_SPEC.xpm";
	cub.floor_color = 0x573D32;
	cub.ceiling_color = 0x87CEEB;
	mlx_initcub(&cub);

	// free_all(&cub);
	return (0);
}
