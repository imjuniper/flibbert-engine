// Normal smoothstep is cubic -- to avoid discontinuities in the gradient, we use a quintic interpolation instead as explained in my perlin noise video
vec2 quinticInterpolation(vec2 t) {
    return t * t * t * (t * (t * vec2(6) - vec2(15)) + vec2(10));
}

// Derivative of above function
vec2 quinticDerivative(vec2 t) {
    return vec2(30) * t * t * (t * (t - vec2(2)) + vec2(1));
}

// it's perlin noise that returns the noise in the x component and the derivatives in the yz components as explained in Acerola's perlin noise video
vec3 perlin_noise2D(vec2 pos, float seed, float gradientRotation) {
    vec2 latticeMin = floor(pos);
    vec2 latticeMax = ceil(pos);

    vec2 remainder = fract(pos);

    // Lattice Corners
    vec2 c00 = latticeMin;
    vec2 c10 = vec2(latticeMax.x, latticeMin.y);
    vec2 c01 = vec2(latticeMin.x, latticeMax.y);
    vec2 c11 = latticeMax;

    // Gradient Vectors assigned to each corner
    vec2 g00 = RandVector(HashPosition(c00, seed), gradientRotation);
    vec2 g10 = RandVector(HashPosition(c10, seed), gradientRotation);
    vec2 g01 = RandVector(HashPosition(c01, seed), gradientRotation);
    vec2 g11 = RandVector(HashPosition(c11, seed), gradientRotation);

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

struct FbmParams {
    float seed;
    int octaves;
    float gradientRotation;
    float noiseRotation;
    float initialAmplitude;
    float amplitudeDecay;
    float lacunarity;
    vec2 angularVariance;
    float frequencyVarianceLowerBound;
    float frequencyVarianceUpperBound;
};

// The fractional brownian motion that sums many noise values as explained in the video accompanying this project
vec3 fbm(vec2 pos, FbmParams params) {
    float amplitude = params.initialAmplitude;

    // height sum
    float height = 0.0;

    // derivative sum
    vec2 grad = vec2(0.0);

    // accumulated rotations
    mat2 m = mat2(1.0, 0.0,
                  0.0, 1.0);

    // generate random angle variance if applicable
    float angle_variance = mix(params.angularVariance.x, params.angularVariance.y, HashPosition(vec2(params.seed, 827), params.seed));
    float theta = (params.noiseRotation + angle_variance) * PI / 180.0;

    // rotation matrix
    mat2 m2 = mat2(cos(theta), -sin(theta),
                   sin(theta),  cos(theta));

    mat2 m2i = inverse(m2);

    for(int i = 0; i < params.octaves; ++i) {
        vec3 n = perlin_noise2D(pos, params.seed, params.gradientRotation);

        // add height scaled by current amplitude
        height += amplitude * n.x;

        // add gradient scaled by amplitude and transformed by accumulated rotations
        grad += amplitude * m * n.yz;

        // apply amplitude decay to reduce impact of next noise layer
        amplitude *= params.amplitudeDecay;

        // generate random angle variance if applicable
        angle_variance = mix(params.angularVariance.x, params.angularVariance.y, HashPosition(vec2(i * 419, params.seed), params.seed));
        theta = (params.noiseRotation + angle_variance) * PI / 180.0;

        // reconstruct rotation matrix, kind of a performance stink since this is technically expensive and doesn't need to be done if no random angle variance but whatever it's 2025
        m2 = mat2(cos(theta), -sin(theta),
                  sin(theta),  cos(theta));

        m2i = inverse(m2);

        // generate frequency variance if applicable
        float freq_variance = mix(params.frequencyVarianceLowerBound, params.frequencyVarianceUpperBound, HashPosition(vec2(i * 422, params.seed), params.seed));

        // apply frequency adjustment to sample position for next noise layer
        pos = (params.lacunarity + freq_variance) * m2 * pos;
        m = (params.lacunarity + freq_variance) * m2i * m;
    }

    return vec3(height, grad);
}
