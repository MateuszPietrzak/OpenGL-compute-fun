#version 460 core

layout (local_size_x = 1, local_size_y = 1) in;
layout (rgba32f, binding = 0) uniform image2D img_output;

vec3 ray_orig;
vec3 ray_dir;

vec3 rayAt(float t) {
    return ray_orig + t * ray_dir;
}

float hitSphere(vec3 center, float radius) {
    vec3 oc = ray_orig - center;
    float a = dot(ray_dir, ray_dir);
    float b = 2.0 * dot(oc, ray_dir);
    float c = dot(oc, oc) - radius * radius;
    float delta = b * b - 4.0 * a * c;
    if(delta < 0) {
        return -1.0;
    }
    return (-b-sqrt(delta))/(2*a);
}

vec3 rayColor() {
    float t = hitSphere(vec3(0.0, 0.0, -1.0), 0.5);
    if(t > 0.0) {
        vec3 N = normalize(rayAt(t) - vec3(0.0, 0.0, -1.0));
        return 0.5 * (N + 1.0);
    }

    vec3 unit_direction = normalize(ray_dir);
    float a = 0.5 * (unit_direction.y + 1.0);
    return (1.0-a) * vec3(1.0, 1.0, 1.0) + a * vec3(0.5, 0.7, 1.0);
}

void main() {
    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

    float max_x = 5.0;
    float max_y = 5.0;

    ivec2 dims = imageSize(img_output);
    float x = (float(pixel_coords.x * 2.0 - dims.x) / dims.x);
    float y = (float(pixel_coords.y * 2.0 - dims.y) / dims.y);

    ray_orig = vec3(0.0, 0.0, 0.0);
    ray_dir = -vec3(x, y, 1.0);

    imageStore(img_output, pixel_coords, vec4(rayColor(), 1.0));
}