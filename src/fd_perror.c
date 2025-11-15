/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_perror.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsongsit <nsongsit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 22:58:40 by nsongsit          #+#    #+#             */
/*   Updated: 2025/11/15 22:58:41 by nsongsit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdtracker.h"
#include "fdtracker_utils.h"

static void	print_small_max_tracker(void)
{
	write(2, "Errors exist in file descriptor tracker: ", 41);
	write(2, "Too small \"MAX_TRACKER\"\n", 24);
	write(2, "NOTE: The least \"MAX_TRACKER\" acceptable size is: 16\n", 53);
	write(2, "Error ID: 301\n", 14);
	exit(301);
}

static void	print_overflow(void)
{
	write(2, "Errors exist in file descriptor tracker: Buffer Overflow\n", 57);
	write(2, "Error ID: 300\n", 14);
	exit(300);
}

void	fd_perror(enum e_error_types flag)
{
	fd_close_all();
	if (flag == BUFFER_OVERFLOW)
		print_overflow();
	if (flag == TOO_SMALL_MAX_TRACKER)
		print_small_max_tracker();
}
