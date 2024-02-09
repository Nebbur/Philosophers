/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42porto.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:19:58 by rboia-pe          #+#    #+#             */
/*   Updated: 2023/04/07 02:19:58 by rboia-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	print_timestamp(char *action, t_philo *philosopher, int color)
{
	int		current_time;
	char	*color_code;

	color_code = NULL;
	if (color == 0)
		color_code = GREEN;
	else if (color == 1)
		color_code = CYAN;
	else if (color == 2)
		color_code = RED;
	pthread_mutex_lock(&philosopher->common->print);
	if (color != 2 && check_end(philosopher) == END)
	{
		pthread_mutex_unlock(&philosopher->common->print);
		return ;
	}
	current_time = get_current_time() - philosopher->start_time;
	printf("%s%03d %s%d %s%s\n", YELLOW, current_time, BLUE, \
		philosopher->philosopher_number, color_code, action);
	pthread_mutex_unlock(&philosopher->common->print);
}

void	destroy_mutex(char *s, t_philo *ph, pthread_mutex_t *fork)
{
	int	i;

	i = -1;
	printf("%s\n", s);
	while (++i < ph[0].philo_amount)
		pthread_mutex_destroy(&fork[i]);
}
