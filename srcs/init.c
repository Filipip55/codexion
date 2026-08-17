/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icoman <icoman@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 09:50:03 by icoman            #+#    #+#             */
/*   Updated: 2026/08/05 09:50:04 by icoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_dongles(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_coders)
	{
		if (pthread_mutex_init(&data->dongles[i].mutex, NULL) != 0)
			return (1);
		if (pthread_cond_init(&data->dongles[i].cond, NULL) != 0)
			return (1);
		data->dongles[i].available = 1;
		data->dongles[i].release_time = 0;
		queue_init(&data->dongles[i].wait_queue, data->num_coders);
		i++;
	}
	return (0);
}

static void	assign_dongles(t_data *data, int i)
{
	if (data->coders[i].id % 2 == 0)
	{
		data->coders[i].left_dongle = i - 1;
		data->coders[i].right_dongle = i;
	}
	else
	{
		data->coders[i].left_dongle = i;
		data->coders[i].right_dongle = i - 1;
		if (i == 0)
			data->coders[i].right_dongle = data->num_coders - 1;
	}
}

static void	init_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].compile_count = 0;
		data->coders[i].last_compile = 0;
		data->coders[i].data = data;
		data->coders[i].thread = 0;
		assign_dongles(data, i);
		i++;
	}
}

int	init_data(t_data *data)
{
	data->dongles = malloc(sizeof(t_dongle) * data->num_coders);
	if (!data->dongles)
		return (1);
	data->coders = malloc(sizeof(t_coder) * data->num_coders);
	if (!data->coders)
	{
		free(data->dongles);
		return (1);
	}
	if (pthread_mutex_init(&data->log_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
		return (1);
	if (init_dongles(data) != 0)
		return (1);
	init_coders(data);
	data->sim_stop = 0;
	data->all_done = 0;
	return (0);
}

void	set_initial_times(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_coders)
	{
		data->coders[i].last_compile = 0;
		data->coders[i].deadline = data->time_to_burnout;
		i++;
	}
}
