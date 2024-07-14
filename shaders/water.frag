uniform float time;
uniform vec2 resolution;
uniform sampler2D texture;

void main() {
    vec2 uv = gl_TexCoord[0].xy;

    uv.y += 0.05 * sin(uv.x * 30.0 + time * 10.0);
    uv.x += 0.05 * cos(uv.y * 30.0 + time * 10.0);

    vec4 color = texture2D(texture, uv);

    gl_FragColor = color;
}
