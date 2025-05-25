version 330 core
out vec4 FragColor;
void main()
{
   // Parameters
    vec3 circleColor = vec3(0.3, 0.5, 1);
    // Control with mouse
    // thickness = iMouse.x / iResolution.x;
    float fade = 0.005;

    // -1 -> 1 local space, adjusted for aspect ratio
    vec2 uv = fragCoord / iResolution.xy * 2.0 - 1.0;
    float aspect = iResolution.x / iResolution.y;
    uv.x *= aspect;
    
    // Calculate distance and fill circle with white
    float distance = 1.0 - length(uv);
    vec3 color = vec3(smoothstep(0.0, fade, distance));

    // Set output color
    fragColor = vec4(color, 1.0);
    fragColor.rgb *= circleColor;
}