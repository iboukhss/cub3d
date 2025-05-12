/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_events.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:29:14 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/12 18:32:21 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

void	register_hook(t_window *win, struct s_hook h)
{
	if (h.func)
	{
		mlx_hook(win->win_ctx, h.x_event, h.x_mask, h.func, h.param);
	}
}

void	add_loop_hook(t_window *win)
{
	if (win->loop_hook)
	{
		mlx_loop_hook(win->mlx_ctx, win->loop_hook, win->param);
	}
}

void	add_key_press_hook(t_window *win)
{
	struct s_hook	h;

	h.x_event = KeyPress;
	h.x_mask = KeyPressMask;
	h.func = win->key_press_hook;
	h.param = win->param;
	register_hook(win, h);
}

void	add_key_release_hook(t_window *win)
{
	struct s_hook	h;

	h.x_event = KeyRelease;
	h.x_mask = KeyReleaseMask;
	h.func = win->key_release_hook;
	h.param = win->param;
	register_hook(win, h);
}
