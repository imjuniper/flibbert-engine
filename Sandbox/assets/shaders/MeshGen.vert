#version 410 core

// Currently, include order is very important
#include "Include/Math.shader"
#include "Include/Random.shader"
#include "Include/Noise.shader"

layout (std140) uniform UniformBufferObject {
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

layout (std140) uniform Matrices
{
    mat4 u_Projection;
    mat4 u_View;
};

uniform mat4 u_Transform;
uniform vec3 u_CameraPosition;

layout(location = 0) in vec3 a_Position;

// This is what the vertex shader will output and send to the fragment shader.
layout(location = 1) out vec3 pos;
layout(location = 2) out vec3 cameraPos;

void main() {
    // The fragment shader also calculates the fractional brownian motion for pixel perfect normal vectors and lighting, so we pass the vertex position to the fragment shader
    pos = a_Position;
    cameraPos = u_CameraPosition;

    // Initial noise sample position offset and scaled by uniform variables
    vec3 noise_pos = (pos + vec3(_Offset.x, 0, _Offset.z)) / _Scale;

    // Calculate LODs for noise. should probably be done more dynamically and uh more maintainable
    float distanceFromCamera = distance(cameraPos, pos);
    int octaves0 = _Octaves;
    int octaves1 = max(int(octaves0 * 0.75), 1);
    int octaves2 = max(int(octaves1 * 0.75), 1);
    int octaves3 = max(int(octaves2 * 0.75), 1);
    int octaves = step(100, distanceFromCamera) == 1 ? (step(200, distanceFromCamera) == 1 ? (step(400, distanceFromCamera) == 1 ? octaves3 : octaves2) : octaves1) : octaves0;

    // The fractional brownian motion
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
    vec3 n = fbm(noise_pos.xz, params);

    // Adjust height of the vertex by fbm result scaled by final desired amplitude
    pos.y += _TerrainHeight * n.x + _TerrainHeight - _Offset.y;

    // Multiply final vertex position with model/view/projection matrices to convert to clip space
    gl_Position = u_Projection * u_View * u_Transform * vec4(pos, 1.0);
}
