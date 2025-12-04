#include "../include/cub_3d.h"

void draw_wall(t_data *data, int ray_index, double wall_top, double wall_bottom)
{
    int         y;
    int         tex_x;
    int         tex_y;
    int         pixel_color;
    t_image     *texture;
    t_ray       *ray;
    double      wall_x;
    int         line_height;
    int         draw_start;
    int         draw_end;

    ray = &data->rays[ray_index];
    texture = texture_decide(data, ray);
    
    line_height = (int)(wall_bottom - wall_top);
    draw_start = (int)wall_top;
    draw_end = (int)wall_bottom;
    
    if (ray->is_vertical_hit == 1)    // FIX: Divide by TILE_SIZE to get fractional position
        wall_x = ray->ray_y / TILE_SIZE;  // ← ADD THIS
    else
        wall_x = ray->ray_x / TILE_SIZE;  // ← ADD THIS
    
    wall_x = wall_x - floor(wall_x);    // Get fractional part (0.0 to 1.0)
    
    tex_x = (int)(wall_x * texture->width);    // Calculate texture X
    if (tex_x < 0)
        tex_x = 0;
    if (tex_x >= texture->width)
        tex_x = texture->width - 1;
    
    if (draw_start < 0)    // Clamp to screen
        draw_start = 0;
    if (draw_end >= WIN_HEIGHT)
        draw_end = WIN_HEIGHT;

    y = draw_start;     // Draw pixels
    while (y < draw_end)
    {
        int d = y * 256 - WIN_HEIGHT * 128 + line_height * 128;
        tex_y = ((d * texture->height) / line_height) / 256;
        
        if (tex_y < 0)
            tex_y = 0;
        if (tex_y >= texture->height)
            tex_y = texture->height - 1;
        
        pixel_color = texture_get_pixel(texture, tex_x, tex_y);
        put_pixel_to_buffer(&data->buffer, ray_index, y, pixel_color);
        
        y++;
    }
}

// You'll also need this helper to draw to your buffer
void put_pixel_to_buffer(t_image *buffer, int x, int y, int color)
{
    char *pixel;

    if (x < 0 || x >= WIN_WIDTH || y < 0 || y >= WIN_HEIGHT)
        return;
    
    pixel = buffer->pixel_data + (y * buffer->line_size + x * (buffer->bpp / 8));
    *(unsigned int *)pixel = color;
}
