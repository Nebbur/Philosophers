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

		//ph[i].ending_flag = CONTINUE;
		//ph[i].eating_flag = NOTEATING;
void	init_philo(t_philo *ph, pthread_mutex_t *fork, \
char **argv, t_common *common)
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
		ph[i].num_times_to_eat = -1;
		if (argv[5] != NULL)
			ph[i].num_times_to_eat = ft_atoi(argv[5]);
		ph[i].last_meal = get_current_time();
		ph[i].start_time = get_current_time();
		ph[i].fork_left = &fork[i];
		ph[i].actual_meal = 0;
		ph[i].philo_amount = ft_atoi(argv[1]);
		if (i == ph[0].philo_amount - 1)
			ph[i].fork_right = &fork[0];
		else
			ph[i].fork_right = &fork[i + 1];
		ph[i].common = common;
	}
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
		print_timestamp("is sleeping", philo);
		print_timestamp("is thinking", philo);
		usleep(philo->time_to_sleep * 1000);
	}
	return (NULL);
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
