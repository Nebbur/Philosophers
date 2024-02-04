/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utilities.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42porto.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:19:58 by rboia-pe          #+#    #+#             */
/*   Updated: 2023/04/07 02:19:58 by rboia-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->fork_left);
	if (check_end(philo) == END)
		return ;
	print_timestamp("has taken his left fork", philo, 1);
	pthread_mutex_lock(philo->fork_right);
	if (check_end(philo) == END)
		return ;
	print_timestamp("has taken his right fork", philo, 1);
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
