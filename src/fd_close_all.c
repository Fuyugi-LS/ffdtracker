/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_close_all.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 22:58:47 by nsongsit          #+#    #+#             */
/*   Updated: 2025/11/15 22:58:48 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdtracker.h"
#include "fdtracker_utils.h"

void	fd_close_all(void)
{
	t_fd *const	data = fd_storage();
	uint64_t	i;

	i = 0;
	while (i < MAX_FD_TRACKER)
	{
		if (data[i] >= 3)
		{
			close(data[i]);
			data[i] = -1;
		}
		i++;
	}
}
