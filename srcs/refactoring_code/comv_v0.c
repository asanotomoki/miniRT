#include "material.h"
#include "object.h"
#include "combination.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mlx.h"
#include "mlx_info.h"
#include "libft.h"

#define PLAIN 1
#define NUMBER 2
void	get_t_value(t_vec3 start_vec, t_vec3 dir_vec, t_objects *object_list, double *t, int i, int shadow)//交差判定
{
	double a = 0;
	double b = 0;
	double c = 0;
	double t1 = 0, t2 = 0;
	// t_vec3	bottom_center_vec;
	if (object_list[i].kind == SPHERE || object_list[i].kind == CYLINDER)
	{
		if (object_list[i].kind == SPHERE)
		{
			t_vec3 camera2sphere_vec = vec3_sub(start_vec, object_list[i].vec); //カメラから球へのベクトル
			a = vec3_mag(dir_vec) * vec3_mag(dir_vec);
			b = 2 * vec3_dot(camera2sphere_vec, dir_vec);
			c = vec3_dot(camera2sphere_vec, camera2sphere_vec) - object_list[i].diameter * object_list[i].diameter;
		}
		else if (object_list[i].kind == CYLINDER)
		{
			// bottom_center_vec = vec3_sub(object_list[i].vec, vec3_mul(object_list[i].normal_vec, object_list[i].height / 2));
			t_vec3	tmp_vec = vec3_sub(start_vec, object_list[i].vec);
			t_vec3	tmp_cross = vec3_cross(dir_vec, object_list[i].normal_vec);
			t_vec3	tmp_vec_and_cross = vec3_cross(tmp_vec, object_list[i].normal_vec);
			a = vec3_mag(tmp_cross) * vec3_mag(tmp_cross);
			b = 2 * vec3_dot(tmp_cross, tmp_vec_and_cross);
			c = vec3_mag(tmp_vec_and_cross) * vec3_mag(tmp_vec_and_cross) - object_list[i].diameter * object_list[i].diameter;
		}
		// 判別式
		double D = b * b - 4 * a * c;
		if (a != 0 && D >= 0)
		{
			//交差判定
			if (D == 0) //接する時
			{
				t[i] = -b / (2.0 * a);//媒介変数tの長さ
			}
			else if (D > 0) //交わる時
			{
				t1 = (-b - sqrt(D)) / (2.0 * a);
				t2 = (-b + sqrt(D)) / (2.0 * a);
				 if (object_list[i].kind == SPHERE)
				 {
					if (t1 > 0 && t2 > 0) //両方正なら小さい方をtとする。
					{
						if (t1 > t2)
							t[i] = t2;
						else
							t[i] = t1;
					}
					else//片方正なら、それをtとする。
					{
						if (t1 > 0)
							t[i] = t1;
						else if (t2 > 0)
							t[i] = t2;
						else
						{
							t[i] = 0;
						}
					}
				 }
				if (object_list[i].kind == CYLINDER)
				{
					double compare1 = vec3_dot(vec3_sub(vec3_add(start_vec, vec3_mul(dir_vec, t1)), object_list[i].vec), object_list[i].normal_vec);
					double compare2 = vec3_dot(vec3_sub(vec3_add(start_vec, vec3_mul(dir_vec, t2)), object_list[i].vec), object_list[i].normal_vec);
					if (0 <= compare1 && compare1 <= object_list[i].height && t1 > 0)
					{
						t[i] = t1;
						if (shadow)
							object_list[i].cylinder_front_or_back = FRONT;
					}
					else if (0 <= compare2 && compare2 <= object_list[i].height && t2 > 0)
					{
						t[i] = t2;
						if (shadow)
							object_list[i].cylinder_front_or_back = BACK;
					}
					else
					{
						t[i] = 0;
						if (shadow)
							object_list[i].cylinder_front_or_back = NOTHING;
					}
				}
			}
		}
		else
			t[i] = 0;
	}
	else if (object_list[i].kind == PLAIN)
	{
		if (vec3_dot(dir_vec, object_list[i].normal_vec) == 0)
			t[i] = 0;
		else if (vec3_dot(vec3_sub(object_list[i].vec, start_vec), object_list[i].normal_vec) / vec3_dot(dir_vec, object_list[i].normal_vec) < 0)
			t[i] = 0;
		else
			t[i] = (vec3_dot(vec3_sub(object_list[i].vec, start_vec), object_list[i].normal_vec) / vec3_dot(dir_vec, object_list[i].normal_vec));
	}
}

int	get_min_index(double *t, int number)
{
	int i;
	double min;
	int min_index;

	for(i = 0; i < number; i++)
	{
		if (t[i] != 0)
		{
			min = t[i];
			min_index = i;
			break ;
		}
		min = 0;
		min_index = 0;
	}
	if (min != 0)
	{
		for(i = 0; i < number; i++)
		{
			if (t[i] < min && t[i] != 0)
			{
				min = t[i];
				min_index = i;
			}
		}
	}
	return (min_index);
}

double	check_shadow(t_vec3 crosspoint_vec, t_vec3 incident_vec, double epsilon, t_objects *object_list)
{
	double	*check_shadow = malloc(sizeof(double) * NUMBER);
	int		tmp_min_index;
	int		index;
	double	shadow_min;
	for(index = 0; index < NUMBER; index++)
		get_t_value(vec3_add(crosspoint_vec, vec3_mul(incident_vec, epsilon)), incident_vec, object_list, check_shadow, index, 0);
	tmp_min_index = get_min_index(check_shadow, NUMBER);
	shadow_min = check_shadow[tmp_min_index];
	free(check_shadow);
	return (shadow_min);
}

t_vec3	determin_normal_vec(t_objects *object_list, t_vec3 crosspoint_vec, int i, t_vec3 camera_vec, double *t, t_vec3 dir_vec)
{
	t_vec3	normal_vec = vec3_init(0, 0, 0);;

	if (object_list[i].kind == SPHERE)
		normal_vec = vec3_normalize(vec3_sub(crosspoint_vec, object_list[i].vec));//法線ベクトル
	else if (object_list[i].kind == PLAIN)
		normal_vec = object_list[i].normal_vec;
	else if (object_list[i].kind == CYLINDER)
	{
		t_vec3 tmp_vec = vec3_sub(vec3_add(camera_vec, vec3_mul(dir_vec, t[i])), object_list[i].vec);
		if (object_list[i].cylinder_front_or_back == FRONT)
		{
			normal_vec = vec3_normalize(vec3_sub(tmp_vec, vec3_mul(object_list[i].normal_vec, vec3_dot(tmp_vec, object_list[i].normal_vec))));
		}
		else if (object_list[i].cylinder_front_or_back == BACK)
		{
			normal_vec = vec3_mul(vec3_normalize(vec3_sub(tmp_vec, vec3_mul(object_list[i].normal_vec, vec3_dot(tmp_vec, object_list[i].normal_vec)))), -1);
		}
	}
	return (normal_vec);
}

t_fcolor	diffusion(t_vec3 incident_vec, t_vec3 reflect_normal_vec, t_objects *object_list, int i, t_fcolor new)
{
	double	diffusion = 0;
	double	dot_1;
	if (vec3_dot(incident_vec, reflect_normal_vec) <= 0)
		dot_1 = 0;
	else
	{
		dot_1 = vec3_dot(incident_vec, reflect_normal_vec);
		// if (object_list[i].kind == SPHERE)
		// {
		// 	printf("dot1: %f\n", dot_1);
		// }
	}
	diffusion = dot_1 * object_list[i].material.kDif;
	return (add_color(new, object_list[i].color, diffusion));
}

t_fcolor specular(t_vec3 dir_vec, t_vec3 reflect_normal_vec,
	t_vec3 incident_vec, t_objects *object_list, t_fcolor new,
		t_fcolor light_color, double light_power, int i)
{
	double	specular = 0;
	t_vec3 reverse_dir_vec = vec3_normalize(vec3_mul(dir_vec, -1));//視線ベクトルの逆向き
	t_vec3 regular_reflection_vec = vec3_normalize(vec3_sub(vec3_mul(vec3_mul(reflect_normal_vec, vec3_dot(incident_vec, reflect_normal_vec)), 2), incident_vec));
	double dot_2;
	if (vec3_dot(reverse_dir_vec, regular_reflection_vec) < 0)
		dot_2 = 0;
	else
		dot_2 = vec3_dot(reverse_dir_vec, regular_reflection_vec);
	specular = pow(dot_2, object_list[i].material.shininess) * object_list[i].material.kSpe;
	return (add_color(new, light_color, light_power * specular));
}

void	my_put_pixel(t_vec3 camera_vec, t_vec3 dir_vec, t_vec3 light_vec, t_objects *object_list, double *t,
			int i, double light_power, double ambient_power, t_game *game, double x, double y, double epsilon,
			t_fcolor light_color, t_fcolor ambient_color)
{
	t_fcolor new = rgb_init(0, 0, 0);
	t_vec3 crosspoint_vec = vec3_add(camera_vec, vec3_mul(dir_vec, t[i]));//視線と物体の交点の位置ベクトル
	t_vec3 incident_vec = vec3_normalize(vec3_sub(light_vec, crosspoint_vec));//入射ベクトル(入射って言ってるけど、向きに注意)

	new = add_color(new, ambient_color, ambient_power);
	//影かどうか
	double	distance = vec3_mag(vec3_sub(light_vec, crosspoint_vec)) - epsilon;
	double	shadow = 0;
	shadow = check_shadow(crosspoint_vec, incident_vec, epsilon, object_list);
	//if (shadow != 0)
	//	printf("shadow: %f\n", shadow);
	// if (object_list[i].kind == CYLINDER)
	// 	printf("shadow; %f\n", shadow);
	t_vec3	reflect_normal_vec;
	if (shadow == 0 || distance < vec3_mag(vec3_add(crosspoint_vec, vec3_mul(incident_vec, epsilon + shadow))))
	{
		reflect_normal_vec = determin_normal_vec(object_list, crosspoint_vec, i, camera_vec, t, dir_vec);
		//拡散反射
		new = diffusion(incident_vec, reflect_normal_vec, object_list, i, new);

		// 鏡面反射
		new = specular(dir_vec, reflect_normal_vec, incident_vec, object_list, new, light_color, light_power, i);
	}
	light_color.blue++;
	light_power++;

	if (t[i] > 0)
	{
		mlx_pixel_put(game->mlx, game->win, x, y, rgb_to_int((int)new.red, (int)new.green, (int)new.blue));
	}
	else
	{
		mlx_pixel_put(game->mlx, game->win, x, y, rgb_to_int(255, 255, 255));
	}
}

t_orthonormal	init_unit(t_vec3 camera_normal_vec)
{
	t_orthonormal	ret;

	t_vec3	y_unit_vector = vec3_init(0, 1, 0); //(0, 1, 0)の単位ベクトル。これを定義しないと自由に回転できちゃう。
	ret.x_unit_vector = vec3_cross(y_unit_vector, camera_normal_vec);
	ret.y_unit_vector = vec3_cross(camera_normal_vec, ret.x_unit_vector);
	return (ret);
}

int		main_loop(t_game *game)
{
	double theta = 100;//FOV
	double	camera2screen = 2 / (2 * tan(((theta / 180 * M_PI)/ 2)));//カメラからスクリーンまでの距離
	double epsilon = 1.0 / 512; //微小距離
	t_vec3 camera_vec = vec3_init(2, 5, -5); //視点位置のベクトル
	t_vec3 camera_normal_vec = vec3_normalize(vec3_init(0, -1, 1)); //視点位置の法線ベクトル
	t_orthonormal screen_unit_vec = init_unit(camera_normal_vec);
	double light_power = 1.0;
	t_fcolor light_color = rgb_init(255, 255, 255);
	double ambient_power = 0.1;
	t_fcolor ambient_color = rgb_init(255, 255, 255);
	t_vec3 light_vec = vec3_init(-5, 10, -5); //光源の位置ベクトル
	t_objects *object_list = (t_objects *)malloc(sizeof(t_objects) * NUMBER);
	// //1個目の球
	// object_list[0].vec = vec3_init(-1, 0, 5); //球の中心座標
	// object_list[0].diameter = 1.0; //球の直径
	// object_list[0].color = rgb_init(255, 0, 0);
	// object_list[0].material = material_init(0.69, 0.3, 8);
	// object_list[0].kind = SPHERE;

	// //2個目の球
	// object_list[1].vec = vec3_init(0, 0, 10); //球の中心座標
	// object_list[1].diameter = 1.0; //球の直径
	// object_list[1].color = rgb_init(255, 255, 0);
	// object_list[1].material = material_init(0.69, 0.3, 8);
	// object_list[1].kind = SPHERE;

	// //3個目の球
	// object_list[2].vec = vec3_init(1, 0, 15); //球の中心座標
	// object_list[2].diameter = 1.0; //球の直径
	// object_list[2].color = rgb_init(0, 0, 255);
	// object_list[2].material = material_init(0.69, 0.3, 8);
	// object_list[2].kind = SPHERE;

	// //4個目の球
	// object_list[3].vec = vec3_init(2, 0, 20); //球の中心座標
	// object_list[3].diameter = 1.0; //球の直径
	// object_list[3].color = rgb_init(255, 0, 255);
	// object_list[3].material = material_init(0.69, 0.3, 8);
	// object_list[3].kind = SPHERE;

	// //5個目の球
	// object_list[4].vec = vec3_init(3, 0, 25); //球の中心座標
	// object_list[4].diameter = 1.0; //球の直径
	// object_list[4].color = rgb_init(0, 255, 0);
	// object_list[4].material = material_init(0.69, 0.3, 8);
	// object_list[4].kind = SPHERE;

	//平面
	object_list[0].vec = vec3_init(0, -1, 0);
	object_list[0].color = rgb_init(0, 255, 255);
	object_list[0].material = material_init(0.69, 0.3, 8);
	object_list[0].normal_vec = vec3_init(0, 1, 0);
	object_list[0].kind = PLAIN;

	//円筒
	object_list[1].vec = vec3_init(0, -1, 0);
	object_list[1].diameter = 0.5;
	object_list[1].height = 3;
	object_list[1].color = rgb_init(0, 0, 255);
	object_list[1].material = material_init(0.69, 0.3, 8);
	object_list[1].normal_vec = vec3_init(0, 1, 0);
	object_list[1].kind = CYLINDER;
	object_list[1].cylinder_front_or_back = NOTHING;

	int i;
	int min_index;
	for (double y = 0; y < HEIGHT; y++)
	{
		for (double x = 0; x < WIDTH; x++)
		{
			double	f_x = 2 * x / WIDTH - 1.0;
			double	f_y = -(2 * y / HEIGHT - 1.0);
			double *t = (double *)malloc(sizeof(double) * NUMBER);
			t_vec3 screen_vec = vec3_add(vec3_add(vec3_add(camera_vec, \
										vec3_mul(camera_normal_vec, camera2screen)), \
										vec3_mul(screen_unit_vec.x_unit_vector, f_x)), \
										vec3_mul(screen_unit_vec.y_unit_vector, f_y)); //スクリーン上の点の三次元空間におけるベクトル
			t_vec3 dir_vec = vec3_normalize(vec3_sub(screen_vec, camera_vec)); //方向ベクトル(視線ベクトル)
			for (i = 0; i < NUMBER; i++)
				get_t_value(camera_vec, dir_vec, object_list, t, i, 1);
			min_index = get_min_index(t, NUMBER);
			//printf("min: %f\n", t[min_index]);
			my_put_pixel(camera_vec, dir_vec, light_vec, object_list, t, min_index, light_power,
				ambient_power, game, x, y, epsilon, light_color, ambient_color);
			free(t);
		}
	}
	free(object_list);
	return (0);
}