#include "../includes/philo.h"

int main(int argc, char **argv)
{
	if (argc != 4 || argc != 5)
		ft_putstr_fd("\tusage: ./philo\n[number_of_philosophers]\t[time_to_die time_to_eat]\n[time_to_sleep]\n[number_of_times_each_philosopher_must_eat]\n", 2);
	return (0);
}