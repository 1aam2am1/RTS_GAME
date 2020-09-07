#version 150

uniform sampler2D texture;

void main()
{
    const float Pi = 6.28318530718;// Pi*2

    // GAUSSIAN BLUR SETTINGS {{{
    const float Directions = 12.0;// BLUR DIRECTIONS (Default 16.0 - More is better but slower)
    const float Quality = 3.0;// BLUR QUALITY (Default 4.0 - More is better but slower)
    const float Size = 4.0;// BLUR SIZE (Radius)
    const float light = 0.6875;
    // GAUSSIAN BLUR SETTINGS }}}

    vec2 iResolution = textureSize(texture, 0);

    //Normalized radius
    vec2 Radius = Size/iResolution;

    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = gl_TexCoord[0].xy;
    // Pixel colour
    vec4 Color = texture2D(texture, uv);

    // Blur calculations
    for (float d=0.0; d<Pi; d+=Pi/Directions)
    {
        for (float i=1.0/Quality; i<=1.0; i+=1.0/Quality)
        {
            Color += texture2D(texture, uv+vec2(cos(d), sin(d))*Radius*i);
        }
    }

    // Output to screen
    Color.rgb /= Quality * Directions * light;
    Color.a /= Quality * Directions;
    gl_FragColor =  Color;
}
