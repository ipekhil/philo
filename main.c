#include "philo.h"

static int	init_data(t_data *data,char **argv, int argc)
{
	if (!data)
		return (1);
	
	data->philo_count = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
	data->dead_flag = 0;
	data->start_time = get_timestamp();
	return (0);
}

static int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->philo_count);
	if (!data->philos)
		return (1);
	i = 0;
	while (i < data->philo_count)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = data->start_time;
		data->philos[i].data = data;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->philo_count];
		pthread_mutex_init(&data->philos[i].meal_lock, NULL);
		i++;
	}
	return (0);
}

static int is_valid_num(char *str)
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
static int validate_arguments(int argc, char **argv)
{
	int	i;
	int	num;

	if (argc != 5 && argc != 6)
		return (1);
	i = 0;
	while (++i < argc)
	{
		if (is_valid_num(argv[i]))
			return (1);
		num = ft_atoi(argv[i]);
		if (num <= 0)
			return (1);
	}
	return (0);
}
int main(int argc, char **argv)
{
	t_data	data;

	if (validate_arguments(argc, argv) != 0)
		return 1;
	if (init_data(&data, argc, argv) != 0)
		return 1;
	if (init_philos(&data) != 0)
		return 1;
	if (start_simulation(&data) != 0)
		return 1;
	cleanup(&data);
    return 0;
}
