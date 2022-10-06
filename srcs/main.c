#include "../includes/philo.h"

int	take_fork(t_philo *philo)
{
	t_info	*info;
	struct timeval	tv;

	info = philo->info;
	pthread_mutex_lock(&info->fork[philo->right]);
	pthread_mutex_lock(&info->atti);
	gettimeofday(&tv, NULL);
	info->time_log = tv.tv_usec;
	printf("%d\t%d has taken a fork\n", info->time_log, philo->right);
	pthread_mutex_unlock(&info->atti);
	pthread_mutex_lock(&info->fork[philo->left]);
	pthread_mutex_lock(&info->atti);
	gettimeofday(&tv, NULL);
	info->time_log = tv.tv_usec;
	printf("%d\t%d has taken a fork\n", info->time_log, philo->left);
	pthread_mutex_unlock(&info->atti);
	return (0);
}

int	launch_eat(t_philo *philo)
{
	t_info	*info;
	struct timeval	tv;

	info = philo->info;
	pthread_mutex_lock(&info->atti);
	gettimeofday(&tv, NULL);
	info->time_log = tv.tv_usec;
	printf("%d\t%d is eating", info->time_log, philo->num);
	pthread_mutex_unlock(&info->atti);
	return (0);
}

void	*loop_attitude(void *arg_philo)
{
	t_philo	*philo;
//	t_info	*info;

	philo = arg_philo;
	while (1)
	{
		if (take_fork(philo))
			return (NULL);
		if (launch_eat(philo))
			return (NULL);

	}
}

void	*do_something(void *arg_philo)
{
	static int i;
	t_philo *philo;

	philo = (t_philo *)arg_philo;
	i++;
	printf("%d, %d\n", i, philo->num);
	return (arg_philo);
}

int	prepare_table(t_info *info)
{
	t_observer	*obs;
	t_philo		*philo;
	int			i;

	philo = info->philo;
	obs = info->obs;
	i = 0;
	while (i < info->num_philo)
	{
		if (pthread_create(&philo[i].phil_thread, NULL, loop_attitude, &philo[i]))
			return (1);
		if (pthread_create(&obs[i].obs_thread, NULL, do_something, &obs[i]))
			return (1);
		i++;
	}
	return (0);
}

int	init_obs(t_info *info)
{
	int	i;

	info->obs = malloc(sizeof(t_observer));
	i = 0;
	while (i < info->num_philo)
	{
		info->obs[i].info = info;
		i++;
	}
	return (0);
}

int	init_philo(t_info *info)
{
	int	i;

	info->philo = malloc(sizeof(t_philo));
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
	info->time_sleep = ft_atoi(argv[3]);
	if (argc == 5)
		info->eat_times = ft_atoi(argv[4]);
	else
		info->eat_times = 0;
	info->time_log = 0;
	pthread_mutex_init(&info->atti, NULL);
	i = -1;
	while (++i < info->num_philo)
		pthread_mutex_init(&info->fork[i], NULL);
	init_philo(info);
	init_obs(info);
//	printf("%d\n", info->num_philo);
//	printf("%d\n", info->time_die);
//	printf("%d\n", info->time_sleep);
//	printf("%d\n", info->eat_times);
}

int main(int argc, char **argv)
{
	t_info	info;
	int		i;

	if (argc != 4 && argc != 5)
	{
		ft_putstr_fd("usage: ./philo  [number_of_philosophers]\n\t\t[time_to_die time_to_eat]\n\t\t[time_to_sleep]\n      (optional)[number_of_times_each_philosopher_must_eat]\n", 2);
		return (1);
	}
	else
		init_info(&info, argc, argv);
	prepare_table(&info);
	i = -1;
	while (++i < info.num_philo)
		pthread_mutex_destroy(&info.fork[i]);
	return (0);
}
