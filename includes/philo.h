#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>

void	ft_putstr_fd(char *s, int fd);
int		ft_atoi(const char *str);

typedef struct s_info
{
	int	num_philo;
	int	time_die;
	int	time_sleep;
	int	eat_times;
}				t_info;

#endif
