/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42porto.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:19:58 by rboia-pe          #+#    #+#             */
/*   Updated: 2023/04/07 02:19:58 by rboia-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	*watcher_routine(void *pointer)
{
	t_philo	*ph;

	ph = (t_philo *)pointer;
	while (1)
		if (search_deads(ph) == 1 || check_every1_ate(ph) == 1)
			break ;
	return (pointer);
}

int	search_deads(t_philo *ph)
{
	int	i;

	i = -1;
	while (++i < ph[0].philo_amount)
	{
		pthread_mutex_lock(&ph->common->eat);
		if (ph[i].eating_flag == NOTEATING && \
		(int)(get_current_time() - ph[i].last_meal) >= ph[i].time_to_die)
		{
			ft_ending(ph);
			print_timestamp("died", ph, 2);
			pthread_mutex_unlock(&ph->common->eat);
			return (1);
		}
		pthread_mutex_unlock(&ph->common->eat);
	}
	return (0);
}

int	check_every1_ate(t_philo *ph)
{
	int	meal_num_reached;
	int	i;

	i = -1;
	meal_num_reached = 0;
	if (ph[0].num_times_to_eat == 0)
		return (0);
	while (++i < ph[0].philo_amount)
	{
		pthread_mutex_lock(&ph->common->eat);
		if (ph[0].num_times_to_eat != -1 && ph[i].actual_meal \
		>= ph[0].num_times_to_eat)
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
