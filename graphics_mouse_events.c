/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_mouse_events.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboukhss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 18:30:16 by iboukhss          #+#    #+#             */
/*   Updated: 2025/05/12 18:34:07 by iboukhss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"

void	add_button_press_hook(t_window *win)
{
	struct s_hook	h;

	h.x_event = ButtonPress;
	h.x_mask = ButtonPressMask;
	h.func = win->button_press_hook;
	h.param = win->param;
	register_hook(win, h);
}

void	add_button_release_hook(t_window *win)
{
	struct s_hook	h;

	h.x_event = ButtonRelease;
	h.x_mask = ButtonReleaseMask;
	h.func = win->button_release_hook;
	h.param = win->param;
	register_hook(win, h);
}

void	add_motion_notify_hook(t_window *win)
{
	struct s_hook	h;

	h.x_event = MotionNotify;
	h.x_mask = PointerMotionMask;
	h.func = win->motion_notify_hook;
	h.param = win->param;
	register_hook(win, h);
}

void	add_expose_hook(t_window *win)
{
	struct s_hook	h;

	h.x_event = Expose;
	h.x_mask = ExposureMask;
	h.func = win->expose_hook;
	h.param = win->param;
	register_hook(win, h);
}

void	add_destroy_notify_hook(t_window *win)
{
	struct s_hook	h;

	h.x_event = DestroyNotify;
	h.x_mask = StructureNotifyMask;
	h.func = win->destroy_notify_hook;
	h.param = win->param;
	register_hook(win, h);
}
