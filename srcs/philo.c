#include "../includes/philo.h"

void	ft_ending(t_philo *ph)
{
	int	i;

	i = -1;
	while (++i < ph[0].philo_amount)
	{
		pthread_mutex_lock(&ph->common->end);
		ph[i].ending_flag = END;
		pthread_mutex_unlock(&ph->common->end);
	}
}

int	check_every1_ate(t_philo *ph)
{
	int	i;
	int	meal_num_reached;

	i = -1;
	meal_num_reached = 0;
	if (ph[0].num_times_to_eat == 0)
		return (0);
	while (++i < ph[0].philo_amount)
	{
		pthread_mutex_lock(&ph->common->eat);
		if (ph[0].num_times_to_eat != -1 && ph[i].actual_meal >= ph[0].num_times_to_eat)
			meal_num_reached++;
		pthread_mutex_unlock(&ph->common->eat);
	}
	if (meal_num_reached == ph[0].philo_amount)
	{
		ft_ending(ph);
		return (1);
	}
	return (0);
}

int	search_deads(t_philo *ph)
{
	int	i;

	i = 0;
	while (i < ph[0].philo_amount)
	{
		pthread_mutex_lock(&ph->common->eat);
		if (ph[i].eating_flag == NOTEATING &&
		get_current_time() - ph[i].last_meal >= ph[i].time_to_die)
		{
			ft_ending(ph);
			print_timestamp("died", ph);
			pthread_mutex_unlock(&ph->common->eat);
			return (1);
		}
		i++;
		pthread_mutex_unlock(&ph->common->eat);
	}
	return (0);
}

void	*watcher_routine(void *pointer)
{
	t_philo	*ph;

	ph = (t_philo *)pointer;
	usleep(2000);

	while (1)
	{
		if (search_deads(ph) == 1 || check_every1_ate(ph) == 1)
			break ;
	}
	return (pointer);
}


void	destroy_mutex(char *s, t_philo *ph, pthread_mutex_t *fork)
{
	int	i;

	i = 0;
	printf("%s\n", s);
	while (i < ph[0].philo_amount)
	{
		pthread_mutex_destroy(&fork[i]);
		i++;
	}
}

int	ft_atoi(const char *str)
{
	int	i;
	int	neg;
	int	res;

	i = 0;
	neg = 1;
	res = 0;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v' ||
	str[i] == '\r' || str[i] == '\f' || str[i] == ' ')
		i++;
	if (str[i] == '-')
		neg = -1;
	if ((str[i] == '-') || (str[i] == '+'))
		i++;
	while ((str[i] != '\0') && (str[i] >= '0') && (str[i] <= '9'))
		res = (res * 10) + (str[i++] - '0');
	return (res * neg);
}

void	init_philo(t_philo *ph, pthread_mutex_t *fork, char **argv, t_common *common)
{
	int	i;

	i = -1;
	while (++i < ft_atoi(argv[1]))
	{
		pthread_mutex_init(&fork[i], NULL);
		ph[i].philosopher_number = i + 1;
		ph[i].time_to_die = ft_atoi(argv[2]);
		ph[i].time_to_eat = ft_atoi(argv[3]);
		ph[i].time_to_sleep = ft_atoi(argv[4]);
		if (argv[5] != NULL)
			ph[i].num_times_to_eat = ft_atoi(argv[5]);
		else
			ph[i].num_times_to_eat = -1;
		ph[i].last_meal = get_current_time();
		ph[i].start_time = get_current_time();
		ph[i].fork_left = &fork[i];
		ph[i].ending_flag = CONTINUE;
		ph[i].eating_flag = NOTEATING;
		ph[i].actual_meal = 0;
		ph[i].philo_amount = ft_atoi(argv[1]);
		if (i == ph[0].philo_amount - 1)
			ph[i].fork_right = &fork[0];
		else
			ph[i].fork_right = &fork[i + 1];
		ph[i].common = common;
	}
}

void	init_mutex(t_philo *ph, t_common *common)
{
	pthread_mutex_init(&common->print, NULL);
	pthread_mutex_init(&common->eat, NULL);
	pthread_mutex_init(&common->end, NULL);
}

void start_routine(t_philo *ph, pthread_t watcher, pthread_mutex_t *fork)
{
	if (pthread_create(&watcher, NULL, &watcher_routine, ph) != 0)
		destroy_mutex("Error\n", ph, fork);

	for (int i = 0; i < ph[0].philo_amount; ++i)
	{
		if (pthread_create(&ph[i].thread, NULL, &philosopher_action, &ph[i]) != 0)
			destroy_mutex("Error\n", ph, fork);
	}

	if (pthread_join(watcher, NULL) != 0)
		destroy_mutex("Error\n", ph, fork);
	// Join philosopher threads
	for (int i = 0; i < ph[0].philo_amount; ++i)
	{
		if (pthread_join(ph[i].thread, NULL) != 0)
			destroy_mutex("Error\n", ph, fork);
	}
}


void	destroy_threads(t_philo *ph, t_common *common, pthread_mutex_t *fork)
{
	for (int i = 0; i < ph[0].philo_amount; ++i)
		pthread_mutex_destroy(&fork[i]);
	pthread_mutex_destroy(&common->print);
	pthread_mutex_destroy(&common->eat);
	pthread_mutex_destroy(&common->end);
}

int	main(int argc, char *argv[])
{
	t_philo			philosopher_info[MAX_PHILOSOPHERS];
	pthread_mutex_t	fork[MAX_PHILOSOPHERS];
	pthread_t		the_watcher;
	t_common		common;

	if (argc < 5 || argc > 6)
	{
		printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", argv[0]);
		return (EXIT_FAILURE);
	}

	if (*argv[1] > MAX_PHILOSOPHERS)
	{
		printf("Invalid number of philosophers. Must be between 1 and %d.\n", MAX_PHILOSOPHERS);
		return (EXIT_FAILURE);
	}
	memset(&common, 0, sizeof(t_common));
	memset(&philosopher_info, 0, sizeof(t_philo));
	init_philo(philosopher_info, fork, argv, &common);
	init_mutex(philosopher_info, &common);
	if (philosopher_info[0].philo_amount == 1)
	{
		print_timestamp("has token his left fork", &philosopher_info[0]);
		usleep(philosopher_info[0].time_to_die * 1000);
		print_timestamp("died", &philosopher_info[0]);
		return (EXIT_SUCCESS);
	}
	start_routine(philosopher_info, the_watcher, fork);
	destroy_threads(philosopher_info, &common, fork);
	return (EXIT_SUCCESS);
}

int	check_end(t_philo *ph)
{
	pthread_mutex_lock(&ph->common->end);
	if (ph->ending_flag == CONTINUE)
	{
		pthread_mutex_unlock(&ph->common->end);
		return (CONTINUE);
	}
	pthread_mutex_unlock(&ph->common->end);
	return (END);
}

void	ft_dinner(t_philo *philo)
{
	take_forks(philo);
	print_timestamp("is eating", philo);
	philo->eating_flag = EATING;
	pthread_mutex_lock(&philo->common->eat);
	philo->actual_meal++;
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(&philo->common->eat);
	usleep(philo->time_to_eat * 1000);
	philo->eating_flag = NOTEATING;
	pthread_mutex_unlock(philo->fork_left);
	pthread_mutex_unlock(philo->fork_right);
}

void *philosopher_action(void *pointer)
{
	t_philo *philo = (t_philo *)pointer;

	if (philo->philosopher_number % 2 != 0)
		usleep(1000);

	while (check_end(philo) == CONTINUE)
	{
		ft_dinner(philo);
		if (check_end(philo) == END)
			break ;
		print_timestamp("is sleeping", philo);
		print_timestamp("is thinking", philo);
		usleep(philo->time_to_sleep * 1000);
	}
	return (NULL);
}

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

/* long check_death(t_philo *philosopher)
{
	struct timeval current_time;
	gettimeofday(&current_time, NULL);
	return (current_time.tv_sec - philosopher->start_time.tv_sec) * 1000 +
		(current_time.tv_usec - philosopher->start_time.tv_usec) / 1000;
} */

void	take_forks(t_philo *philo)
{
	// Lock the forks to prevent duplication
	pthread_mutex_lock(philo->fork_left);
	print_timestamp("has taken his left fork", philo);
	pthread_mutex_lock(philo->fork_right);
	print_timestamp("has taken his right fork", philo);
}

void	put_forks(t_philo *philosopher)
{
	// Unlock the forks
	pthread_mutex_unlock(philosopher->fork_left);
	pthread_mutex_unlock(philosopher->fork_right);
}

void	print_timestamp(char *action, t_philo *philosopher)
{
	int	current_time;

	pthread_mutex_lock(&philosopher->common->print);
	current_time = get_current_time() - philosopher->start_time;
	//printf("%s%d %s%d %s%s\n", ORANG, current_time, BLUE, philosopher->philosopher_number, RESET, action);
	printf("%s%03d %s%d %s%s\n", ORANG, current_time, BLUE, philosopher->philosopher_number, RESET, action);
	pthread_mutex_unlock(&philosopher->common->print);
}
