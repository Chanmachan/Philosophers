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

typedef struct s_info	t_info;
typedef struct s_philo	t_philo;

typedef struct s_philo
{
	pthread_t	phil_thread;
	int			num;
	int			right;
	int			left;
	size_t		last_eat_time;
	int			count_eat;
	t_info		*info;
	int			eat_flag;
}				t_philo;

typedef struct s_info
{
	int				error;
	int				num_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				eat_times;
	size_t			time_log;
	size_t			start_time;
	int				status;
	pthread_mutex_t	atti;
	pthread_mutex_t	var_lock;
	pthread_mutex_t	fork[200];
	t_philo			*philo;
}				t_info;

//main.c
size_t	get_time(void);
void	precise_sleep(size_t sleep_time);
void	print_attitude(t_info *info, t_philo *philo, int num);

//utils.c
void	ft_putstr_fd(char *s, int fd);
int		ft_isdigit(int c);
int		ft_atoi(const char *str);

//init.c
int		init_info(t_info *info, int argc, char **argv);

//valid_arg.c
int		valid_arg(int argc, char **argv);

//attitude.c
int		take_fork(t_philo *philo);
int		launch_eat(t_philo *philo);
int		start_sleep(t_philo *philo);
void	*loop_attitude(void *arg_philo);

//monitor.c
void	*monitor_philo(void *arg_philo);
int		create_monitoring(t_philo *philo);

//philo_utils.c
size_t	get_time(void);
void	precise_sleep(size_t sleep_time);
void	all_free(t_info *info);
void	destroy_all_mutex(t_info *info);

#endif
