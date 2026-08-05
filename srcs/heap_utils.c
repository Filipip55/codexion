/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icoman <icoman@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 09:50:10 by icoman            #+#    #+#             */
/*   Updated: 2026/08/05 09:50:11 by icoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap_nodes(t_heap_node *a, t_heap_node *b)
{
	t_heap_node	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int	compare_nodes(t_heap_node a, t_heap_node b)
{
	if (a.priority < b.priority)
		return (-1);
	if (a.priority > b.priority)
		return (1);
	if (a.coder_id < b.coder_id)
		return (-1);
	if (a.coder_id > b.coder_id)
		return (1);
	return (0);
}

void	min_heapify(t_heap *heap, int i)
{
	int	left;
	int	right;
	int	smallest;

	left = 2 * i + 1;
	right = 2 * i + 2;
	smallest = i;
	if (left < heap->size
		&& compare_nodes(heap->nodes[left], heap->nodes[smallest]) < 0)
		smallest = left;
	if (right < heap->size
		&& compare_nodes(heap->nodes[right], heap->nodes[smallest]) < 0)
		smallest = right;
	if (smallest != i)
	{
		swap_nodes(&heap->nodes[i], &heap->nodes[smallest]);
		min_heapify(heap, smallest);
	}
}

void	heap_destroy(t_heap *heap)
{
	if (heap->nodes)
		free(heap->nodes);
}
