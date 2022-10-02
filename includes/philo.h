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
void	*ft_calloc(size_t n, size_t size);

typedef struct s_info	t_info;

typedef struct s_observer
{
	pthread_t	obs_thread;
	t_info		*info;
}				t_observer;

typedef struct s_philo
{
	pthread_t	phil_thread;
	int			num;
	int			right;
	int			left;
	t_info		*info;
}				t_philo;

typedef struct s_info
{
	int				num_philo;
	int				time_die;
	int				time_sleep;
	int				eat_times;
	pthread_mutex_t	fork;
	t_philo			*philo;
	t_observer		*obs;
}				t_info;

#endif
