/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42porto.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:19:58 by rboia-pe          #+#    #+#             */
/*   Updated: 2023/04/07 02:19:58 by rboia-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	init_mutex(t_philo *ph, t_common *common)
{
	pthread_mutex_init(&common->print, NULL);
	pthread_mutex_init(&common->eat, NULL);
	pthread_mutex_init(&common->end, NULL);
}

void	destroy_threads(t_philo *ph, t_common *common, pthread_mutex_t *fork)
{
	int	i;

	i = -1;
	while (++i < ph[0].philo_amount)
		pthread_mutex_destroy(&fork[i]);
	pthread_mutex_destroy(&common->print);
	pthread_mutex_destroy(&common->eat);
	pthread_mutex_destroy(&common->end);
}

void	start_routine(t_philo *ph, pthread_t watcher, pthread_mutex_t *fork)
{
	int	i;

	i = -1;
	if (pthread_create(&watcher, NULL, &watcher_routine, ph) != 0)
		destroy_mutex("Error\n", ph, fork);
	while (++i < ph[0].philo_amount)
	{
		if (pthread_create(&ph[i].thread, NULL, \
			&philosopher_action, &ph[i]) != 0)
			destroy_mutex("Error\n", ph, fork);
	}
	if (pthread_join(watcher, NULL) != 0)
		destroy_mutex("Error\n", ph, fork);
	i = -1;
	while (++i < ph[0].philo_amount)
		if (pthread_join(ph[i].thread, NULL) != 0)
			destroy_mutex("Error\n", ph, fork);
}

static int	check_args(int argc, char *argv[])
{
	if (argc < 5 || argc > 6)
	{
		printf("Usage: %s number_of_philosophers time_to_die time_to_eat \
time_to_sleep [number_of_times_each_philosopher_must_eat]\n", argv[0]);
		return (EXIT_FAILURE);
	}
	if (*argv[1] > MAX_PHILOSOPHERS)
	{
		printf("Invalid number of philosophers. Must be between 1 \
and %d.\n", MAX_PHILOSOPHERS);
		return (EXIT_FAILURE);
	}
}

int	main(int argc, char *argv[])
{
	t_philo			philosopher_info[MAX_PHILOSOPHERS];
	pthread_mutex_t	fork[MAX_PHILOSOPHERS];
	pthread_t		the_watcher;
	t_common		common;

	if (check_args(argc, argv) == EXIT_FAILURE)
		return (EXIT_FAILURE);
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
