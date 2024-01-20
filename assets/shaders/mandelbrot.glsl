#version 460 core

layout (local_size_x = 1, local_size_y = 1) in;
layout (rgba32f, binding = 0) uniform image2D img_output;

void main() {
    vec4 pixel = vec4(0.0, 0.0, 0.0, 1.0);

    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

    vec2 image_size = imageSize(img_output);
    vec2 c;
    c.y = float(pixel_coords.y) / (image_size.x / 4.0) - 2.0 * (image_size.y / image_size.x);
    c.x = float(pixel_coords.x) / (image_size.x / 4.0) - 2.0;
    vec2 z = c;

    int max_iters = 50;

    for (int i = 0; i < max_iters; i++) {
        vec2 tmp;
        tmp.x = z.x * z.x-z.y * z.y + c.x;
        tmp.y = 2 * z.x * z.y + c.y;
        z = tmp;
        if (z.x * z.x + z.y * z.y >= 4.0) {

            pixel = vec4(vec3( 1 - float(i) / max_iters), 1.0);
        }
    }

    imageStore(img_output, pixel_coords, pixel);
}