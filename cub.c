/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 10:47:10 by mel-badd          #+#    #+#             */
/*   Updated: 2025/12/06 18:09:27 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static int	init_map(t_cub *cub, char *file)
{
	if (!read_map(file, cub))
		return (0);
	if (!handle_colors_str(cub->_F) || !handle_colors_str(cub->_C))
		return (0);
	if (!pars_map(cub))
		return (0);
	change_space(cub);
	cub->map_lines = ft_split(cub->map, '\n');
	if (!cub->map_lines)
		return (0);
	find_p(cub);
	if (!check_maps(cub->map_lines))
		return (0);
	pad_map(cub->map_lines);
	return (1);
}

static void	build_map_string(t_cub *cub)
{
	int	i;

	i = 0;
	free(cub->map);
	cub->map = strdup("");
	while (cub->map_lines[i])
	{
		cub->map = ft_strjoin(cub->map, cub->map_lines[i]);
		cub->map = ft_strjoin(cub->map, "\n");
		i++;
	}
}

int	main(int ac, char **av)
{
	t_cub	cub;

	init_cub(&cub);
	if (!pars_av(ac, av))
		return (fprintf(stderr, "Error: Invalid arguments\n"), EXIT_FAILURE);
	if (!init_map(&cub, av[1]))
		return (fprintf(stderr, "Error: Failed to init map\n"), EXIT_FAILURE);
	build_map_string(&cub);
	cub.floor_color = 0x573D32;
	cub.ceiling_color = 0x87CEEB;
	init_player_raycasting(&cub);
	mlx_initcub(&cub);
	return (0);
}
