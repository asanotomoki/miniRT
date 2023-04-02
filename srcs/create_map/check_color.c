/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tasano <tasano@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 16:25:57 by tasano            #+#    #+#             */
/*   Updated: 2023/04/02 12:16:13 by tasano           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int check_color(t_fcolor color)
{
	if (color.red < 0 || 255 < color.red)
		return (1);
	if (color.green < 0 || 255 < color.green)
		return (1);
	if (color.blue < 0 || 255 < color.blue)
		return (1);
	return (0);
}