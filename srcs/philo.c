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

void take_forks(t_philo *philo)
{
	pthread_mutex_t *first_fork;
	pthread_mutex_t *second_fork;

	first_fork = philo->fork_left;
	second_fork = philo->fork_right;
	if (philo->fork_left > philo->fork_right)
	{
		first_fork = philo->fork_right;
		second_fork = philo->fork_left;
	}
	if (pthread_mutex_lock(first_fork) == 0 && check_end(philo) == CONTINUE)
		print_timestamp("has taken his left fork", philo, 1);
	else
	{
		pthread_mutex_unlock(first_fork);
		return ;
	}
	if (pthread_mutex_lock(second_fork) == 0 && check_end(philo) == CONTINUE)
		print_timestamp("has taken his right fork", philo, 1);
	else
	{
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
	}
}

void	put_forks(t_philo *philosopher)
{
	pthread_mutex_unlock(philosopher->fork_left);
	pthread_mutex_unlock(philosopher->fork_right);
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
	if (check_end(philo) == END)
		return ;
	pthread_mutex_lock(&philo->common->eat);
	print_timestamp("is eating", philo, 0);
	philo->eating_flag = EATING;
	philo->actual_meal++;
	philo->last_meal = get_current_time();
	pthread_mutex_unlock(&philo->common->eat);
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_lock(&philo->common->eat);
	philo->eating_flag = NOTEATING;
	pthread_mutex_unlock(&philo->common->eat);
	pthread_mutex_unlock(philo->fork_left);
	pthread_mutex_unlock(philo->fork_right);
}

void	*philosopher_action(void *pointer)
{
	t_philo	*philo;

	philo = (t_philo *)pointer;
	if (philo->philosopher_number % 2 == 0)
		usleep(1000);
	while (check_end(philo) == CONTINUE)
	{
		ft_dinner(philo);
		if (check_end(philo) == END)
			break ;
		print_timestamp("is sleeping", philo, 0);
		usleep(philo->time_to_sleep * 1000);
		if (check_end(philo) == END)
			break ;
		print_timestamp("is thinking", philo, 0);
		usleep(500);
	}
	return (NULL);
}
