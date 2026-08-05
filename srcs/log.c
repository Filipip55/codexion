/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icoman <icoman@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 09:49:59 by icoman            #+#    #+#             */
/*   Updated: 2026/08/05 09:50:00 by icoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_action(t_data *data, int coder_id, char *action)
{
	long	timestamp;

	pthread_mutex_lock(&data->log_mutex);
	if (!check_sim_stop(data))
	{
		timestamp = elapsed_ms(data);
		printf("%ld %d %s\n", timestamp, coder_id, action);
	}
	pthread_mutex_unlock(&data->log_mutex);
}

void	log_burnout(t_data *data, int coder_id)
{
	long	timestamp;

	pthread_mutex_lock(&data->log_mutex);
	timestamp = elapsed_ms(data);
	printf("%ld %d burned out\n", timestamp, coder_id);
	set_sim_stop(data);
	pthread_mutex_unlock(&data->log_mutex);
}
