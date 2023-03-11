/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hommayunosuke <hommayunosuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 03:49:06 by hommayunosu       #+#    #+#             */
/*   Updated: 2022/10/14 03:49:07 by hommayunosu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_time(void)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return (tp.tv_sec * 1000 + tp.tv_usec / 1000);
}

void	precise_sleep(size_t sleep_time)
{
	size_t	end_time;

	end_time = get_time() + sleep_time;
	while (get_time() < end_time)
		usleep(100);
}

void	all_free(t_info *info)
{
	free(info->philo);
	free(info->fork);
}

void	destroy_all_mutex(t_info *info)
{
	int		i;

	i = -1;
	while (++i < info->num_philo)
		pthread_mutex_destroy(&info->fork[i]);
	pthread_mutex_destroy(&info->atti);
	pthread_mutex_destroy(&info->var_lock);
}
