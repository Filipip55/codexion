/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icoman <icoman@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 09:46:46 by icoman            #+#    #+#             */
/*   Updated: 2026/08/20 20:18:58 by icoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>

/* Nodo della coda d'attesa */
typedef struct s_queue_node
{
	int		coder_id;
	long	priority;
}	t_queue_node;

/*  Size = dimensione corrente della lista
	capacity = dimensione massima della lista */
typedef struct s_queue
{
	t_queue_node	*nodes;
	int				size;
	int				capacity;
}	t_queue;

/* Ogni dongle ha una sua lista d'attesa, un mutex e una wait cond */
typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				available;
	long			release_time;
	t_queue			wait_queue;
}	t_dongle;

struct	s_data;

/* Tutti i coder condividono un t_data */
typedef struct s_coder
{
	int				id;
	int				compile_count;
	long			last_compile;
	long			deadline;
	pthread_mutex_t	state_mutex;
	pthread_t		thread;
	struct s_data	*data;
	int				left_dongle;
	int				right_dongle;
}	t_coder;

/* Contiene tutti i dati sulla simulazione,
	i mutex per l'accesso alle flag di print log msg
	e di fine simulazione */
typedef struct s_data
{
	int				num_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				compiles_required;
	long			dongle_cooldown;
	int				scheduler;
	long			start_time;
	int				sim_stop;
	int				all_done;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	stop_mutex;
	t_dongle		*dongles;
	t_coder			*coders;
}	t_data;

/* parsing e inizializzazione */
int				parse_args(int argc, char **argv, t_data *data);
int				init_data(t_data *data);
void			set_initial_times(t_data *data);

/* metodi per la gestione del tempo */
long			get_time_ms(void);
void			smart_sleep(long duration_ms, t_data *data);
long			elapsed_ms(t_data *data);
int				check_sim_stop(t_data *data);
void			set_sim_stop(t_data *data);

/* metodi per viasulizzazione dei msg */
void			print_log_msg(t_data *data, int coder_id, char *action);
void			log_burnout(t_data *data, int coder_id);

/* metodi per la gestione delle liste di attesa */
void			queue_init(t_queue *queue, int capacity);
void			queue_push(t_queue *queue, int coder_id, long priority);
t_queue_node	queue_pop(t_queue *queue);
t_queue_node	queue_peek(t_queue *queue);
int				queue_is_empty(t_queue *queue);
void			queue_destroy(t_queue *queue);

/* metodi per la gestione dei dongle */
int				acquire_dongle(t_data *data, t_coder *coder, int dongle_idx);
void			release_dongle(t_data *data, int dongle_idx);
void			broadcast_all_dongles(t_data *data);

/* metodi di routine e cleanup finale */
void			*coder_routine(void *arg);
void			*monitor_routine(void *arg);
void			cleanup(t_data *data);

#endif
