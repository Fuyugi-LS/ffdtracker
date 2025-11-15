/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_storage.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 22:58:34 by nsongsit          #+#    #+#             */
/*   Updated: 2025/11/15 22:58:35 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdtracker.h"
#include "fdtracker_utils.h"

t_fd	*fd_storage(void)
{
	static t_fd		data[MAX_FD_TRACKER + 1];
	static int32_t	start_state;
	int64_t			i;

	if (start_state != 0)
		return (data);
	i = 0;
	while (i < MAX_FD_TRACKER)
	{
		data[i] = -1;
		i++;
	}
	start_state = 1;
	return (data);
}
