#include "../includes/philo.h"

size_t    get_time(void)
{
	struct timeval    tp;

	gettimeofday(&tp, NULL);
	return (tp.tv_sec * 1000 + tp.tv_usec / 1000);
}

void    precise_sleep(size_t sleep_time)
{
	size_t    end_time;

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
		return (-1);
	if (pthread_detach(obs))
		return (-1);
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
		printf("hoge\n");
	}
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

int	init_obs(t_info *info)
{
	int	i;

	info->obs = malloc(sizeof(t_observer) * info->num_philo);
	if (info->obs == NULL)
		return (-1);
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
	if (info->philo == NULL)
		return (-1);
	i = 0;
	while (i < info->num_philo)
	{
		info->philo[i].num = i;
		info->philo[i].right = i;
		info->philo[i].left = (i + 1) % info->num_philo;
		info->philo[i].last_eat_time = 0;
		info->philo[i].count_eat = 0;
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
	info->status = false;
	pthread_mutex_init(&info->atti, NULL);
	pthread_mutex_init(&info->var_lock, NULL);
	pthread_mutex_init(&info->status_lock, NULL);
	i = -1;
	while (++i < info->num_philo)
		pthread_mutex_init(&info->fork[i], NULL);
	init_philo(info);
	init_obs(info);
}

int	put_usage(void)
{
	ft_putstr_fd("\x1b[31musage\x1b[0m: ", 2);
	ft_putstr_fd("./philo  [number_of_philosophers]\n", 2);
	ft_putstr_fd("\t\t[time_to_die]\n\t\t[time_to_eat]\n", 2);
	ft_putstr_fd("\t\t[time_to_sleep]\n      (optional)", 2);
	ft_putstr_fd("[number_of_times_each_philosopher_must_eat]\n", 2);
	return (1);
}

int	check_each_args(int argc, char **argv)
{
	int	i;
	int	philo_num;
	int	sec;

	philo_num = ft_atoi(argv[1]);
	if (philo_num > 200)
	{
		ft_putstr_fd("\x1b[31mError\x1b[0m: Invalid Arguments\n", 2);
		return (1);
	}
	i = 1;
	if (argc == 6)
		argc -= 1;
	while (++i < argc)
	{
		sec = ft_atoi(argv[i]);
		if (sec < 60)
		{
			ft_putstr_fd("\x1b[31mError\x1b[0m: Invalid Arguments\n", 2);
			return (1);
		}
	}
	return (0);
}

int	valid_arg(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc != 5 && argc != 6)
		return (put_usage());
	i = 0;
	while (++i < argc)
	{
		j = 0;
		while (argv[i][j] != '\0')
		{
			if (ft_isdigit(argv[i][j++]) == 0)
			{
				ft_putstr_fd("\x1b[31mError\x1b[0m: Invalid Arguments\n", 2);
				return (1);
			}
		}
	}
	if (check_each_args(argc, argv))
		return (1);
	return (0);
}

int main(int argc, char **argv)
{
	t_info	info;
	int		i;

	if (valid_arg(argc, argv))
		return (1);
	init_info(&info, argc, argv);
	prepare_table(&info);
	i = -1;
	while (++i < info.num_philo)
		pthread_mutex_destroy(&info.fork[i]);
	pthread_mutex_destroy(&info.atti);
	pthread_mutex_destroy(&info.var_lock);
//	system("leaks -q philo");
	return (0);
}
