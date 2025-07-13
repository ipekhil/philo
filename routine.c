#include "philo.h"

void	print_status(t_philo *philo, char *msg)
{
	long	now;

	pthread_mutex_lock(&philo->data->print_mutex);
	now = get_timestamp() - philo->data->start_time;
	if (!control_dead(philo->data))
		printf("%ld %d %s\n", now, philo->id, msg);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

void	*philos_life_cycle(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!control_dead(philo->data))
	
	{
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->right_fork);
			print_status(philo, "has taken a fork");
			if (control_dead(philo->data))
			{
				pthread_mutex_unlock(philo->right_fork);
				return (NULL);
			}
			pthread_mutex_lock(philo->left_fork);
			print_status(philo, "has taken a fork");
		}
		else
		{
			pthread_mutex_lock(philo->left_fork);
			print_status(philo, "has taken a fork");
			if (control_dead(philo->data))
			{
				pthread_mutex_unlock(philo->left_fork);
				return (NULL);
			}
			pthread_mutex_lock(philo->right_fork);
			print_status(philo, "has taken a fork");
		}
		if (control_dead(philo->data))
		{
			pthread_mutex_unlock(philo->right_fork);
			pthread_mutex_unlock(philo->left_fork);
			return (NULL);
		}
		if (!control_dead(philo->data))
		{
			pthread_mutex_lock(&philo->meal_lock);
			philo->last_meal_time = get_timestamp();
			philo->meals_eaten++;
			pthread_mutex_unlock(&philo->meal_lock);
			print_status(philo, "is eating");
			advanced_usleep(philo->data->time_to_eat, philo->data);
			pthread_mutex_unlock(philo->right_fork);
			pthread_mutex_unlock(philo->left_fork);			
		}
		
		if (!control_dead(philo->data))
		{
			print_status(philo, "is sleeping");
			advanced_usleep(philo->data->time_to_sleep, philo->data);
		}
		
		if (!control_dead(philo->data))
			print_status(philo, "is thinking");
	}
	return (NULL);
}
bool	control_dead(t_data *data)
{
	bool status;

	pthread_mutex_lock(&data->death_check_mutex);
	status = data->dead_flag;	
	pthread_mutex_unlock(&data->death_check_mutex);
	return (status);
}

void	*monitor_philos(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (!control_dead(data))
	{
		i = 0;
		while (i < data->philo_count)
		{
			pthread_mutex_lock(&data->philos[i].meal_lock);
			if(get_timestamp() - data->philos[i].last_meal_time >= data->time_to_die)
			{
				pthread_mutex_lock(&data->death_check_mutex);
				data->dead_flag = true;
				pthread_mutex_unlock(&data->death_check_mutex);
				pthread_mutex_lock(&data->print_mutex);
				printf("%lld %d died\n", get_timestamp() - data->start_time, data->philos[i].id);
				pthread_mutex_unlock(&data->print_mutex);
				pthread_mutex_unlock(&data->philos[i].meal_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&data->philos[i].meal_lock);
			i++;
		}
		if (data->must_eat > 0)
		{
			if (check_all_eaten(data))
				return (NULL);				
		}
		usleep(1000);
	}
	return (NULL);
}

