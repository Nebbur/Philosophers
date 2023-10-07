#include "../includes/philosophers.h"

/*
number_of_philosophers
time_to_die
time_to_eat
time_to_sleep
[number_of_times_each_philosopher_must_eat]*/


/* int		philo_is_eaten(int philo, philo_info *st)
{

}

// RETURN 0 IF FORK OF PHILO IS AVAILABLE, ELSE RETURN 1
int		fork_is_available(int philo, philo_info *st)
{

} */

void	time_to_eat(int even_or_odd, philo_info *st)
{
	printf("A table %i\n", even_or_odd);
	usleep(st->time_to_eat);
}

void	*get_organized(philo_info *st)
{
	int	even_or_odd; // par 0 ou impar 1
	pthread_t	philosopher;

	philosopher = st->pid;
	printf("philosofer %li\n", philosopher);
	if (philosopher % 2 == 0)
		even_or_odd = 0;
	else
		even_or_odd = 1;
	
	time_to_eat(even_or_odd, st);

}

void	init(philo_info *st)
{
	pthread_t	philosopher[st->nop];
	int			pid[st->nop];
	int			i;


	i = -1;
	while (++i != st->nop)
	{
		st->pid = i;
		pthread_create(&philosopher[i], NULL, get_organized, st);
	}

	i = -1;
	while (++i != st->nop)
		pthread_join(philosopher[i], NULL);

}

int	main(int argc, char **argv)
{
	philo_info  st;

	if (argc != 5 && argc != 6)
	{
		write (2, "Invalid number of arguments\n", 28);
		return (0);
	}
	st.nop = ft_atoi(argv[1]);
	st.time_to_die = ft_atoi(argv[2]);
	st.time_to_eat = ft_atoi(argv[3]);
	st.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		st.notep_must_eat = ft_atoi(argv[5]);
	init(&st);	

	return (0);
}


/*

preciso de fazer uma funcao que determina quem ]e o prioritario*

*: quem nao comeu na ronda passada OU quem nao come ha mais tempo


*/