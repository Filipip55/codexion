/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icoman <icoman@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 09:50:13 by icoman            #+#    #+#             */
/*   Updated: 2026/08/13 15:03:58 by icoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	set_timeout_absolute(struct timespec *ts, long timeout_ms)
{
	struct timeval	now;
	long			remaining_ms;

	gettimeofday(&now, NULL);
	remaining_ms = timeout_ms - get_time_ms();
	if (remaining_ms < 0)
		remaining_ms = 0;
	ts->tv_sec = now.tv_sec + (remaining_ms / 1000);
	ts->tv_nsec = (now.tv_usec * 1000) + ((remaining_ms % 1000) * 1000000);
	if (ts->tv_nsec >= 1000000000)
	{
		ts->tv_sec++;
		ts->tv_nsec -= 1000000000;
	}
}

static long	get_wait_until(t_data *data, t_dongle *dongle, t_coder *coder)
{
	long	wait_until;
	long	cooldown_until;

	wait_until = data->start_time + coder->deadline;
	if (dongle->available
		&& queue_peek(&dongle->wait_queue).coder_id == coder->id)
	{
		cooldown_until = dongle->release_time + data->dongle_cooldown;
		if (cooldown_until < wait_until)
			wait_until = cooldown_until;
	}
	return (wait_until);
}

static int	wait_dongle_loop(t_data *data, t_dongle *dongle, t_coder *coder)
{
	struct timespec	ts;
	int				ret;

	while (!check_sim_stop(data))
	{
		if (dongle->available
			&& queue_peek(&dongle->wait_queue).coder_id == coder->id)
		{
			if (get_time_ms() >= dongle->release_time + data->dongle_cooldown)
				break ;
		}
		set_timeout_absolute(&ts, get_wait_until(data, dongle, coder));
		ret = pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
		if (ret == ETIMEDOUT)
		{
			if (dongle->available
				&& queue_peek(&dongle->wait_queue).coder_id == coder->id
				&& get_time_ms() >= dongle->release_time
				+ data->dongle_cooldown)
				continue ;
			break ;
		}
	}
	return (0);
}

int	acquire_dongle(t_data *data, t_coder *coder, int dongle_idx)
{
	t_dongle	*dongle;
	long		priority;

	dongle = &data->dongles[dongle_idx];
	pthread_mutex_lock(&dongle->mutex);
	if (data->scheduler == 0)
		priority = get_time_ms();
	else
		priority = coder->deadline;
	queue_push(&dongle->wait_queue, coder->id, priority);
	wait_dongle_loop(data, dongle, coder);
	if (check_sim_stop(data)
		|| get_time_ms() >= data->start_time + coder->deadline)
	{
		if (queue_peek(&dongle->wait_queue).coder_id == coder->id)
			queue_pop(&dongle->wait_queue);
		pthread_cond_broadcast(&dongle->cond);
		pthread_mutex_unlock(&dongle->mutex);
		return (1);
	}
	queue_pop(&dongle->wait_queue);
	dongle->available = 0;
	pthread_mutex_unlock(&dongle->mutex);
	print_log_msg(data, coder->id, "has taken a dongle");
	return (0);
}

void	release_dongle(t_data *data, int dongle_idx)
{
	t_dongle	*dongle;

	dongle = &data->dongles[dongle_idx];
	pthread_mutex_lock(&dongle->mutex);
	dongle->available = 1;
	dongle->release_time = get_time_ms();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}
