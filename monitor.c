/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hilalipek <hilalipek@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 02:25:55 by hilalipek         #+#    #+#             */
/*   Updated: 2025/07/16 02:37:21 by hilalipek        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_all_eaten(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_lock(&data->philos[i].meal_lock);
		if (data->philos[i].meals_eaten < data->must_eat)
		{
			pthread_mutex_unlock(&data->philos[i].meal_lock);
			return (0);
		}
		pthread_mutex_unlock(&data->philos[i].meal_lock);
		i++;
	}
	return (1);
}

bool	control_dead(t_data *data)
{
	bool	status;

	pthread_mutex_lock(&data->death_check_mutex);
	status = data->dead_flag;
	pthread_mutex_unlock(&data->death_check_mutex);
	return (status);
}

static int	check_philo_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_lock);
	if (get_timestamp() - philo->last_meal_time > philo->data->time_to_die)
	{
		pthread_mutex_lock(&philo->data->death_check_mutex);
		philo->data->dead_flag = true;
		pthread_mutex_unlock(&philo->data->death_check_mutex);
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%lld %d died\n", get_timestamp() - \
				philo->data->start_time, philo->id);
		pthread_mutex_unlock(&philo->data->print_mutex);
		pthread_mutex_unlock(&philo->meal_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->meal_lock);
	return (0);
}

void	*monitor_philos(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (!control_dead(data))
	{
		if (data->must_eat > 0 && check_all_eaten(data))
		{
			pthread_mutex_lock(&data->death_check_mutex);
			data->dead_flag = true;
			pthread_mutex_unlock(&data->death_check_mutex);
			pthread_mutex_lock(&data->print_mutex);
			pthread_mutex_unlock(&data->print_mutex);
			return (NULL);
		}
		i = 0;
		while (i < data->philo_count)
		{
			if (check_philo_death(&data->philos[i]))
				return (NULL);
			i++;
		}
		usleep(100);
	}
	return (NULL);
}
