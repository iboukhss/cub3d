/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_events_debug.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:15:52 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/12 18:15:53 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

int	keyrelease_debug(int keysym, void *param)
{
	if (keysym == XK_Escape)
	{
		close_debug_window(param);
	}
	return (0);
}

int	close_main_window(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	mlx_loop_end(game->mlx_ctx);
	return (0);
}

int	close_debug_window(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	destroy_window(&game->debug);
	return (0);
}
