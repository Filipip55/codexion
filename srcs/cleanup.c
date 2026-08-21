/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icoman <icoman@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 09:50:18 by icoman            #+#    #+#             */
/*   Updated: 2026/08/05 09:50:19 by icoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	cleanup(t_data *data)
{
	int	i;

	if (data->dongles)
	{
		i = 0;
		while (i < data->num_coders)
		{
			pthread_mutex_destroy(&data->dongles[i].mutex);
			pthread_cond_destroy(&data->dongles[i].cond);
			queue_destroy(&data->dongles[i].wait_queue);
			i++;
		}
		free(data->dongles);
	}
	if (data->coders)
	{
		i = 0;
		while (i < data->num_coders)
		{
			pthread_mutex_destroy(&data->coders[i].state_mutex);
			i++;
		}
		free(data->coders);
	}
	pthread_mutex_destroy(&data->log_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
}
