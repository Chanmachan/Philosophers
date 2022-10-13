/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   attitude.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hommayunosuke <hommayunosuke@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 03:48:14 by hommayunosu       #+#    #+#             */
/*   Updated: 2022/10/14 03:48:15 by hommayunosu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	take_fork(t_philo *philo)
{
	t_info	*info;

	info = philo->info;
	pthread_mutex_lock(&info->fork[philo->right]);
	print_attitude(info, philo, FORK);
	pthread_mutex_lock(&info->fork[philo->left]);
	print_attitude(info, philo, FORK);
	return (0);
}

int	launch_eat(t_philo *philo)
{
	t_info	*info;

	info = philo->info;
	print_attitude(info, philo, EAT);
	precise_sleep(info->time_eat);
	if (info->num_philo == 1)
		return (0);
	pthread_mutex_lock(&info->var_lock);
	info->time_log = get_time();
	pthread_mutex_unlock(&info->var_lock);
	pthread_mutex_unlock(&info->fork[philo->right]);
	pthread_mutex_unlock(&info->fork[philo->left]);
	return (0);
}

int	start_sleep(t_philo *philo)
{
	t_info	*info;

	info = philo->info;
	print_attitude(info, philo, SLEEP);
	precise_sleep(info->time_sleep);
	pthread_mutex_lock(&info->var_lock);
	info->time_log = get_time();
	pthread_mutex_unlock(&info->var_lock);
	print_attitude(info, philo, THINK);
	return (0);
}

void	*loop_attitude(void *arg_philo)
{
	t_philo		*philo;
	t_info		*info;

	philo = (t_philo *)arg_philo;
	info = philo->info;
	if (philo->num % 2)
		precise_sleep(200);
	if (create_monitoring(philo))
		return (NULL);
	while (1)
	{
		take_fork(philo);
		if (info->num_philo == 1)
			return (NULL);
		launch_eat(philo);
		start_sleep(philo);
		pthread_mutex_lock(&info->atti);
		if (info->status == true)
		{
			pthread_mutex_unlock(&info->atti);
			return (NULL);
		}
		pthread_mutex_unlock(&info->atti);
	}
}
