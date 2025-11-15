/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_register.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 22:58:37 by nsongsit          #+#    #+#             */
/*   Updated: 2025/11/15 22:58:38 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdtracker.h"
#include "fdtracker_utils.h"

static void	fd_integrity(void)
{
	if (MAX_FD_TRACKER < 16)
		fd_perror(TOO_SMALL_MAX_TRACKER);
}

static void	fd_assign(t_fd	*data, int32_t fd)
{
	uint64_t	last_fd;

	last_fd = 0;
	while (data[last_fd] != -1 && last_fd < MAX_FD_TRACKER)
		last_fd++;
	if (last_fd < MAX_FD_TRACKER)
		data[last_fd] = fd;
	else
	{
		close(fd);
		fd_perror(BUFFER_OVERFLOW);
	}
}

void	fd_register(size_t size, int fd, ...)
{
	t_fd *const	data = fd_storage();
	va_list		va_lst;
	uint64_t	i;

	fd_integrity();
	if (size < 1 || fd < 3)
		return ;
	fd_assign(data, fd);
	if (size < 2)
		return ;
	va_start(va_lst, fd);
	i = 0;
	while (i < size - 1)
	{
		fd_assign((t_fd *)data, va_arg(va_lst, int32_t));
		i++;
	}
	va_end(va_lst);
}
