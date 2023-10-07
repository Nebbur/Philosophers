
#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

/*
number_of_philosophers
time_to_die
time_to_eat
time_to_sleep
[number_of_times_each_philosopher_must_eat]*/

typedef struct philosophers
{
	int		nop; //number_of_philosophers
	int		pid;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		notep_must_eat; //number_of_times_each_philosopher_must_eat
}				philo_info;


int	main(int argc, char **argv);


/*.................. UTILS ..................*/

int	ft_atoi(const char *str);


#endif