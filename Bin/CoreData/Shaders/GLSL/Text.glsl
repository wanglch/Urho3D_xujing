#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"

varying vec2 vTexCoord;
varying vec4 vColor;

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    
    vTexCoord = iTexCoord;
    vColor = iColor;
}

void PS()
{
    gl_FragColor.rgb = vColor.rgb;

#ifdef SIGNED_DISTANCE_FIELD
    float distance = texture2D(sDiffMap, vTexCoord).a;
    if (distance < 0.5f)
        gl_FragColor.a = 0.0;
    else
        gl_FragColor.a = vColor.a;

    gl_FragColor.a *= smoothstep(0.5f, 0.505f, distance);
#else
    gl_FragColor.a = vColor.a * texture2D(sDiffMap, vTexCoord).a;
#endif
}
