#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in mat3x2 Tile;

// Texture map
uniform sampler2D uTexture;

void main()
{
    vec2 tile = vec2(-1);
    float light = 0.0f;

    // Choose the corresponding tile depending on the normal
    // Either top, side or bottom
    if (Normal.x < 0 || Normal.x > 0 || Normal.z < 0 || Normal.z > 0)
    {
        light = -0.1f;
        tile = Tile[1];
    }
    else if (Normal.y > 0)
    {
        tile = Tile[0];
    }
        
    else if (Normal.y < 0)
    {
        light = -0.15f;
        tile = Tile[2];
    }

    if (tile != vec2(-1))
    {
        vec2 coords = tile + TexCoords;
        gl_FragColor = vec4(texture(uTexture, (coords * 64) / 1024).xyz + light, 1);
        //gl_FragColor = vec4(gl_FragP, 1.0f);
    }
    else
    {
        gl_FragColor = vec4(vec3(0), 1);
    }
}