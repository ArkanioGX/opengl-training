#version 450

uniform float time;

layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) uniform image2D img_output;

void main() {
  // Base pixel colour for image
  vec4 pixel = vec4(0.0, 0.0, 0.0, 1.0);
  // Get index in global work group i.e x,y position
  ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
  
  // Hard coded scene
  vec3 sphere_c = vec3((cos(time) * 5), 0.0 + sin(time) * 3, -10.0 );
  float sphere_r = 1.0;

  float fov = 110;

  float max_x = 5.0;
  float max_y = 5.0;
  ivec2 dims = imageSize(img_output); // fetch image dimensions
  float x = (float(pixel_coords.x * 2 - dims.x) / dims.x);
  float y = (float(pixel_coords.y * 2 - dims.y) / dims.y);
  vec3 ray_o = vec3(x * max_x, y * max_y, 0.0);
  vec3 ray_d = vec3(0.0, 0.0, -1.0); // ortho

  vec3 omc = sphere_c - ray_o;
  float a = dot(ray_d,ray_d);
  float b = -2.0 * dot(ray_d, omc);
  float c = dot(omc, omc) - sphere_r * sphere_r;
  float bsqmc = b * b - 4*a*c;
  double h = bsqmc < 0 ? -1.0 : -b - sqrt(bsqmc)  / (2.0*a);
    if (h > 0.0) {
      vec3 N = normalize( ray_d - ray_o);
      pixel = vec4(0.5*N.x+1, 0.5*N.y+1,0.5*N.z+1, 1.0);
    }
    else{
      pixel = vec4(1.0,1.0,1.0,1.0);
        vec3 unit_direction = normalize(ray_d);
        double m = 0.5*unit_direction.y + 1.0 ;
        pixel = vec4((1.0-m)*vec3(1.0, 1.0, 1.0) + m*vec3(0.5, 0.7, 1.0),1.0);
    }
  // Output to a specific pixel in the image
  imageStore(img_output, pixel_coords, pixel);
}