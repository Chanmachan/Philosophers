#include "../includes/philo.h"

void	all_free(t_info *info)
{
	free(info->philo);
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
