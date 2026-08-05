/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icoman <icoman@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 09:49:38 by icoman            #+#    #+#             */
/*   Updated: 2026/08/05 09:49:41 by icoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	parse_numeric_arg(const char *arg)
{
	if (!is_numeric(arg))
		return (-1);
	return (atoi(arg));
}

static int	validate_args(t_data *data, char **argv)
{
	if (data->num_coders <= 0 || data->time_to_burnout < 0
		|| data->time_to_compile < 0 || data->time_to_debug < 0
		|| data->time_to_refactor < 0 || data->compiles_required < 0
		|| data->dongle_cooldown < 0)
	{
		fprintf(stderr, "Error: Invalid or missing numeric arguments\n");
		return (1);
	}
	if (strcmp(argv[8], "fifo") == 0)
		data->scheduler = 0;
	else if (strcmp(argv[8], "edf") == 0)
		data->scheduler = 1;
	else
	{
		fprintf(stderr, "Error: Invalid scheduler, chhose fifo or edf\n");
		return (1);
	}
	return (0);
}

int	parse_args(int argc, char **argv, t_data *data)
{
	if (argc != 9)
	{
		fprintf(stderr, "Usage: ./codexion [number_of_coders] ");
		fprintf(stderr, "[time_to_burnout] [time_to_compile] [time_to_debug] ");
		fprintf(stderr, "[time_to_refactor] [number_of_compiles_required] ");
		fprintf(stderr, "[dongle_cooldown] [scheduler]\n");
		return (1);
	}
	data->num_coders = parse_numeric_arg(argv[1]);
	data->time_to_burnout = parse_numeric_arg(argv[2]);
	data->time_to_compile = parse_numeric_arg(argv[3]);
	data->time_to_debug = parse_numeric_arg(argv[4]);
	data->time_to_refactor = parse_numeric_arg(argv[5]);
	data->compiles_required = parse_numeric_arg(argv[6]);
	data->dongle_cooldown = parse_numeric_arg(argv[7]);
	return (validate_args(data, argv));
}
