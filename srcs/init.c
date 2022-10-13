#include "../includes/philo.h"

static int	init_philo(t_info *info)
{
	int	i;

	info->philo = malloc(sizeof(t_philo) * info->num_philo);
	if (info->philo == NULL)
	{
		ft_putstr_fd("\x1b[31mError\x1b[0m: malloc error", 2);
		return (1);
	}
	i = 0;
	while (i < info->num_philo)
	{
		info->philo[i].num = i;
		info->philo[i].right = i;
		info->philo[i].left = (i + 1) % info->num_philo;
		info->philo[i].last_eat_time = 0;
		info->philo[i].count_eat = 0;
		info->philo[i].info = info;
		info->philo[i].eat_flag = false;
		i++;
	}
	return (0);
}

int	init_info(t_info *info, int argc, char **argv)
{
	int	i;

	info->error = 0;
	info->num_philo = ft_atoi(argv[1]);
	info->time_die = ft_atoi(argv[2]);
	info->time_eat = ft_atoi(argv[3]);
	info->time_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		info->eat_times = ft_atoi(argv[5]);
	else
		info->eat_times = -1;
	info->time_log = 0;
	info->status = false;
	pthread_mutex_init(&info->atti, NULL);
	pthread_mutex_init(&info->var_lock, NULL);
	i = -1;
	while (++i < info->num_philo)
		pthread_mutex_init(&info->fork[i], NULL);
	if (init_philo(info))
		return (1);
	return (0);
}
