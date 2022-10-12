#include "../includes/philo.h"

static int	put_usage(void)
{
	ft_putstr_fd("\x1b[31musage\x1b[0m: ", 2);
	ft_putstr_fd("./philo  [number_of_philosophers]\n", 2);
	ft_putstr_fd("\t\t[time_to_die]\n\t\t[time_to_eat]\n", 2);
	ft_putstr_fd("\t\t[time_to_sleep]\n      (optional)", 2);
	ft_putstr_fd("[number_of_times_each_philosopher_must_eat]\n", 2);
	return (1);
}

static int	check_each_args(int argc, char **argv)
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