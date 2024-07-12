uniform float time;
uniform vec2 resolution;
uniform sampler2D texture;

void main() {
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    float wave = sin(uv.x * 10.0 + time) * 0.2;
    uv.y += wave;
    vec4 color = texture2D(texture, uv);
    gl_FragColor = color;
}
