/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icoman <icoman@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 09:46:46 by icoman            #+#    #+#             */
/*   Updated: 2026/08/05 09:47:03 by icoman           ###   ########.fr       */
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

typedef struct s_heap_node
{
	int		coder_id;
	long	priority;
}	t_heap_node;

typedef struct s_heap
{
	t_heap_node	*nodes;
	int			size;
	int			capacity;
}	t_heap;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				available;
	long			release_time;
	t_heap			wait_queue;
}	t_dongle;

struct	s_data;

typedef struct s_coder
{
	int				id;
	int				compile_count;
	long			last_compile;
	long			deadline;
	pthread_t		thread;
	struct s_data	*data;
	int				left_dongle;
	int				right_dongle;
}	t_coder;

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

int			parse_args(int argc, char **argv, t_data *data);
int			init_data(t_data *data);
void		set_initial_times(t_data *data);

long		get_time_ms(void);
void		smart_sleep(long duration_ms, t_data *data);
long		elapsed_ms(t_data *data);
int			check_sim_stop(t_data *data);
void		set_sim_stop(t_data *data);

void		log_action(t_data *data, int coder_id, char *action);
void		log_burnout(t_data *data, int coder_id);

void		heap_init(t_heap *heap, int capacity);
void		heap_push(t_heap *heap, int coder_id, long priority);
t_heap_node	heap_pop(t_heap *heap);
t_heap_node	heap_peek(t_heap *heap);
int			heap_is_empty(t_heap *heap);
void		heap_destroy(t_heap *heap);
void		swap_nodes(t_heap_node *a, t_heap_node *b);
int			compare_nodes(t_heap_node a, t_heap_node b);
void		min_heapify(t_heap *heap, int i);

int			acquire_dongle(t_data *data, t_coder *coder, int dongle_idx);
void		release_dongle(t_data *data, int dongle_idx);
void		broadcast_all_dongles(t_data *data);

void		*coder_routine(void *arg);
void		*monitor_routine(void *arg);
void		cleanup(t_data *data);

#endif
