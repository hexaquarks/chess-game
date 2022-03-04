const char VertexShader[] =
"void main()"
"{"
        "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
        "gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"
        "gl_FrontColor = gl_Color;"
"}";

const char RadialGradient[] =
"uniform vec4 color;"
"uniform vec2 center;"
"uniform float radius;"
"uniform float expand;"
"uniform float windowHeight;"
"void main(void)"
"{"
"vec2 centerFromSfml = vec2(center.x, windowHeight - center.y);"
"vec2 p = (gl_FragCoord.xy - centerFromSfml) / radius;"
        "float r = sqrt(dot(p, p));"
        "if (r < 1.0)"
        "{"
                "gl_FragColor = mix(color, gl_Color, (r - expand) / (1.0 - expand));"
        "}"
        "else"
        "{"
                "gl_FragColor = gl_Color;"
        "}"
"}";