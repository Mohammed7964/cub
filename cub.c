/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 10:47:10 by mel-badd          #+#    #+#             */
/*   Updated: 2025/12/20 15:54:01 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static int	is_invalid_char(char c)
{
	return (c == ' ' || c == '\0');
}
static int	check_ziro(t_cub *cub)
{
	int	i;
	int	j;

	if (!cub || !cub->map_lines)
		return (0);
	i = 0;
	while (cub->map_lines[i])
	{
		j = 0;
		while (cub->map_lines[i][j])
		{
			if (cub->map_lines[i][j] == '0' ||
			    cub->map_lines[i][j] == 'N' ||
			    cub->map_lines[i][j] == 'S' ||
			    cub->map_lines[i][j] == 'E' ||
			    cub->map_lines[i][j] == 'W')
			{
				if (i == 0 || j == 0 || !cub->map_lines[i + 1])
					return (0);
				if (j >= (int)strlen(cub->map_lines[i - 1]) ||
				    j >= (int)strlen(cub->map_lines[i + 1]))
					return (0);
				if (is_invalid_char(cub->map_lines[i][j - 1]) ||
				    is_invalid_char(cub->map_lines[i][j + 1]) ||
				    is_invalid_char(cub->map_lines[i - 1][j]) ||
				    is_invalid_char(cub->map_lines[i + 1][j]))
					return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}
int new_lines_map(t_cub *cub)
{
	int i;
	int end;

	i = 0;
	/* skip leading newlines */
	while (cub->map[i] && cub->map[i] == '\n')
		i++;

	end = ft_strlen(cub->map) - 1;
	/* skip trailing newlines */
	while (end > i && cub->map[end] == '\n')
		end--;

	while (i < end)
	{
		if (cub->map[i] == '\n' && cub->map[i + 1] == '\n')
		{
			fprintf(stderr, "Error: New line inside map\n");
			return (0);
		}
		i++;
	}
	return (1);
}

static int	init_map(t_cub *cub, char *file)
{
	if (!read_map(file, cub))
		return (0);
	if (!new_lines_map(cub))
		return (0);
	printf("Map read successfully:\n%s\n", cub->map);
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
	if (!check_ziro(cub))
		return (0);
	pad_map(cub->map_lines);
	return (1);
}
void	cleanup(t_cub *cub)
{
	if (cub->map)
		free(cub->map);

	if (cub->map_lines)
		ft_free_split(cub->map_lines);

	free(cub->north_texture);
	free(cub->south_texture);
	free(cub->west_texture);
	free(cub->east_texture);
	free(cub->_F);
	free(cub->_C);
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
void f()
{
	system("leaks cub3D");
}

int	main(int ac, char **av)
{
	t_cub	cub;
	// atexit(f);
	init_cub(&cub);
	if (!pars_av(ac, av))
	{
		cleanup(&cub);
		return (fprintf(stderr, "Error: Invalid arguments\n"), EXIT_FAILURE);
	}
	if (!init_map(&cub, av[1]))
	{
		cleanup(&cub);
		return (fprintf(stderr, "Error: Failed to init map\n"), EXIT_FAILURE);
	}
	build_map_string(&cub);
	// cub.floor_color = 0x573D32;
	// cub.ceiling_color = 0x87CEEB;
	// init_player_raycasting(&cub);
	// mlx_initcub(&cub);
	cleanup(&cub);
	return (0);
}
