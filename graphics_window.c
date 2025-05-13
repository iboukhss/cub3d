/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_window.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:45:30 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/13 03:33:34 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

int	create_window(t_window *win)
{
	if (win->win_ctx)
	{
		return (1);
	}
	win->win_ctx = mlx_new_window(win->mlx_ctx, win->width, win->height,
			win->title);
	create_image(&win->frame, win->width, win->height);
	add_loop_hook(win);
	add_key_press_hook(win);
	add_key_release_hook(win);
	add_button_press_hook(win);
	add_button_release_hook(win);
	add_motion_notify_hook(win);
	add_expose_hook(win);
	add_destroy_notify_hook(win);
	return (0);
}

int	destroy_window(t_window *win)
{
	if (!win->mlx_ctx || !win->win_ctx)
	{
		return (1);
	}
	mlx_destroy_window(win->mlx_ctx, win->win_ctx);
	win->win_ctx = NULL;
	destroy_image(&win->frame);
	return (0);
}
