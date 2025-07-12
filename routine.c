#include "philo.h"

void	print_status(t_philo *philo, char *msg)
{
	long	now;

	pthread_mutex_lock(&philo->data->print_mutex);
	now = get_timestamp() - philo->data->start_time;
	if (!philo->data->dead_flag)
		printf("%ld %d %s\n", now, philo->id + 1, msg);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void	*philos_life_cycle(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!philo->data->dead_flag)
	
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a left fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a right fork");
		philo->last_meal_time = get_timestamp();
		print_status(philo, "is eating");
		philo->meals_eaten++;
		advanced_usleep(philo->data->time_to_eat, philo->data);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);

		print_status(philo, "is sleeping");
		advanced_usleep(philo->data->time_to_sleep, philo->data);

		print_status(philo, "is thinking");
	}
	return (NULL);
}

void	*monitor_philos(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (!data->dead_flag)
	{
		i = 0;
		while (i < data->philo_count)
		{
			pthread_mutex_lock(&data->philos[i].last_meal_time);
			if(get_timestamp() - data->philos[i].last_meal_time >= data->time_to_die) // > or >= ?
			{
				data->dead_flag = true;
				pthread_mutex_lock(&data->print_mutex);
				printf("%lld %d died\n", get_timestamp() - data->start_time, data->philos[i].id);
				pthread_mutex_unlock(&data->print_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&data->philos[i].last_meal_time);
			i++;
		}	
	}
}