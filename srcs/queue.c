/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icoman <icoman@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 14:00:00 by icoman            #+#    #+#             */
/*   Updated: 2026/08/17 14:54:14 by icoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <string.h>

void	queue_init(t_queue *queue, int capacity)
{
	queue->nodes = malloc(sizeof(t_queue_node) * capacity);
	if (!queue->nodes)
		return ;
	queue->size = 0;
	queue->capacity = capacity;
}

static int	compare_queue_nodes(t_queue_node a, t_queue_node b)
{
	if (a.priority < b.priority)
		return (-1);
	if (a.priority > b.priority)
		return (1);
	if (a.coder_id > b.coder_id)
		return (-1);
	if (a.coder_id < b.coder_id)
		return (1);
	return (0);
}

void	queue_push(t_queue *queue, int coder_id, long priority)
{
	t_queue_node	new_node;
	int				i;

	if (queue->size >= queue->capacity)
		return ;
	new_node.coder_id = coder_id;
	new_node.priority = priority;
	i = 0;
	while (i < queue->size)
	{
		if (compare_queue_nodes(new_node, queue->nodes[i]) > 0)
			break ;
		i++;
	}
	if (i < queue->size)
		memmove(&queue->nodes[i + 1], &queue->nodes[i],
			sizeof(t_queue_node) * (queue->size - i));
	queue->nodes[i] = new_node;
	queue->size++;
}

t_queue_node	queue_pop(t_queue *queue)
{
	t_queue_node	root;

	if (queue->size <= 0)
	{
		root.coder_id = -1;
		root.priority = -1;
		return (root);
	}
	queue->size--;
	return (queue->nodes[queue->size]);
}

t_queue_node	queue_peek(t_queue *queue)
{
	t_queue_node	root;

	if (queue->size <= 0)
	{
		root.coder_id = -1;
		root.priority = -1;
		return (root);
	}
	return (queue->nodes[queue->size - 1]);
}
