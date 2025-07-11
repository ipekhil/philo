#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdbool.h>

# define INT_MAX	2147483647
# define INT_MIN	-2147483648

typedef struct s_data t_data;

typedef struct s_philo
{
    int				id;
    int				meals_eaten;
    long long		last_meal_time;
    pthread_mutex_t meal_lock;
    pthread_t		thread_id;
    pthread_mutex_t	*left_fork;
    pthread_mutex_t	*right_fork;
    t_data			*data;
}   t_philo;

typedef struct s_data
{
    int				philo_count;
    int				time_to_die;
    int				time_to_eat;
    int				time_to_sleep;
    int				must_eat;

    bool			dead_flag;
    long long		start_time;

    pthread_mutex_t	*forks;
    pthread_mutex_t	print_mutex;
    pthread_mutex_t	death_check_mutex;

    t_philo			*philos;
}	t_data;

long long	get_timestamp(void);
void		custom_usleep(long long ms);
void		print_status(t_philo *philo, char *msg);
int			ft_atoi(const char *str);
int	error_exit(t_data *data, char *msg, int exit_code);

#endif