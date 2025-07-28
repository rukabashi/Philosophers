/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukabash <rukabash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:08:22 by rukabash          #+#    #+#             */
/*   Updated: 2025/06/30 18:08:23 by rukabash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *str)
{
	int			i;
	long long	res;
	int			sign;

	i = 0;
	res = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		if (res > 2147483647 && sign == 1)
			return (-1);
		if (res > 2147483648 && sign == -1)
			return (0);
		i++;
	}
	return ((int)(res * sign));
}

long long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_log(t_program *prog, int id, char *status)
{
	long long	timestamp;

	pthread_mutex_lock(&prog->write_lock);
	pthread_mutex_lock(&prog->dead_lock);
	if (!prog->is_dead && !prog->all_ate)
	{
		timestamp = get_current_time() - prog->start_time;
		printf("%lld %d %s\n", timestamp, id, status);
	}
	pthread_mutex_unlock(&prog->dead_lock);
	pthread_mutex_unlock(&prog->write_lock);
}

void	precise_usleep(long long time, t_program *prog)
{
	long long	start;

	start = get_current_time();
	while (1)
	{
		pthread_mutex_lock(&prog->dead_lock);
		if (prog->is_dead || prog->all_ate)
		{
			pthread_mutex_unlock(&prog->dead_lock);
			break ;
		}
		pthread_mutex_unlock(&prog->dead_lock);
		if (get_current_time() - start >= time)
			break ;
		usleep(100);
	}
}

int	error_exit(char *message)
{
	printf("Error: %s\n", message);
	return (1);
}