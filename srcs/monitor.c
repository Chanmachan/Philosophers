#include "../includes/philo.h"

void	*monitor_philo(void *arg_philo)
{
	t_philo		*philo;
	t_info		*info;

	philo = (t_philo *)arg_philo;
	info = philo->info;
	philo->last_eat_time = get_time();
	while (1)
	{
		pthread_mutex_lock(&info->var_lock);
		if (info->status == true)
			break ;
		if ((size_t)info->time_die <= get_time() - philo->last_eat_time)
		{
			pthread_mutex_unlock(&info->var_lock);
			print_attitude(info, philo, DIED);
			if (info->num_philo == 1)
				pthread_mutex_unlock(&info->fork[philo->right]);
			break ;
		}
		pthread_mutex_unlock(&info->var_lock);
		usleep(1000);
	}
	pthread_mutex_unlock(&info->var_lock);
	return (NULL);
}

int	create_monitoring(t_philo *philo)
{
	pthread_t	obs;

	if (pthread_create(&obs, NULL, &monitor_philo, philo))
	{
		philo->info->error = 1;
		ft_putstr_fd("\x1b[31mError\x1b[0m: In monitor thread\n", 2);
		return (1);
	}
	if (pthread_detach(obs))
	{
		ft_putstr_fd("\x1b[31mError\x1b[0m: In monitor thread\n", 2);
		philo->info->error = 1;
		return (1);
	}
	return (0);
}
