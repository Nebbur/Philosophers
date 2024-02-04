/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42porto.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:19:58 by rboia-pe          #+#    #+#             */
/*   Updated: 2023/04/07 02:19:58 by rboia-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	init_philo(t_philo *ph, pthread_mutex_t *fork, \
char **argv, t_common *common)
{
	ph[0].i = -1;
	while (++ph[0].i < ft_atoi(argv[1]))
	{
		pthread_mutex_init(&fork[ph[0].i], NULL);
		ph[ph[0].i].philosopher_number = ph[0].i + 1;
		ph[ph[0].i].philo_amount = ft_atoi(argv[1]);
		ph[ph[0].i].time_to_die = ft_atoi(argv[2]);
		ph[ph[0].i].time_to_eat = ft_atoi(argv[3]);
		ph[ph[0].i].time_to_sleep = ft_atoi(argv[4]);
		ph[ph[0].i].num_times_to_eat = -1;
		if (argv[5] != NULL)
			ph[ph[0].i].num_times_to_eat = ft_atoi(argv[5]);
		ph[ph[0].i].actual_meal = 0;
		ph[ph[0].i].last_meal = get_current_time();
		ph[ph[0].i].start_time = get_current_time();
		ph[ph[0].i].ending_flag = CONTINUE;
		ph[ph[0].i].eating_flag = NOTEATING;
		ph[ph[0].i].fork_left = &fork[ph[0].i];
		if (ph[0].i == ph[0].philo_amount - 1)
			ph[ph[0].i].fork_right = &fork[0];
		else
			ph[ph[0].i].fork_right = &fork[ph[0].i + 1];
		ph[ph[0].i].common = common;
	}
}

void	ft_dinner(t_philo *philo)
{
	take_forks(philo);
	print_timestamp("is eating", philo, 0);
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

void	*philosopher_action(void *pointer)
{
	t_philo	*philo;

	philo = (t_philo *)pointer;
	if (philo->philosopher_number % 2 != 0)
		usleep(1000);
	while (check_end(philo) == CONTINUE)
	{
		ft_dinner(philo);
		if (check_end(philo) == END)
			break ;
		print_timestamp("is sleeping", philo, 0);
		usleep(philo->time_to_sleep * 1000);
		print_timestamp("is thinking", philo, 0);
		usleep(10000);
	}
	return (NULL);
}
