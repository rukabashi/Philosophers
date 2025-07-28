/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukabash <rukabash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:08:19 by rukabash          #+#    #+#             */
/*   Updated: 2025/06/30 18:08:20 by rukabash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	philo_eat(t_philo *philo)
{
	t_program	*prog;

	prog = philo->prog;
	// デッドロック回避のため、偶数IDの哲学者は右から、奇数IDは左からフォークを取る
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_log(prog, philo->id, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_log(prog, philo->id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_log(prog, philo->id, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_log(prog, philo->id, "has taken a fork");
	}
	pthread_mutex_lock(&prog->meal_check_lock);
	print_log(prog, philo->id, "is eating");
	philo->last_meal_time = get_current_time();
	philo->eat_count++;
	pthread_mutex_unlock(&prog->meal_check_lock);
	precise_usleep(prog->time_to_eat, prog);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static int	is_simulation_over(t_program *prog)
{
	int	res;

	pthread_mutex_lock(&prog->dead_lock);
	res = prog->is_dead || prog->all_ate;
	pthread_mutex_unlock(&prog->dead_lock);
	return (res);
}

void	*philosopher_routine(void *arg)
{
	t_philo		*philo;
	t_program	*prog;

	philo = (t_philo *)arg;
	prog = philo->prog;
	// 全員が同時に開始するのを防ぐため、偶数IDは少し待つ
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!is_simulation_over(prog))
	{
		philo_eat(philo);
		if (is_simulation_over(prog))
			break ;
		print_log(prog, philo->id, "is sleeping");
		precise_usleep(prog->time_to_sleep, prog);
		if (is_simulation_over(prog))
			break ;
		print_log(prog, philo->id, "is thinking");
	}
	return (NULL);
}