/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukabash <rukabash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:08:25 by rukabash          #+#    #+#             */
/*   Updated: 2025/08/08 18:54:44 by rukabash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philosophers.h"

static int	check_if_all_ate(t_program *prog)
{
	int	i;
	int	finished_eating;

	if (prog->num_must_eat == -1)
		return (0);
	finished_eating = 0;
	i = 0;
	while (i < prog->num_of_philos)
	{
		pthread_mutex_lock(&prog->meal_check_lock);
		if (prog->philos[i].eat_count >= prog->num_must_eat)
			finished_eating++;
		pthread_mutex_unlock(&prog->meal_check_lock);
		i++;
	}
	if (finished_eating == prog->num_of_philos)
	{
		pthread_mutex_lock(&prog->dead_lock);
		prog->all_ate = 1;
		pthread_mutex_unlock(&prog->dead_lock);
		return (1);
	}
	return (0);
}

static int	check_if_dead(t_program *prog)
{
	int	i;

	i = 0;
	while (i < prog->num_of_philos)
	{
		pthread_mutex_lock(&prog->meal_check_lock);
		if (get_current_time() - prog->philos[i].last_meal_time
			> prog->time_to_die)
		{
			print_log(prog, prog->philos[i].id, "died");
			pthread_mutex_lock(&prog->dead_lock);
			prog->is_dead = 1;
			pthread_mutex_unlock(&prog->dead_lock);
			pthread_mutex_unlock(&prog->meal_check_lock);
			return (1);
		}
		pthread_mutex_unlock(&prog->meal_check_lock);
		i++;
	}
	return (0);
}

void	*monitor(void *arg)
{
	t_program	*prog;

	prog = (t_program *)arg;
	while (1)
	{
		if (check_if_dead(prog) || check_if_all_ate(prog))
			break ;
		usleep(100);
	}
	return (NULL);
}
