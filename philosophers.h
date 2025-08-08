/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukabash <rukabash@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 18:08:15 by rukabash          #+#    #+#             */
/*   Updated: 2025/08/08 18:51:54 by rukabash         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

// Forward declaration
struct s_program;

typedef struct s_philo
{
	int					id;
	int					eat_count;
	long long			last_meal_time;
	pthread_t			thread;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	struct s_program	*prog;
}						t_philo;

typedef struct s_program
{
	int					num_of_philos;
	long long			time_to_die;
	long long			time_to_eat;
	long long			time_to_sleep;
	int					num_must_eat;
	int					is_dead;
	int					all_ate;
	long long			start_time;
	t_philo				*philos;
	pthread_mutex_t		*forks;
	pthread_mutex_t		meal_check_lock;
	pthread_mutex_t		write_lock;
	pthread_mutex_t		dead_lock;
}						t_program;

// init.c
int						init_program(t_program *prog, char **argv);

// routine.c
void					*philosopher_routine(void *arg);

// monitor.c
void					*monitor(void *arg);

// utils.c
int						ft_atoi(const char *str);
long long				get_current_time(void);
void					print_log(t_program *prog, int id, char *status);
void					precise_usleep(long long time, t_program *prog);
int						error_exit(char *message);

#endif
