/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hilalipek <hilalipek@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 20:14:04 by hiipek            #+#    #+#             */
/*   Updated: 2025/07/16 03:27:55 by hilalipek        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_valid_num(char *str)
{
	int	i;

	if (!str || !*str)
		return (1);
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int	ft_atoi(char *str)
{
	int		i;
	long	result;

	i = 0;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == 43)
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] - 48;
		if ((result > INT_MAX))
			return (-1);
		i++;
	}
	return ((int)result);
}

long long	get_timestamp(void)
{
	struct timeval	time;
	long long		timestamp;

	gettimeofday(&time, NULL);
	timestamp = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (timestamp);
}

void	advanced_usleep(long long duration_time, t_data *data)
{
	long long	start_time;

	start_time = get_timestamp();
	while ((get_timestamp() - start_time < duration_time))
	{
		if (control_dead(data))
			return ;
		usleep(100);
	}
}

void	print_status(t_philo *philo, char *msg)
{
	long	now;

	pthread_mutex_lock(&philo->data->print_mutex);
	now = get_timestamp() - philo->data->start_time;
	if (!control_dead(philo->data))
		printf("%ld %d %s\n", now, philo->id, msg);
	pthread_mutex_unlock(&philo->data->print_mutex);
}
