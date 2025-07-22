#version 410 core

// Currently, include order is very important
#include "Include/Math.shader"
#include "Include/Random.shader"
#include "Include/Noise.shader"

layout (std140) uniform MeshGenData {
    vec3 _LightDirection;
    float _GradientRotation;
    float _NoiseRotation;
    float _TerrainHeight;
    vec2 _AngularVariance;
    float _Scale;
    int _Octaves;
    float _AmplitudeDecay;
    float _NormalStrength;
    vec3 _Offset;
    float _Seed;
    float _InitialAmplitude;
    float _Lacunarity;
    vec2 _SlopeRange;
    vec4 _LowSlopeColor;
    vec4 _HighSlopeColor;
    float _FrequencyVarianceLowerBound;
    float _FrequencyVarianceUpperBound;
    float _SlopeDamping;
    vec3 _AmbientLight;
    float _FogDensity;
    vec3 _FogColor;
};

layout (std140) uniform PerFrameData
{
    mat4 u_View;
    mat4 u_Projection;
    vec3 u_ViewPosition;
};

// These are the variables that we expect to receive from the vertex shader
in VertexData
{
    vec3 Position;
} vertexData;

// This is what the fragment shader will output, usually just a pixel color
layout(location = 0) out vec4 frag_color;

void main() {
    // Recalculate initial noise sampling position same as vertex shader
    vec3 noise_pos = (vertexData.Position + vec3(_Offset.x, 0, _Offset.z)) / _Scale;

    // Calculate LODs for noise. should probably be done more dynamically and uh more maintainable
    float distanceFromCamera = distance(u_ViewPosition, vertexData.Position);
    int octaves0 = _Octaves;
    int octaves1 = max(int(octaves0 * 0.75), 1);
    int octaves2 = max(int(octaves1 * 0.75), 1);
    int octaves3 = max(int(octaves2 * 0.75), 1);
    int octaves = step(100, distanceFromCamera) == 1 ? (step(200, distanceFromCamera) == 1 ? (step(400, distanceFromCamera) == 1 ? octaves3 : octaves2) : octaves1) : octaves0;

    // Calculate fbm, we don't care about the height just the derivatives here for the normal vector so the ` + _TerrainHeight - _Offset.y` drops off as it isn't relevant to the derivative
    FbmParams params = FbmParams(
        _Seed,
        octaves,
        _GradientRotation,
        _NoiseRotation,
        _InitialAmplitude,
        _AmplitudeDecay,
        _Lacunarity,
        _AngularVariance,
        _FrequencyVarianceLowerBound,
        _FrequencyVarianceUpperBound
    );
    vec3 n = _TerrainHeight * fbm(noise_pos.xz, params);

    // To more easily customize the color slope blending this is a separate normal vector with its horizontal gradients significantly reduced so the normal points upwards more
    vec3 slope_normal = normalize(vec3(-n.y, 1, -n.z) * vec3(_SlopeDamping, 1, _SlopeDamping));

    // Use the slope of the above normal to create the blend value between the two terrain colors
    float material_blend_factor = smoothstep(_SlopeRange.x, _SlopeRange.y, 1 - slope_normal.y);

    // Blend between the two terrain colors
    vec4 albedo = mix(_LowSlopeColor, _HighSlopeColor, vec4(material_blend_factor));

    // This is the actual surface normal vector
    vec3 normal = normalize(vec3(-n.y, 1, -n.z));

    // Lambertian diffuse, negative dot product values clamped off because negative light doesn't exist
    float ndotl = clamp(dot(_LightDirection, normal), 0, 1);

    // Direct light cares about the diffuse result, ambient light does not
    vec4 direct_light = albedo * ndotl;
    vec4 ambient_light = albedo * vec4(_AmbientLight, 1);

    // Combine lighting values, clip to prevent pixel values greater than 1 which would really really mess up the gamma correction below
    vec4 lit = clamp(direct_light + ambient_light, vec4(0), vec4(1));

    // Convert from linear rgb to srgb for proper color output, ideally you'd do this as some final post processing effect because otherwise you will need to revert this gamma correction elsewhere
    vec4 lit_srgb = pow(lit, vec4(2.2));

    // most basic-ass fog
    float fogFactor = exp2(-pow(distance(u_ViewPosition, vertexData.Position) * _FogDensity, 2));

    frag_color = mix(vec4(_FogColor, 1.0f), lit_srgb, fogFactor);
}
