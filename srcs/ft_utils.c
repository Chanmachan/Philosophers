#include "../includes/philo.h"

void	ft_putstr_fd(char *s, int fd)
{
	size_t	i;

	if (s == NULL)
		return ;
	i = 0;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
}

static int	ft_isspace(int c)
{
	if (c == ' ' || c == '\n' || c == '\r' || \
			c == '\f' || c == '\t' || c == '\v')
		return (1);
	return (0);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	long	ret;
	int		sign;
	size_t	i;

	ret = 0;
	sign = 1;
	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		if (sign == 1 && (ret > ((LONG_MAX - sign * (str[i] - '0')) / 10)))
			return ((int)LONG_MAX);
		if (sign == -1 && (ret < ((LONG_MIN - sign * (str[i] - '0')) / 10)))
			return ((int)LONG_MIN);
		ret = (ret * 10) + sign * (str[i] - '0');
		i++;
	}
	return ((int)ret);
}
