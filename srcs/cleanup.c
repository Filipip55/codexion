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
			heap_destroy(&data->dongles[i].wait_queue);
			i++;
		}
		free(data->dongles);
	}
	if (data->coders)
		free(data->coders);
	pthread_mutex_destroy(&data->log_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
}
