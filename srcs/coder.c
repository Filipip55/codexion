/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icoman <icoman@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 09:50:16 by icoman            #+#    #+#             */
/*   Updated: 2026/08/13 15:03:58 by icoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	coder_work_cycle(t_coder *coder, t_data *data)
{
	pthread_mutex_lock(&coder->state_mutex);
	coder->last_compile = elapsed_ms(data);
	coder->deadline = coder->last_compile + data->time_to_burnout;
	pthread_mutex_unlock(&coder->state_mutex);
	print_log_msg(data, coder->id, "is compiling");
	smart_sleep(data->time_to_compile, data);
	pthread_mutex_lock(&coder->state_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->state_mutex);
	release_dongle(data, coder->left_dongle);
	release_dongle(data, coder->right_dongle);
	print_log_msg(data, coder->id, "is debugging");
	smart_sleep(data->time_to_debug, data);
	print_log_msg(data, coder->id, "is refactoring");
	smart_sleep(data->time_to_refactor, data);
}

static void	*single_coder_routine(t_data *data)
{
	while (!check_sim_stop(data))
		smart_sleep(100, data);
	return (NULL);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_data	*data;

	coder = (t_coder *)arg;
	data = coder->data;
	if (data->num_coders == 1)
		return (single_coder_routine(data));
	while (!check_sim_stop(data))
	{
		if (acquire_dongle(data, coder, coder->left_dongle))
			break ;
		if (acquire_dongle(data, coder, coder->right_dongle))
		{
			release_dongle(data, coder->left_dongle);
			break ;
		}
		coder_work_cycle(coder, data);
	}
	return (NULL);
}
