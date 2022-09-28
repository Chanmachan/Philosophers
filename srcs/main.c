#include "../includes/philo.h"

int main(int argc, char **argv)
{
	if (argc != 6 && argc != 7)
		ft_putstr_fd("usage: ./philo  [number_of_philosophers]\n\t\t[time_to_die time_to_eat]\n\t\t[time_to_sleep]\n      (optional)[number_of_times_each_philosopher_must_eat]\n", 2);
	else
		printf("%s\n", argv[0]);
	return (0);
}