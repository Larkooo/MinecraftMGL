#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in mat3x2 Tile;

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
    vec2 tile = vec2(-1);

    if (Normal.x < 0 || Normal.x > 0 || Normal.z < 0 || Normal.z > 0)
        tile = Tile[1];
    else if (Normal.y > 0)
        tile = Tile[0];
    else if (Normal.y < 0)
        tile = Tile[2];

    if (tile != -1)
    {
        vec2 coords = tile + TexCoords;
        gl_FragColor = texture(uTexture, (coords * 64) / 1024);
    }
    else
    {
        gl_FragColor = vec4(vec3(0), 1);
    }
}