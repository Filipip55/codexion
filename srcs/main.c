/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icoman <icoman@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 09:49:55 by icoman            #+#    #+#             */
/*   Updated: 2026/08/17 11:48:24 by icoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	create_coder_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_coders)
	{
		if (pthread_create(&data->coders[i].thread, NULL,
				coder_routine, &data->coders[i]) != 0)
		{
			fprintf(stderr, "Error: Failed to create coder thread\n");
			set_sim_stop(data);
			return (1);
		}
		i++;
	}
	return (0);
}

static void	wait_threads(t_data *data, pthread_t monitor)
{
	int	i;

	i = 0;
	while (i < data->num_coders)
	{
		if (data->coders[i].thread)
			pthread_join(data->coders[i].thread, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
}

/* Punto d'entrata di codexion, in ordine:
	- parsing degli argomenti
	- init della struct contenente i dati comuni
	- salva il tempo di inizio e si settano le deadline dei coder
	- crea il thread monitor e i coder
	- mette in pausa il main e aspetta l'esecuzione del monitor
	- esegue pulizie finali della memoria */
int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	monitor_thread;

	memset(&data, 0, sizeof(t_data));
	if (parse_args(argc, argv, &data))
		return (1);
	if (init_data(&data))
		return (1);
	data.start_time = get_time_ms();
	set_initial_times(&data);
	if (pthread_create(&monitor_thread, NULL, monitor_routine, &data) != 0)
	{
		fprintf(stderr, "Error: Failed to create monitor thread\n");
		cleanup(&data);
		return (1);
	}
	create_coder_threads(&data);
	wait_threads(&data, monitor_thread);
	cleanup(&data);
	return (0);
}
