/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukabash <rukabash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:08:31 by rukabash          #+#    #+#             */
/*   Updated: 2025/08/08 19:03:33 by rukabash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	init_mutexes(t_program *prog)
{
	int	i;

	prog->forks = malloc(sizeof(pthread_mutex_t) * prog->num_of_philos);
	if (!prog->forks)
		return (error_exit("Malloc failed for forks"));
	i = 0;
	while (i < prog->num_of_philos)
	{
		if (pthread_mutex_init(&prog->forks[i], NULL) != 0)
			return (error_exit("Mutex init failed"));
		i++;
	}
	if (pthread_mutex_init(&prog->write_lock, NULL) != 0)
		return (error_exit("Mutex init failed"));
	if (pthread_mutex_init(&prog->meal_check_lock, NULL) != 0)
		return (error_exit("Mutex init failed"));
	if (pthread_mutex_init(&prog->dead_lock, NULL) != 0)
		return (error_exit("Mutex init failed"));
	return (0);
}

static void	init_philosophers(t_program *prog)
{
	int	i;

	i = 0;
	while (i < prog->num_of_philos)
	{
		prog->philos[i].id = i + 1;
		prog->philos[i].eat_count = 0;
		prog->philos[i].last_meal_time = 0;
		prog->philos[i].prog = prog;
		prog->philos[i].left_fork = &prog->forks[i];
		if (i == 0)
			prog->philos[i].right_fork = &prog->forks[prog->num_of_philos - 1];
		else
			prog->philos[i].right_fork = &prog->forks[i - 1];
		i++;
	}
}

int	init_program(t_program *prog, char **argv)
{
	prog->num_of_philos = ft_atoi(argv[1]);
	prog->time_to_die = ft_atoi(argv[2]);
	prog->time_to_eat = ft_atoi(argv[3]);
	prog->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		prog->num_must_eat = ft_atoi(argv[5]);
	else
		prog->num_must_eat = -1;
	if (prog->num_of_philos <= 0 || prog->time_to_die < 0
		|| prog->time_to_eat < 0 || prog->time_to_sleep < 0)
		return (error_exit("Invalid arguments"));
	prog->is_dead = 0;
	prog->all_ate = 0;
	prog->philos = malloc(sizeof(t_philo) * prog->num_of_philos);
	if (!prog->philos)
		return (error_exit("Malloc failed for philos"));
	if (init_mutexes(prog) != 0)
		return (1);
	init_philosophers(prog);
	return (0);
}
