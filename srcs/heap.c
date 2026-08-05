/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icoman <icoman@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 09:50:06 by icoman            #+#    #+#             */
/*   Updated: 2026/08/05 09:50:07 by icoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heap_init(t_heap *heap, int capacity)
{
	heap->nodes = malloc(sizeof(t_heap_node) * capacity);
	if (!heap->nodes)
		return ;
	heap->size = 0;
	heap->capacity = capacity;
}

void	heap_push(t_heap *heap, int coder_id, long priority)
{
	int	i;
	int	parent;

	if (heap->size >= heap->capacity)
		return ;
	i = heap->size;
	heap->size++;
	heap->nodes[i].coder_id = coder_id;
	heap->nodes[i].priority = priority;
	while (i != 0)
	{
		parent = (i - 1) / 2;
		if (compare_nodes(heap->nodes[i], heap->nodes[parent]) < 0)
		{
			swap_nodes(&heap->nodes[i], &heap->nodes[parent]);
			i = parent;
		}
		else
			break ;
	}
}

t_heap_node	heap_pop(t_heap *heap)
{
	t_heap_node	root;

	if (heap->size <= 0)
	{
		root.coder_id = -1;
		root.priority = -1;
		return (root);
	}
	if (heap->size == 1)
	{
		heap->size--;
		return (heap->nodes[0]);
	}
	root = heap->nodes[0];
	heap->nodes[0] = heap->nodes[heap->size - 1];
	heap->size--;
	min_heapify(heap, 0);
	return (root);
}

t_heap_node	heap_peek(t_heap *heap)
{
	t_heap_node	root;

	if (heap->size <= 0)
	{
		root.coder_id = -1;
		root.priority = -1;
		return (root);
	}
	return (heap->nodes[0]);
}

int	heap_is_empty(t_heap *heap)
{
	return (heap->size == 0);
}
