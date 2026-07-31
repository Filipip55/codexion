#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long	elapsed_ms(t_data *data)
{
	return (get_time_ms() - data->start_time);
}

int	check_sim_stop(t_data *data)
{
	int	stop;

	pthread_mutex_lock(&data->stop_mutex);
	stop = data->sim_stop;
	pthread_mutex_unlock(&data->stop_mutex);
	return (stop);
}

void	set_sim_stop(t_data *data)
{
	pthread_mutex_lock(&data->stop_mutex);
	data->sim_stop = 1;
	pthread_mutex_unlock(&data->stop_mutex);
}

void	smart_sleep(long duration_ms, t_data *data)
{
	long	start;

	start = get_time_ms();
	while (!check_sim_stop(data))
	{
		if (get_time_ms() - start >= duration_ms)
			break ;
		usleep(500);
	}
}
