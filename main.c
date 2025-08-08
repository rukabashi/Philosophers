/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukabash <rukabash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:08:29 by rukabash          #+#    #+#             */
/*   Updated: 2025/08/08 18:54:09 by rukabash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	cleanup(t_program *prog)
{
	int	i;

	i = 0;
	while (i < prog->num_of_philos)
	{
		pthread_mutex_destroy(&prog->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&prog->write_lock);
	pthread_mutex_destroy(&prog->meal_check_lock);
	pthread_mutex_destroy(&prog->dead_lock);
	if (prog->philos)
		free(prog->philos);
	if (prog->forks)
		free(prog->forks);
}

static int	start_simulation(t_program *prog)
{
	int			i;
	pthread_t	monitor_thread;

	i = 0;
	prog->start_time = get_current_time();
	while (i < prog->num_of_philos)
	{
		prog->philos[i].last_meal_time = prog->start_time;
		if (pthread_create(&prog->philos[i].thread, NULL,
				&philosopher_routine, &prog->philos[i]) != 0)
			return (error_exit("Failed to create thread"));
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, &monitor, prog) != 0)
		return (error_exit("Failed to create thread"));
	i = 0;
	while (i < prog->num_of_philos)
	{
		if (pthread_join(prog->philos[i].thread, NULL) != 0)
			return (error_exit("Failed to join thread"));
		i++;
	}
	if (pthread_join(monitor_thread, NULL) != 0)
		return (error_exit("Failed to join thread"));
	return (0);
}

int	main(int argc, char **argv)
{
	t_program	prog;

	if (argc < 5 || argc > 6)
		return (error_exit("Usage: ./philo num_philos time_die \
time_eat time_sleep [num_must_eat]"));
	if (init_program(&prog, argv) != 0)
		return (1);
	if (start_simulation(&prog) != 0)
	{
		cleanup(&prog);
		return (1);
	}
	cleanup(&prog);
	return (0);
}
