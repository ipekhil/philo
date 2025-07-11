#include "philo.h"

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

int	error_exit(t_data *data, char *msg, int exit_code)
{
	if (msg)
	{
		pthread_mutex_lock(&data->print_mutex);
		printf("%s\n", msg);
		pthread_mutex_unlock(&data->print_mutex);
	}
	if (data)
		cleanup(data);
	return (exit_code);
}
