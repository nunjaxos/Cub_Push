/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abhmidat <abhmidat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 13:02:42 by abhmidat          #+#    #+#             */
/*   Updated: 2025/12/09 14:28:09 by abhmidat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub_3d.h"

int texture_get_pixel(t_image *tex, int x, int y)
{
    char *pixel;

    if (x < 0 || x >= tex->width || y < 0 || y >= tex->height)
        return (0);

    pixel = tex->pixel_data + (y * tex->line_size + x * (tex->bpp / 8));
    return (*(unsigned int *)pixel);
}

int load_texture(t_data *data, t_image *tex, char *path)
{
    tex->img_ptr = mlx_xpm_file_to_image(
        data->mlx, path, &tex->width, &tex->height
    );
    if (!tex->img_ptr)
    {
        write(2, "Error\nFailed to load texture\n", 30);
        return (1);
    }

    tex->pixel_data = mlx_get_data_addr(
        tex->img_ptr, &tex->bpp, &tex->line_size, &tex->endian
    );
    if (!tex->pixel_data)
    {
        write(2, "Error\nFailed to get texture address\n", 37);
        return (1);
    }

    return (0);
}

int set_textures(t_data *data)
{
    if (load_texture(data, &data->tex_no, data->textures.north))
        return (1);

    if (load_texture(data, &data->tex_so, data->textures.south))
        return (1);

    if (load_texture(data, &data->tex_we, data->textures.west))
        return (1);

    if (load_texture(data, &data->tex_ea, data->textures.east))
        return (1);

    return (0);
}

t_image *texture_decide(t_data *data, t_ray *ray)
{
    if (ray->is_vertical_hit == 1) // Vertical wall hit
    {
        if (ray->ray_x > data->player.x)
            return &data->tex_ea; // East wall
        else
            return &data->tex_we; // West wall
    }
    else // Horizontal wall hit
    {
        if (ray->ray_y > data->player.y)
            return &data->tex_so; // South wall
        else
            return &data->tex_no; // North wall
    }
}
