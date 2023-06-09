/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tasano <tasano@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 02:50:26 by tasano            #+#    #+#             */
/*   Updated: 2023/04/21 14:44:58 by tasano           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "object.h"
#include "miniRT.h"
#include "create_map.h"

static int	check_sphere(char **argv, t_sphere *sphere)
{
	if (check_vec(argv[0]))
		return (SP_CENTER_ERR);
	if (check_color(sphere->color))
		return (SP_COLOR_ERR);
	if (sphere->diameter < 0)
		return (SP_DIAMETER_ERR);
	return (0);
}

int	set_sphere(t_rt *rt, char **argv)
{
	t_sphere	*sphere;
	int			status;

	if (!argv || !argv[0] || !argv[1] || !argv[2] || argv[3])
		return (SP_DIF_ELEM_ERR);
	sphere = (t_sphere *)malloc(sizeof(t_sphere));
	if (!sphere)
		return (MALLOC_ERR);
	sphere->center = set_vec3(argv[0]);
	sphere->diameter = ft_atof(argv[1]);
	sphere->color = set_color(argv[2]);
	status = check_sphere(argv, sphere);
	if (status)
		free (sphere);
	else
		status = set_list(rt, (void *)sphere, SPHERE);
	return (status);
}
