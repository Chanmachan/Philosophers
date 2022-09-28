#include "../includes/philo.h"

void	init_info(t_info *info, int argc, char **argv)
{
	info->num_philo = ft_atoi(argv[1]);
	info->time_die= ft_atoi(argv[2]);
	info->time_sleep = ft_atoi(argv[3]);
	if (argc == 5)
		info->eat_times = ft_atoi(argv[4]);
	else
		info->eat_times = 0;
	printf("%d\n", info->num_philo);
	printf("%d\n", info->time_die);
	printf("%d\n", info->time_sleep);
	printf("%d\n", info->eat_times);
}

int main(int argc, char **argv)
{
	t_info	info;
	if (argc != 4 && argc != 5)
		ft_putstr_fd("usage: ./philo  [number_of_philosophers]\n\t\t[time_to_die time_to_eat]\n\t\t[time_to_sleep]\n      (optional)[number_of_times_each_philosopher_must_eat]\n", 2);
	else
		init_info(&info, argc, argv);
	return (0);
}
