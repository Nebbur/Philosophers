/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rboia-pe <rboia-pe@student.42porto.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 02:19:58 by rboia-pe          #+#    #+#             */
/*   Updated: 2023/04/07 02:19:58 by rboia-pe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>

# define MAX_PHILOSOPHERS 200
# define HASFORK 0
# define PRNTEAT 1
# define PRNTSLP 2
# define PRNTTHK 3
# define PRNTDIED 4
# define CONTINUE 0
# define END 1
# define EATING	1
# define NOTEATING 0

# define CYAN "\033[0;36m"
# define BLUE "\033[0;34m"
# define YELLOW "\033[0;33m"
# define GREEN "\033[0;32m"
# define RED "\033[0;31m"
# define RESET "\033[0m"

typedef struct s_common
{
	pthread_mutex_t	print;
	pthread_mutex_t	eat;
	pthread_mutex_t	end;
}				t_common;

// Structure to hold philosopher information
typedef struct s_philo
{
	pthread_t		thread;
	int				philosopher_number;
	int				philo_amount;
	int				actual_meal;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_times_to_eat;
	int				ending_flag;
	int				eating_flag;
	int				i;

	size_t			last_meal;
	size_t			start_time;
	t_common		*common;
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;
}			t_philo;

// Function declarations
int		main(int argc, char *argv[]);
void	*philosopher_action(void *pointer);
void	take_forks(t_philo *philosopher);
void	put_forks(t_philo *philosopher);
void	print_timestamp(char *action, t_philo *philosopher, int color);
size_t	get_current_time(void);

void	*watcher_routine(void *pointer);
int		search_deads(t_philo *ph);
int		check_every1_ate(t_philo *ph);
void	ft_ending(t_philo *ph);
int		check_end(t_philo *ph);
void	destroy_mutex(char *s, t_philo *ph, pthread_mutex_t *fork);

void	init_philo(t_philo *ph, pthread_mutex_t *fork, \
char	**argv, t_common *common);
void	init_mutex(t_common *common);
void	start_routine(t_philo *ph, pthread_t watcher, pthread_mutex_t *fork);
void	destroy_threads(t_philo *ph, t_common *common, pthread_mutex_t *fork);

int		ft_atoi(const char *str);
#endif
