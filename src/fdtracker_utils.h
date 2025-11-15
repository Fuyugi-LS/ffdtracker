/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdtracker_utils.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 22:58:27 by nsongsit          #+#    #+#             */
/*   Updated: 2025/11/15 22:58:27 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDTRACKER_UTILS_H
# define FDTRACKER_UTILS_H

# include <stdarg.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>

typedef int32_t	t_fd;

enum e_error_types
{
	BUFFER_OVERFLOW,
	TOO_SMALL_MAX_TRACKER
};

t_fd	*fd_storage(void);
void	fd_perror(enum e_error_types flag);

#endif
