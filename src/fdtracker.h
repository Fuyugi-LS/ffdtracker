/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdtracker.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 22:58:30 by nsongsit          #+#    #+#             */
/*   Updated: 2025/11/15 22:58:31 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDTRACKER_H
# define FDTRACKER_H

# include <unistd.h>

# define MAX_FD_TRACKER 1024

void	fd_register(size_t size, int fd, ...);
void	fd_close_all(void);
void	fd_close_partial(size_t size, int fd, ...);

#endif
