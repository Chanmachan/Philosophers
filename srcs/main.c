#include "../includes/philo.h"

long		get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_attitude(t_info *info, t_philo *philo, int num)
{
	pthread_mutex_lock(&info->atti);
	if (num == FORK && info->status == false)
		printf("%ld\t%d has taken a fork\n", get_time(), philo->num);
	else if (num == EAT && info->status == false)
	{
		info->count_eat++;
		info->time_log = get_time();
		printf("%ld\t%d is eating\n", info->time_log, philo->num);
	}
	else if (num == SLEEP && info->status == false)
		printf("%ld\t%d is sleeping\n", get_time(), philo->num);
	else if (num == THINK && info->status == false)
		printf("%ld\t%d is thinking\n", get_time(), philo->num);
	else if (num == DIED && info->status == false)
	{
		info->status = true;
		printf("%ld\t%d died\n", get_time(), philo->num);
	}
	pthread_mutex_unlock(&info->atti);
}

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
//
//int	launch_eat(t_philo *philo)
//{
//	t_info	*info;
//
//	info = philo->info;
//	info->time_log = get_time();
//	print_attitude(info, philo, EAT);
//	pthread_mutex_unlock(&info->fork[philo->right]);
//	pthread_mutex_unlock(&info->fork[philo->left]);
//	return (0);
//}

int	launch_eat(t_philo *philo)
{
	t_info	*info;
	long	start_eat_time;

	info = philo->info;
	print_attitude(info, philo, EAT);
	start_eat_time = get_time();
	while (1)
	{
		if (info->time_eat < get_time() - start_eat_time)
		{
//			print_attitude(info, philo, EAT);
			break ;
		}
		usleep(200);
	}
	pthread_mutex_unlock(&info->fork[philo->right]);
	pthread_mutex_unlock(&info->fork[philo->left]);
	return (0);
}

int	start_sleep(t_philo *philo)
{
	t_info	*info;
	long	start_sleep_time;

	info = philo->info;
	start_sleep_time = get_time();
	print_attitude(info, philo, SLEEP);
	while (1)
	{
		if (info->time_sleep < get_time() - start_sleep_time)
		{
//			print_attitude(info, philo, SLEEP);
			break ;
		}
		usleep(200);
	}
	print_attitude(info, philo, THINK);
	return (0);
}

int	and_think(t_philo *philo)
{
	t_info *info;

	info = philo->info;
	pthread_mutex_lock(&info->atti);
	if (info->status == true)
	{
		pthread_mutex_unlock(&info->atti);
		return (1);
	}
	pthread_mutex_unlock(&info->atti);
	print_attitude(info, philo, THINK);
	return (0);
}

void	*loop_attitude(void *arg_philo)
{
	t_philo	*philo;
	t_info	*info;

	philo = (t_philo *)arg_philo;
	info = philo->info;
	if (philo->num % 2 == 0)
		usleep(200);
	while (1)
	{
		take_fork(philo);
		launch_eat(philo);
		start_sleep(philo);
		pthread_mutex_lock(&info->atti);
		if (info->status == true)
		{
			pthread_mutex_unlock(&info->atti);
			return (0);
		}
		pthread_mutex_unlock(&info->atti);
	}
}

void	*monitor_philo(void *arg_obs)
{
	t_observer	*obs;
	t_info		*info;
	t_philo		*philo;

	obs = (t_observer *)arg_obs;
	info = obs->info;
	philo = obs->philo;
	while (1)
	{
//		pthread_mutex_lock(&info->monitor);
		if (info->eat_times >= info->count_eat)
		{
//			pthread_mutex_unlock(&info->monitor);
			break ;
		}
//		pthread_mutex_unlock(&info->monitor);
//		pthread_mutex_lock(&info->monitor);
		if (info->time_die < (get_time() - info->time_log))
		{
//			pthread_mutex_unlock(&info->monitor);
			print_attitude(info, philo, DIED);
			break ;
		}
//		pthread_mutex_unlock(&info->monitor);
		usleep(200);
	}
	return (NULL);
}

int	prepare_table(t_info *info)
{
	t_observer	*obs;
	t_philo		*philo;
	int			i;

	philo = info->philo;
	obs = info->obs;
	i = 0;
	info->time_log = get_time();
	while (i < info->num_philo)
	{
		if (pthread_create(&philo[i].phil_thread, NULL, &loop_attitude, &philo[i]))
			return (1);
		if (pthread_create(&obs[i].obs_thread, NULL, &monitor_philo, &obs[i]))
			return (1);
		i++;
	}
	return (0);
}

int	init_obs(t_info *info)
{
	int	i;

	info->obs = malloc(sizeof(t_observer) * info->num_philo);
	i = 0;
	while (i < info->num_philo)
	{
		info->obs[i].num = i;
		info->obs[i].info = info;
		info->obs[i].philo = info->philo;
		i++;
	}
	return (0);
}

int	init_philo(t_info *info)
{
	int	i;

	info->philo = malloc(sizeof(t_philo) * info->num_philo);
	i = 0;
	while (i < info->num_philo)
	{
		info->philo[i].num = i;
		info->philo[i].right = i;
		info->philo[i].left = (i + 1) % info->num_philo;
		info->philo[i].info = info;
		i++;
	}
	return (0);
}

void	init_info(t_info *info, int argc, char **argv)
{
	int	i;

	info->num_philo = ft_atoi(argv[1]);
	info->time_die= ft_atoi(argv[2]);
	info->time_eat= ft_atoi(argv[3]);
	info->time_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		info->eat_times = ft_atoi(argv[5]);
	else
		info->eat_times = -1;
	info->time_log = 0;
	info->count_eat = 0;
	info->status = false;
	pthread_mutex_init(&info->atti, NULL);
	pthread_mutex_init(&info->monitor, NULL);
	i = -1;
	while (++i < info->num_philo)
		pthread_mutex_init(&info->fork[i], NULL);
	init_philo(info);
	init_obs(info);
}

int main(int argc, char **argv)
{
	t_info	info;
	int		i;

	if (argc != 5 && argc != 6)
	{
		ft_putstr_fd("usage: ./philo  [number_of_philosophers]\n\t\t[time_to_die]\n\t\t[time_to_eat]\n\t\t[time_to_sleep]\n      (optional)[number_of_times_each_philosopher_must_eat]\n", 2);
		return (1);
	}
	else
		init_info(&info, argc, argv);
	prepare_table(&info);
	i = -1;
	while (++i < info.num_philo)
		pthread_mutex_destroy(&info.fork[i]);
	pthread_mutex_destroy(&info.atti);
	pthread_mutex_destroy(&info.monitor);
	i = -1;
	while (++i < info.num_philo)
	{
		pthread_detach(info.philo[i].phil_thread);
	}
	i = -1;
	while (++i < info.num_philo)
	{
		pthread_detach(info.obs[i].obs_thread);
	}
//	system("leaks -q philo");
	return (0);
}
