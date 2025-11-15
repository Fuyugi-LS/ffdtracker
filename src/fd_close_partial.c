/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_close_partial.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 22:58:43 by nsongsit          #+#    #+#             */
/*   Updated: 2025/11/15 22:58:44 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdtracker.h"
#include "fdtracker_utils.h"

static void	fd_close(t_fd *data, int32_t fd)
{
	uint64_t	i;

	i = 0;
	while (i < MAX_FD_TRACKER)
	{
		if (data[i] == fd)
		{
			close(data[i]);
			data[i] = -1;
		}
		i++;
	}
}

void	fd_close_partial(size_t size, int fd, ...)
{
	t_fd *const	data = fd_storage();
	va_list		va_lst;
	uint64_t	i;

	if (size < 1 || fd < 3)
		return ;
	fd_close(data, fd);
	if (size < 2)
		return ;
	i = 0;
	va_start(va_lst, fd);
	while (i < size - 1)
	{
		fd_close(data, va_arg(va_lst, int32_t));
		i++;
	}
	va_end(va_lst);
}
