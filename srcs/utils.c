#include "../includes/philosophers.h"

int	ft_atoi(const char *str)
{
	int	sign;
	int	result;

	result = 0;
	sign = 1;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	if (*str == 45)
		sign *= -1;
	if (*str == 45 || *str == 43)
		str++;
	while (*str >= 48 && *str <= 57)
		result = result * 10 + *str++ - 48;
	return (result * sign);
}