#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec2 Tile;
in vec2 Factor;

uniform sampler2D uTexture;

// Returns the color value for the current fragment
// on the specified subtexture rect tiled 'tile'
// times.
//
vec4 SubTextureTile(vec2 uv, vec4 rect, vec2 factor)
{
    // uv is currently mapped from (0, 0) - (1, 1).
    
    // Multiply by the number of times we want to tile.
    // for a tiling of '2' this will cause the range of uv
    // to be (0, 0) - (2, 2).
    //
    uv *= factor;
    
    // The fractional part will repeat 'tile' times
    // 
    uv = fract(uv);
    
    // Map the uv coordinates to fit within the subtexture
    // rect.
    //
    uv *= vec2(rect.z, rect.w);
    uv += vec2(rect.x, rect.y);
    
    return texture(uTexture, uv);
}

void main()
{
    //vec2 uv = gl_FragCoord.xy / 800.0f;
    vec2 factor = abs(Factor);
    if (factor.x == 0)
        factor.x = 1;
    if (Factor.y == 0)
        factor.y = 1;

	gl_FragColor = SubTextureTile(TexCoords, vec4((Tile * 64.0f) / 1024.0f, 64.0f / 1024.0f, 64.0f / 1024.0f), factor);
    //gl_FragColor = vec4(Normal, 1.0f);
}