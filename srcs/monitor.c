#include "codexion.h"

void	broadcast_all_dongles(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_coders)
	{
		pthread_mutex_lock(&data->dongles[i].mutex);
		pthread_cond_broadcast(&data->dongles[i].cond);
		pthread_mutex_unlock(&data->dongles[i].mutex);
		i++;
	}
}

static int	check_burnout(t_data *data, int i)
{
	long	current_time;
	long	time_since_last_compile;

	current_time = elapsed_ms(data);
	time_since_last_compile = current_time - data->coders[i].last_compile;
	if (time_since_last_compile >= data->time_to_burnout)
	{
		log_burnout(data, data->coders[i].id);
		broadcast_all_dongles(data);
		return (1);
	}
	return (0);
}

static int	check_all_fed(t_data *data)
{
	int	all_fed;
	int	i;

	all_fed = 1;
	i = 0;
	while (i < data->num_coders)
	{
		if (data->coders[i].compile_count < data->compiles_required)
			all_fed = 0;
		i++;
	}
	if (all_fed)
	{
		set_sim_stop(data);
		broadcast_all_dongles(data);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (!check_sim_stop(data))
	{
		i = 0;
		while (i < data->num_coders)
		{
			if (check_burnout(data, i))
				return (NULL);
			i++;
		}
		if (check_all_fed(data))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
