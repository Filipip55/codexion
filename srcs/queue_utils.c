/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icoman <icoman@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 14:00:00 by icoman            #+#    #+#             */
/*   Updated: 2026/08/17 14:41:08 by icoman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <string.h>

int	queue_is_empty(t_queue *queue)
{
	return (queue->size == 0);
}

void	queue_destroy(t_queue *queue)
{
	if (queue->nodes)
		free(queue->nodes);
}
