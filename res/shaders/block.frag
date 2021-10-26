#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec2 Tile;

out vec4 FragColor;

// Texture map
uniform sampler2D uTexture;

void main()
{
    float light = 0.0f;

    // Choose the corresponding tile depending on the normal
    // Either top, side or bottom
    if (Normal.x < 0 || Normal.x > 0 || Normal.z < 0 || Normal.z > 0)
        light = -0.1f;
    else if (Normal.y < 0)
        light = -0.15f;

//    if (Tile == vec2(-1) || Tile.x < 0 || Tile.x > 15 || Tile.y < 0 || Tile.y > 15)
//        // no texture tile
//        Tile = vec2(13, 13);

    vec2 coords = Tile + TexCoords;
    FragColor = vec4(texture(uTexture, (coords * 64) / 1024).xyz + light, 1);
}
