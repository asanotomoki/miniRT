/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tasano <tasano@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 14:22:16 by tasano            #+#    #+#             */
/*   Updated: 2023/04/09 16:14:01 by tasano           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIAL_H
#define MATERIAL_H

typedef struct     s_material {
    double        kDif;      // 拡散反射係数  // これを物体の色とする
    double        kSpe;      // 鏡面反射係数
    float          shininess; // 光沢度
} t_material;

t_material		material_init(double kDif, double kSpe, double shininess);

#endif