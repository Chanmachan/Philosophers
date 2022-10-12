#include "../includes/philo.h"

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

void	print_attitude(t_info *info, t_philo *philo, int num)
{
	size_t	output_time;

	pthread_mutex_lock(&info->atti);
	pthread_mutex_lock(&info->var_lock);
	output_time = info->time_log - info->start_time;
	pthread_mutex_unlock(&info->var_lock);
	if (num == FORK && info->status == false)
		printf("%ld\t%d has taken a fork\n", output_time, philo->num + 1);
	else if (num == EAT && info->status == false)
	{
		pthread_mutex_lock(&info->var_lock);
		if (info->eat_times != -1)
			philo->count_eat++;
		philo->last_eat_time = get_time();
		if (info->eat_times <= philo->count_eat && info->eat_times != -1)
			info->status = true;
		pthread_mutex_unlock(&info->var_lock);
		printf("\x1b[32m%ld\t%d is eating\n\x1b[0m", output_time, philo->num + 1);
	}
	else if (num == SLEEP && info->status == false)
		printf("\x1b[35m%ld\t%d is sleeping\n\x1b[0m", output_time, philo->num + 1);
	else if (num == THINK && info->status == false)
		printf("\x1b[36m%ld\t%d is thinking\n\x1b[0m", output_time, philo->num + 1);
	else if (num == DIED && info->status == false)
	{
		pthread_mutex_lock(&info->var_lock);
		info->status = true;
		pthread_mutex_unlock(&info->var_lock);
		output_time = get_time() - info->start_time;
		printf("\x1b[31m%ld\t%d died\n\x1b[0m", output_time, philo->num + 1);
	}
	pthread_mutex_unlock(&info->atti);
}

int	prepare_table(t_info *info)
{
	t_philo		*philo;
	int			i;

	philo = info->philo;
	i = 0;
	info->time_log = get_time();
	philo->last_eat_time = info->time_log;
	info->start_time = get_time();
	while (i < info->num_philo)
	{
		if (pthread_create(&philo[i].phil_thread, NULL, &loop_attitude, &philo[i]))
			return (1);
		i++;
	}
	i = 0;
	while (i < info->num_philo)
	{
		if (pthread_join(philo[i].phil_thread, (void *)&philo[i]))
			return (1);
		i++;
	}
	return (0);
}

int	error_msg(void)
{
	ft_putstr_fd("\x1b[31mError\x1b[0m: malloc error\n", 2);
	return (1);
}

int main(int argc, char **argv)
{
	t_info	info;
	int		i;

	if (valid_arg(argc, argv))
		return (1);
	if (init_info(&info, argc, argv))
		return (1);
	if (prepare_table(&info))
	{
		if (info.error != 0)
			return (1);
		ft_putstr_fd("\x1b[31mError\x1b[0m: In \"pthread_create\"\n", 2);
		return (1);
	}
	if (info.error != 0)
	{
		ft_putstr_fd("\x1b[31mError\x1b[0m: ", 2);
		return (1);
	}
	i = -1;
	while (++i < info.num_philo)
		pthread_mutex_destroy(&info.fork[i]);
	pthread_mutex_destroy(&info.atti);
	pthread_mutex_destroy(&info.var_lock);
//	system("leaks -q philo");
	return (0);
}
