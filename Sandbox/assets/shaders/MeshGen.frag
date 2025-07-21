#version 410 core

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

// These are the variables that we expect to receive from the vertex shader
layout(location = 1) in vec3 pos;

// This is what the fragment shader will output, usually just a pixel color
layout(location = 0) out vec4 frag_color;

#define PI 3.141592653589793238462

// UE's PseudoRandom function
// https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Shaders/Private/Random.ush
float pseudo(vec2 v) {
    v = fract(v/128.)*128. + vec2(-64.340622, -72.465622);
    return fract(dot(v.xyx * v.xyy, vec3(20.390625, 60.703125, 2.4281209)));
}

// Takes our xz positions and turns them into a random number between 0 and 1 using the above pseudo random function
float HashPosition(vec2 pos) {
    return pseudo(pos * vec2(_Seed, _Seed + 4));
}

// Generates a random gradient vector for the perlin noise lattice points, watch my perlin noise video for a more in depth explanation
vec2 RandVector(float seed) {
    float theta = seed * 360 * 2 - 360;
    theta += _GradientRotation;
    theta = theta * PI / 180.0;
    return normalize(vec2(cos(theta), sin(theta)));
}

// Normal smoothstep is cubic -- to avoid discontinuities in the gradient, we use a quintic interpolation instead as explained in my perlin noise video
vec2 quinticInterpolation(vec2 t) {
    return t * t * t * (t * (t * vec2(6) - vec2(15)) + vec2(10));
}

// Derivative of above function
vec2 quinticDerivative(vec2 t) {
    return vec2(30) * t * t * (t * (t - vec2(2)) + vec2(1));
}

// it's perlin noise that returns the noise in the x component and the derivatives in the yz components as explained in my perlin noise video
vec3 perlin_noise2D(vec2 pos) {
    vec2 latticeMin = floor(pos);
    vec2 latticeMax = ceil(pos);

    vec2 remainder = fract(pos);

    // Lattice Corners
    vec2 c00 = latticeMin;
    vec2 c10 = vec2(latticeMax.x, latticeMin.y);
    vec2 c01 = vec2(latticeMin.x, latticeMax.y);
    vec2 c11 = latticeMax;

    // Gradient Vectors assigned to each corner
    vec2 g00 = RandVector(HashPosition(c00));
    vec2 g10 = RandVector(HashPosition(c10));
    vec2 g01 = RandVector(HashPosition(c01));
    vec2 g11 = RandVector(HashPosition(c11));

    // Directions to position from lattice corners
    vec2 p0 = remainder;
    vec2 p1 = p0 - vec2(1.0);

    vec2 p00 = p0;
    vec2 p10 = vec2(p1.x, p0.y);
    vec2 p01 = vec2(p0.x, p1.y);
    vec2 p11 = p1;

    vec2 u = quinticInterpolation(remainder);
    vec2 du = quinticDerivative(remainder);

    float a = dot(g00, p00);
    float b = dot(g10, p10);
    float c = dot(g01, p01);
    float d = dot(g11, p11);

    // Expanded interpolation freaks of nature from https://iquilezles.org/articles/gradientnoise/
    float noise = a + u.x * (b - a) + u.y * (c - a) + u.x * u.y * (a - b - c + d);

    vec2 gradient = g00 + u.x * (g10 - g00) + u.y * (g01 - g00) + u.x * u.y * (g00 - g10 - g01 + g11) + du * (u.yx * (a - b - c + d) + vec2(b, c) - a);
    return vec3(noise, gradient);
}

// The fractional brownian motion that sums many noise values as explained in the video accompanying this project
vec3 fbm(vec2 pos) {
    float lacunarity = _Lacunarity;
    float amplitude = _InitialAmplitude;

    // height sum
    float height = 0.0;

    // derivative sum
    vec2 grad = vec2(0.0);

    // accumulated rotations
    mat2 m = mat2(1.0, 0.0,
    0.0, 1.0);

    // generate random angle variance if applicable
    float angle_variance = mix(_AngularVariance.x, _AngularVariance.y, HashPosition(vec2(_Seed, 827)));
    float theta = (_NoiseRotation + angle_variance) * PI / 180.0;

    // rotation matrix
    mat2 m2 = mat2(cos(theta), -sin(theta),
    sin(theta),  cos(theta));

    mat2 m2i = inverse(m2);

    for(int i = 0; i < _Octaves; ++i) {
        vec3 n = perlin_noise2D(pos);

        // add height scaled by current amplitude
        height += amplitude * n.x;

        // add gradient scaled by amplitude and transformed by accumulated rotations
        grad += amplitude * m * n.yz;

        // apply amplitude decay to reduce impact of next noise layer
        amplitude *= _AmplitudeDecay;

        // generate random angle variance if applicable
        angle_variance = mix(_AngularVariance.x, _AngularVariance.y, HashPosition(vec2(i * 419, _Seed)));
        theta = (_NoiseRotation + angle_variance) * PI / 180.0;

        // reconstruct rotation matrix, kind of a performance stink since this is technically expensive and doesn't need to be done if no random angle variance but whatever it's 2025
        m2 = mat2(cos(theta), -sin(theta),
        sin(theta),  cos(theta));

        m2i = inverse(m2);

        // generate frequency variance if applicable
        float freq_variance = mix(_FrequencyVarianceLowerBound, _FrequencyVarianceUpperBound, HashPosition(vec2(i * 422, _Seed)));

        // apply frequency adjustment to sample position for next noise layer
        pos = (lacunarity + freq_variance) * m2 * pos;
        m = (lacunarity + freq_variance) * m2i * m;
    }

    return vec3(height, grad);
}

void main() {
    // Recalculate initial noise sampling position same as vertex shader
    vec3 noise_pos = (pos + vec3(_Offset.x, 0, _Offset.z)) / _Scale;

    // Calculate fbm, we don't care about the height just the derivatives here for the normal vector so the ` + _TerrainHeight - _Offset.y` drops off as it isn't relevant to the derivative
    vec3 n = _TerrainHeight * fbm(noise_pos.xz);

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

    // most basic-ass fog, with some nice magic numbers instead of uniforms because i'm too lazy
    float zFar = 1000.0f;
    float zNear = 0.1f;
    float depth = zNear * zFar / (zFar + gl_FragCoord.z * (zNear - zFar));
    float fogFactor = exp2(-pow(depth * _FogDensity, 2));

    frag_color = mix(vec4(_FogColor, 1.0f), lit_srgb, fogFactor);
}
