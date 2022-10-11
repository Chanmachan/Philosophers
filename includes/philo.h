#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>
# include <stdbool.h>

# define FORK 1
# define EAT 2
# define SLEEP 3
# define THINK 4
# define DIED 5

void	ft_putstr_fd(char *s, int fd);
int		ft_atoi(const char *str);
void	*ft_calloc(size_t n, size_t size);

typedef struct s_info	t_info;
typedef struct s_philo	t_philo;

typedef struct s_observer
{
	int			num;
	pthread_t	obs_thread;
	t_info		*info;
	t_philo		*philo;
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
	int				time_eat;
	int				time_sleep;
	int				eat_times;
	size_t			last_eat_time;
	size_t			time_log;
	size_t			start_time;
	int				count_eat;
	int				status;
	pthread_mutex_t	atti;
	pthread_mutex_t	var_lock;
	pthread_mutex_t	status_lock;
	pthread_mutex_t	fork[200];
	t_philo			*philo;
	t_observer		*obs;
}				t_info;

#endif
