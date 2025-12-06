/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 09:36:42 by omaezzem          #+#    #+#             */
/*   Updated: 2025/12/06 17:00:35 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"

size_t ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
	{
		i++;
	}
	return i;
}

void init_cub(t_cub *cub)
{
	cub->map = NULL;
	cub->texture = NULL;
	cub->NO = 0;
	cub->SO = 0;
	cub->WE = 0;
	cub->EA = 0;
	cub->F = 0;
	cub->C = 0;
	cub->north_texture = NULL;
	cub->south_texture = NULL;
	cub->west_texture = NULL;
	cub->east_texture = NULL;
	cub->map_lines = NULL;
	cub->_F = NULL;
	cub->_C = NULL;
}

int pars_av(int ac, char **av)
{
	int	len;

	if (ac != 2)
		return (0);
	len = strlen(av[1]);
	if (len < 4)
		return (0);
	if (strcmp(av[1] + len - 4, ".cub") != 0)
		return (0);
	return (1);
}

void init_texture(char *path, t_cub *cub)
{
	if (strcmp(path, "NO") == 0)
		cub->NO += 1;
	else if (strcmp(path, "SO") == 0)
		cub->SO += 1;
	else if (strcmp(path, "WE") == 0)
		cub->WE += 1;
	else if (strcmp(path, "EA") == 0)
		cub->EA += 1;
	else if (strcmp(path, "F") == 0)
		cub->F += 1;
	else if (strcmp(path, "C") == 0)
		cub->C += 1;
}

int check_path(char *path)
{
	int i;

	if (!path)
		return (0);
	i = 0;
	while (path[i])
		i++;
	if (i < 5)
		return (0);
	i--;
	while (i > 0 && (path[i] == ' ' || path[i] == '\t' ||
		path[i] == '\n' || path[i] == '\r'))
	{
		path[i] = '\0';
		i--;
	}
	if (i >= 3 && path[i] == 'm' && path[i - 1] == 'p' &&
		path[i - 2] == 'x' && path[i - 3] == '.')
		return (1);
	if (i >= 3 && path[i] == 'g' && path[i - 1] == 'n' &&
		path[i - 2] == 'p' && path[i - 3] == '.')
		return (1);
	return (0);
}

int is_empty_line(char *line)
{
	int i;

	if (!line)
		return (1);
	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\r')
			return (0);
		i++;
	}
	return (1);
}
// int check_new_lines(char **str)
// {
// 	int i;
// 	int j;

// 	if (!str)
// 		return (0);
// 	i = 0;
// 	int f = 0;
// 	while (str[i])
// 	{
// 		j = 0;
// 		while (str[i][j])
// 		{
// 			if (str[i][j] == '1')
// 			{
// 				f = 1;
// 				break;
// 			}
// 				j++;
// 		}
// 		if (f == 1)
// 			break;
// 		i++;
// 	}
// 	printf("str[i]: %s\n", str[i]);
// 	return (1);
// }

int	handle_empty_line(t_cub *cub, char *line)
{
	if (is_empty_line(line))
	{
		if (cub->map == NULL)
			return (1);
		ft_putstr_fd("Error: empty line inside map\n", 2);
		return (0);
	}
	return (2);
}

int	handle_colors2(char **split, t_cub *cub)
{
	if (!ft_strcmp(split[0], "F") || !ft_strcmp(split[0], "C"))
	{
		if (!split[1])
			return (0);
		init_texture(split[0], cub);
		return (1);
	}
	return (2);
}

int	handle_textures2(char **split, t_cub *cub)
{
	if (!ft_strcmp(split[0], "NO") || !ft_strcmp(split[0], "SO") ||
		!ft_strcmp(split[0], "WE") || !ft_strcmp(split[0], "EA"))
	{
		if (!split[1] || !check_path(split[1]))
			return (0);
		init_texture(split[0], cub);
		return (1);
	}
	return (2);
}

int	can_start_map(t_cub *cub)
{
	if (cub->NO == 1 && cub->SO == 1 && cub->WE == 1
		&& cub->EA == 1 && cub->F == 1 && cub->C == 1)
		return (1);
	return (0);
}

int	is_path(char **split, t_cub *cub, char *line)
{
	int	status;

	status = handle_empty_line(cub, line);
	if (status != 2)
		return (status);
	if (!split || !split[0])
		return (1);
	status = handle_colors2(split, cub);
	if (status != 2)
		return (status);
	status = handle_textures2(split, cub);
	if (status != 2)
		return (status);
	if (!can_start_map(cub))
		return (0);
	cub->map = ft_strjoin(cub->map, line);
	return (1);
}
static char *clean_newline_str(char *str)
{
    int i = 0;

    if (!str)
        return NULL;
    while (str[i])
    {
        if (str[i] == '\n' || str[i] == '\r')
        {
            str[i] = '\0';
            break;
        }
        i++;
    }
    return str;
}

static void set_texture_or_color(t_cub *cub, char *key, char *value)
{
    if (!key || !value)
        return;
    if (strcmp(key, "NO") == 0)
        cub->north_texture = value;
    else if (strcmp(key, "SO") == 0)
        cub->south_texture = value;
    else if (strcmp(key, "WE") == 0)
        cub->west_texture = value;
    else if (strcmp(key, "EA") == 0)
        cub->east_texture = value;
    else if (strcmp(key, "F") == 0)
        cub->_F = value;
    else if (strcmp(key, "C") == 0)
        cub->_C = value;
    else
        free(value);
}


void join(t_cub *cub, char **path)
{
    char *cleaned;

    if (!path || !path[0] || !path[1])
        return;
    cleaned = ft_strdup(path[1]);
    if (!cleaned)
        return;
    cleaned = clean_newline_str(cleaned);
    set_texture_or_color(cub, path[0], cleaned);
}


void clean_newline(char *str)
{
	int i;

	if (!str)
		return;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\n' || str[i] == '\r')
		{
			str[i] = '\0';
			break;
		}
		i++;
	}
}

int read_map(char *av, t_cub *cub)
{
	int fd;
	char *line;
	char **split;
	char *line2;

	fd = open(av, O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening file");
		return (0);
	}
	line = get_next_line(fd);
	while (line)
	{
		split = ft_split(line, ' ');
		line2 = ft_strdup(line);
		if (!is_path(split, cub, line))
		{
			fprintf(stderr, "Error: Invalid line: %s\n", line);
			if (split)
				free(split);
			free(line);
			close(fd);
			return (0);
		}
		join(cub, split);
		if (split)
			free(split);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);

	if (cub->NO != 1 || cub->SO != 1 || cub->WE != 1 || cub->EA != 1 ||
		cub->F != 1 || cub->C != 1)
	{
		fprintf(stderr, "Error: Missing textures or colors (NO=%d SO=%d WE=%d EA=%d F=%d C=%d)\n",
			cub->NO, cub->SO, cub->WE, cub->EA, cub->F, cub->C);
		return (0);
	}

	// Verify all texture paths were stored
	if (!cub->north_texture || !cub->south_texture ||
		!cub->west_texture || !cub->east_texture)
	{
		fprintf(stderr, "Error: Failed to store texture paths\n");
		return (0);
	}

	printf("\n=== Successfully parsed textures and colors ===\n");
	printf("North: %s\n", cub->north_texture);
	printf("South: %s\n", cub->south_texture);
	printf("West: %s\n", cub->west_texture);
	printf("East: %s\n", cub->east_texture);
	printf("Floor: %s\n", cub->_F);
	printf("Ceiling: %s\n", cub->_C);
	return (1);
}

int handle_colors_F(t_cub *cub)
{
	char **split;
	int i;
	int j;
	char *temp;

	if (!cub->_F)
		return (0);
	i = 0;
	temp = cub->_F;
	while (*temp)
	{
		if (*temp == ',')
			i++;
		temp++;
	}
	if (i != 2)
		return (0);
	split = ft_split(cub->_F, ',');
	if (!split || !split[0] || !split[1] || !split[2])
		return (0);
	j = atoi(split[0]);
	if (j < 0 || j > 255)
		return (0);
	j = atoi(split[1]);
	if (j < 0 || j > 255)
		return (0);
	j = atoi(split[2]);
	if (j < 0 || j > 255)
		return (0);
	return (1);
}

int handle_colors_C(t_cub *cub)
{
	char **split;
	int i;
	int j;
	char *temp;

	if (!cub->_C)
		return (0);
	i = 0;
	temp = cub->_C;
	while (*temp)
	{
		if (*temp == ',')
			i++;
		temp++;
	}
	if (i != 2)
		return (0);
	split = ft_split(cub->_C, ',');
	if (!split || !split[0] || !split[1] || !split[2])
		return (0);
	j = atoi(split[0]);
	if (j < 0 || j > 255)
		return (0);
	j = atoi(split[1]);
	if (j < 0 || j > 255)
		return (0);
	j = atoi(split[2]);
	if (j < 0 || j > 255)
		return (0);
	return (1);
}

int number_of_char(t_cub *cub)
{
	int i;
	int walls;
	int spaces;
	int players;

	if (!cub->map)
		return (0);
	i = 0;
	walls = 0;
	spaces = 0;
	players = 0;
	while (cub->map[i])
	{
		if (cub->map[i] == '1')
			walls++;
		else if (cub->map[i] == '0')
			spaces++;
		else if (cub->map[i] == 'N' || cub->map[i] == 'S' ||
				cub->map[i] == 'E' || cub->map[i] == 'W')
			players++;
		i++;
	}
	if (walls < 1 || spaces < 1 || players != 1)
		return (0);
	return (1);
}

int pars_map(t_cub *cub)
{
	char **map_lines;
	int i;
	int j;

	if (!cub->map)
		return (0);
	map_lines = ft_split(cub->map, '\n');
	if (!map_lines)
		return (0);

	cub->map_lines = map_lines;

	// Check first line
	i = 0;
	while (map_lines[0][i])
	{
		if (map_lines[0][i] != '1' && map_lines[0][i] != ' ' && map_lines[0][i] != '\t')
			return (0);
		i++;
	}

	// Check middle lines
	j = 0;
	while (map_lines[j])
	{
		i = 0;
		if (map_lines[j][i] != '1' && map_lines[j][i] != ' ' && map_lines[j][i] != '\t')
			return (0);
		while (map_lines[j][i])
		{
			if (map_lines[j][i] != '1' && map_lines[j][i] != ' ' &&
				map_lines[j][i] != '\t' && map_lines[j][i] != '0' &&
				map_lines[j][i] != 'N' && map_lines[j][i] != 'S' &&
				map_lines[j][i] != 'E' && map_lines[j][i] != 'W')
				return (0);
			i++;
		}
		if (i > 0 && map_lines[j][i - 1] != '1' &&
			map_lines[j][i - 1] != ' ' && map_lines[j][i - 1] != '\t')
			return (0);
		if (map_lines[j + 1] == NULL)
			break;
		j++;
	}

	// Check last line
	i = 0;
	while (map_lines[j][i])
	{
		if (map_lines[j][i] != '1' && map_lines[j][i] != ' ' && map_lines[j][i] != '\t')
			return (0);
		i++;
	}

	if (!number_of_char(cub))
		return (0);
	return (1);
}

void change_space(t_cub *cub)
{
	int i;

	if (!cub->map)
		return;
	i = 0;
	while (cub->map[i])
	{
		if (cub->map[i] == ' ' || cub->map[i] == '\t')
			cub->map[i] = '1';
		i++;
	}
}

void find_p(t_cub *cub)
{
	int y;
	int x;
	int found;

	y = 0;
	found = 0;
	while (cub->map_lines[y])
	{
		x = 0;
		while (cub->map_lines[y][x])
		{
			if (cub->map_lines[y][x] == 'N' || cub->map_lines[y][x] == 'S' ||
				cub->map_lines[y][x] == 'E' || cub->map_lines[y][x] == 'W')
			{
				cub->player_pos.x = x;
				cub->player_pos.y = y;
				found = 1;
				break;
			}
			x++;
		}
		if (found)
			break;
		y++;
	}

	if (!found)
	{
		ft_putstr_fd("Error: Player not found in map\n", 2);
		exit(EXIT_FAILURE);
	}
}
int check_maps(char **map)
{
	int i = 0;

	i = ft_strlen(map[0]);
	if (map[1][i] == '0')
		return (0);
	return 1;
}
static int get_max_len(char **lines)
{
    int max = 0;
    int len;

    for (int i = 0; lines[i]; i++)
    {
        len = strlen(lines[i]);
        if (len > max)
            max = len;
    }
    return max;
}

static char *pad_line(char *line, int max_len)
{
    int len = strlen(line);
    char *new_line = malloc(max_len + 1);

    if (!new_line)
        return NULL;

    memcpy(new_line, line, len);
    for (int i = len; i < max_len; i++)
        new_line[i] = '1';       // كنزيد spaces باش يتساووا
    new_line[max_len] = '\0';

    return new_line;
}

void pad_map(char **map_lines)
{
    int max_len = get_max_len(map_lines);

    for (int i = 0; map_lines[i]; i++)
    {
        char *padded = pad_line(map_lines[i], max_len);
        free(map_lines[i]);          // نحرّر القديم
        map_lines[i] = padded;       // ندير الجديد
    }
}
void print_parsed_data(t_cub *cub)
{
	printf("\n=== Parsed Configuration ===\n");
	printf("North texture: %s\n", cub->north_texture ? cub->north_texture : "NULL");
	printf("South texture: %s\n", cub->south_texture ? cub->south_texture : "NULL");
	printf("West texture: %s\n", cub->west_texture ? cub->west_texture : "NULL");
	printf("East texture: %s\n", cub->east_texture ? cub->east_texture : "NULL");
	printf("Floor color: %s\n", cub->_F ? cub->_F : "NULL");
	printf("Ceiling color: %s\n", cub->_C ? cub->_C : "NULL");
	printf("\n=== Map ===\n%s\n", cub->map ? cub->map : "NULL");
}
