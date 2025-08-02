/*
 * Copyright (c) 2025 Samuel Ugochukwu <sammycageagle@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#ifndef PLUTOFILTER_H
#define PLUTOFILTER_H

#include <stdint.h>

#define PLUTOFILTER_VERSION 1

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PLUTOFILTER_API
#ifdef PLUTOFILTER_BUILD_STATIC
#define PLUTOFILTER_API static
#else
#define PLUTOFILTER_API extern
#endif
#endif

/**
 * @brief Represents a 2D image surface in ARGB32 premultiplied format.
 * 
 * Each pixel is a 32-bit unsigned integer with channels ordered as: alpha, red, green, blue.
 * The red, green, and blue channels are premultiplied by the alpha channel.
 * That is, red = red * alpha / 255, and similarly for green and blue.
 * 
 * The pixel data is stored in row-major order. Each row begins at a multiple of `stride`.
 */
typedef struct {
    /**
     * @brief Pointer to the pixel buffer.
     *
     * Must point to at least `stride * height` elements in ARGB32 premultiplied format.
     */
    uint32_t* pixels;

    /**
     * @brief The width of the surface in pixels.
     */
    uint16_t width;

    /**
     * @brief The height of the surface in pixels.
     */
    uint16_t height;

    /**
     * @brief The number of pixels per row.
     *
     * Must be greater than or equal to `width`.
     */
    uint32_t stride;
} plutofilter_surface_t;

/**
 * @brief Creates a surface from a raw pixel buffer.
 * 
 * @param pixels Pointer to the pixel buffer in ARGB32 premultiplied format.
 * @param width The width of the surface in pixels.
 * @param height The height of the surface in pixels.
 * @param stride The number of pixels per row (must be greater than or equal to width).
 * @return A plutofilter_surface_t representing the given pixel buffer.
 */
plutofilter_surface_t plutofilter_surface_make(uint32_t* pixels, uint16_t width, uint16_t height, uint32_t stride);

/**
 * @brief Creates a subregion of an existing surface.
 * 
 * @param surface The source surface.
 * @param x The horizontal offset of the subregion, in pixels.
 * @param y The vertical offset of the subregion, in pixels.
 * @param width The width of the subregion in pixels.
 * @param height The height of the subregion in pixels.
 * @return A surface referencing the specified subregion, clipped to the bounds of the source surface.
 */
plutofilter_surface_t plutofilter_surface_make_sub(plutofilter_surface_t surface, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
 * @brief Applies a 5x4 color transformation matrix to each pixel in the input surface.
 * 
 * The transformation is applied in-place from `in` to `out`, using a matrix that operates
 * on premultiplied ARGB channels. Each output pixel is computed as:
 * 
 *     [R']   [ m0  m1  m2  m3  m4 ]   [R]
 *     [G'] = [ m5  m6  m7  m8  m9 ] x [G]
 *     [B']   [m10 m11 m12 m13 m14]   [B]
 *     [A']   [m15 m16 m17 m18 m19]   [A]
 * 
 * The matrix must be provided in row-major order as a flat array of 20 floats.
 * The input and output surfaces may refer to the same buffer.
 * 
 * @param in The input surface (read-only if different from out).
 * @param out The output surface.
 * @param matrix A 5x4 color matrix represented as a 20-element float array.
 */
PLUTOFILTER_API void plutofilter_color_transform(plutofilter_surface_t in, plutofilter_surface_t out, const float matrix[20]);

/**
 * @brief Adjusts the opacity of each pixel by a uniform amount.
 * 
 * Multiplies the alpha channel by the given amount. The result is clamped to the range [0, 255].
 * The input and output surfaces may refer to the same buffer.
 * 
 * @param in The input surface.
 * @param out The output surface.
 * @param amount The opacity multiplier (0 for fully transparent, 1 for unchanged).
 */
PLUTOFILTER_API void plutofilter_color_transform_opacity(plutofilter_surface_t in, plutofilter_surface_t out, float amount);

/**
 * @brief Adjusts the brightness of each pixel by a uniform amount.
 * 
 * Multiplies the color channels (red, green, blue) by the given amount. The result is clamped to the range [0, 255].
 * The alpha channel is not affected. The input and output surfaces may refer to the same buffer.
 * 
 * @param in The input surface.
 * @param out The output surface.
 * @param amount The brightness multiplier (1 for unchanged, <1 to darken, >1 to brighten).
 */
PLUTOFILTER_API void plutofilter_color_transform_brightness(plutofilter_surface_t in, plutofilter_surface_t out, float amount);

/**
 * @brief Inverts the color channels of each pixel by a uniform amount.
 * 
 * Linearly interpolates between the original color and its inverse based on the given amount.
 * The alpha channel is not affected. The input and output surfaces may refer to the same buffer.
 * 
 * @param in The input surface.
 * @param out The output surface.
 * @param amount The inversion amount (0 for unchanged, 1 for fully inverted).
 */
PLUTOFILTER_API void plutofilter_color_transform_invert(plutofilter_surface_t in, plutofilter_surface_t out, float amount);

/**
 * @brief Adjusts the contrast of each pixel by a uniform amount.
 * 
 * Scales the color channels (red, green, blue) away from or toward the midpoint (0.5) by the given amount.
 * The alpha channel is not affected. The input and output surfaces may refer to the same buffer.
 * 
 * @param in The input surface.
 * @param out The output surface.
 * @param amount The contrast multiplier (1 for unchanged, <1 to reduce contrast, >1 to increase contrast).
 */
PLUTOFILTER_API void plutofilter_color_transform_contrast(plutofilter_surface_t in, plutofilter_surface_t out, float amount);

/**
 * @brief Adjusts the saturation of each pixel by a uniform amount.
 * 
 * Modifies the intensity of color while preserving luminance. A value of 0 produces a fully desaturated result.
 * The alpha channel is not affected. The input and output surfaces may refer to the same buffer.
 * 
 * @param in The input surface.
 * @param out The output surface.
 * @param amount The saturation multiplier (1 for unchanged, 0 for fully desaturated, >1 to increase saturation).
 */
PLUTOFILTER_API void plutofilter_color_transform_saturate(plutofilter_surface_t in, plutofilter_surface_t out, float amount);

/**
 * @brief Converts each pixel toward grayscale by a uniform amount.
 * 
 * Reduces the influence of color while preserving luminance. A value of 1 produces a fully grayscale result.
 * The alpha channel is not affected. The input and output surfaces may refer to the same buffer.
 * 
 * @param in The input surface.
 * @param out The output surface.
 * @param amount The grayscale amount (0 for unchanged, 1 for fully grayscale).
 */
PLUTOFILTER_API void plutofilter_color_transform_grayscale(plutofilter_surface_t in, plutofilter_surface_t out, float amount);

/**
 * @brief Applies a sepia tone to each pixel by a uniform amount.
 * 
 * Shifts the colors toward warm brown tones, simulating the appearance of old photographs.
 * A value of 1 produces a fully sepia-toned result. The alpha channel is not affected.
 * The input and output surfaces may refer to the same buffer.
 * 
 * @param in The input surface.
 * @param out The output surface.
 * @param amount The sepia amount (0 for unchanged, 1 for fully sepia).
 */
PLUTOFILTER_API void plutofilter_color_transform_sepia(plutofilter_surface_t in, plutofilter_surface_t out, float amount);

/**
 * @brief Rotates the hue of each pixel by a given angle.
 * 
 * Shifts the hue component of the color while preserving luminance and saturation.
 * The angle is specified in degrees and wraps around automatically.
 * The alpha channel is not affected. The input and output surfaces may refer to the same buffer.
 * 
 * @param in The input surface.
 * @param out The output surface.
 * @param angle The hue rotation angle in degrees (0 for unchanged, 360 for full rotation).
 */
PLUTOFILTER_API void plutofilter_color_transform_hue_rotate(plutofilter_surface_t in, plutofilter_surface_t out, float angle);

/**
 * @brief Sets the alpha channel of each pixel based on its luminance.
 * 
 * Replaces the alpha channel with the computed luminance of the color channels.
 * The RGB channels are set to zero. Luminance is calculated using the formula:
 * 
 *     alpha = 0.2126 * R + 0.7152 * G + 0.0722 * B
 * 
 * The input and output surfaces may refer to the same buffer.
 * 
 * @param in The input surface.
 * @param out The output surface.
 */
PLUTOFILTER_API void plutofilter_color_transform_luminance_to_alpha(plutofilter_surface_t in, plutofilter_surface_t out);

/**
 * @brief Converts the color channels from sRGB to linear RGB.
 * 
 * Applies gamma correction to convert red, green, and blue channels from sRGB to linear space.
 * The alpha channel is not affected. The input and output surfaces may refer to the same buffer.
 * 
 * @param in The input surface.
 * @param out The output surface.
 */
PLUTOFILTER_API void plutofilter_color_transform_srgb_to_linear_rgb(plutofilter_surface_t in, plutofilter_surface_t out);

/**
 * @brief Converts the color channels from linear RGB to sRGB.
 * 
 * Applies gamma encoding to convert red, green, and blue channels from linear space to sRGB.
 * The alpha channel is not affected. The input and output surfaces may refer to the same buffer.
 * 
 * @param in The input surface.
 * @param out The output surface.
 */
PLUTOFILTER_API void plutofilter_color_transform_linear_rgb_to_srgb(plutofilter_surface_t in, plutofilter_surface_t out);

/**
 * @brief Applies a Gaussian blur to the input surface.
 * 
 * Performs separable convolution with a Gaussian kernel along the X and Y axes.
 * The blur strength is controlled by the standard deviation parameters.
 * 
 * The input and output surfaces may refer to the same buffer.
 * 
 * @param in The input surface.
 * @param out The output surface.
 * @param std_deviation_x The standard deviation of the blur along the X axis.
 * @param std_deviation_y The standard deviation of the blur along the Y axis.
 */
PLUTOFILTER_API void plutofilter_gaussian_blur(plutofilter_surface_t in, plutofilter_surface_t out, float std_deviation_x, float std_deviation_y);

/**
 * @brief Blend modes for combining source and backdrop surfaces.
 */
typedef enum plutofilter_blend_mode {
    PLUTOFILTER_BLEND_MODE_NORMAL,     /**< Standard alpha compositing (source over backdrop) */
    PLUTOFILTER_BLEND_MODE_MULTIPLY,   /**< Multiplies source and backdrop, producing a darker result */
    PLUTOFILTER_BLEND_MODE_SCREEN,     /**< Inverts, multiplies, then inverts result, producing a lighter result */
    PLUTOFILTER_BLEND_MODE_OVERLAY,    /**< Multiplies or screens depending on backdrop tone */
    PLUTOFILTER_BLEND_MODE_DARKEN,     /**< Chooses the darker of source and backdrop */
    PLUTOFILTER_BLEND_MODE_LIGHTEN,    /**< Chooses the lighter of source and backdrop */
    PLUTOFILTER_BLEND_MODE_COLOR_DODGE,/**< Brightens backdrop in proportion to source */
    PLUTOFILTER_BLEND_MODE_COLOR_BURN, /**< Darkens backdrop in proportion to source */
    PLUTOFILTER_BLEND_MODE_HARD_LIGHT, /**< Multiplies or screens depending on source tone */
    PLUTOFILTER_BLEND_MODE_SOFT_LIGHT, /**< Darkens or lightens depending on source tone */
    PLUTOFILTER_BLEND_MODE_DIFFERENCE, /**< Subtracts darker from lighter color */
    PLUTOFILTER_BLEND_MODE_EXCLUSION   /**< Similar to difference but with lower contrast */
} plutofilter_blend_mode_t;

/**
 * @brief Blends two input surfaces using the specified blend mode.
 * 
 * Applies the selected blend mode to combine `in1` (source) over `in2` (backdrop).
 * The output surface may refer to either input.
 * 
 * @param in1 The source surface.
 * @param in2 The backdrop surface.
 * @param out The output surface.
 * @param mode The blend mode to apply.
 */
PLUTOFILTER_API void plutofilter_blend(plutofilter_surface_t in1, plutofilter_surface_t in2, plutofilter_surface_t out, plutofilter_blend_mode_t mode);

/**
 * @brief Compositing operators for combining source and backdrop surfaces.
 */
typedef enum plutofilter_composite_operator {
    PLUTOFILTER_COMPOSITE_OPERATOR_OVER, /**< Display source over backdrop */
    PLUTOFILTER_COMPOSITE_OPERATOR_IN,   /**< Keep only portions of source within backdrop */
    PLUTOFILTER_COMPOSITE_OPERATOR_OUT,  /**< Keep only portions of source outside backdrop */
    PLUTOFILTER_COMPOSITE_OPERATOR_ATOP, /**< Display source over backdrop, preserving backdrop alpha */
    PLUTOFILTER_COMPOSITE_OPERATOR_XOR   /**< Keep non‑overlapping parts of both source and backdrop */
} plutofilter_composite_operator_t;

/**
 * @brief Composites two input surfaces using the specified operator.
 * 
 * Applies the selected compositing rule to combine `in1` (source) over `in2` (backdrop).
 * The output surface may refer to the same buffer as either input.
 * 
 * @param in1 The source surface.
 * @param in2 The backdrop surface.
 * @param out The output surface.
 * @param op The compositing operator to apply.
 */
PLUTOFILTER_API void plutofilter_composite(plutofilter_surface_t in1, plutofilter_surface_t in2, plutofilter_surface_t out, plutofilter_composite_operator_t op);

/**
 * @brief Composites two input surfaces using an arithmetic combination of their color components.
 * 
 * Computes each output channel as:  
 * result = k1 * in1 * in2 + k2 * in1 + k3 * in2 + k4
 * The result is clamped to the valid range.
 *
 * The output surface may refer to the same buffer as either input.
 *
 * @param in1 The source surface.
 * @param in2 The backdrop surface.
 * @param out The output surface.
 * @param k1 The coefficient for in1 * in2.
 * @param k2 The coefficient for in1.
 * @param k3 The coefficient for in2.
 * @param k4 The constant bias term.
 */
PLUTOFILTER_API void plutofilter_composite_arithmetic(plutofilter_surface_t in1, plutofilter_surface_t in2, plutofilter_surface_t out, float k1, float k2, float k3, float k4);

#ifdef __cplusplus
}
#endif

#endif // PLUTOFILTER_H

#ifdef PLUTOFILTER_IMPLEMENTATION

#include <math.h>

plutofilter_surface_t plutofilter_surface_make(uint32_t* pixels, uint16_t width, uint16_t height, uint32_t stride)
{
    plutofilter_surface_t surface;

    surface.pixels = pixels;
    surface.width = width;
    surface.height = height;
    surface.stride = stride;

    return surface;
}

plutofilter_surface_t plutofilter_surface_make_sub(plutofilter_surface_t surface, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    if(x > surface.width) x = surface.width;
    if(y > surface.height) y = surface.height;
    if(x + width > surface.width) width = surface.width - x;
    if(y + height > surface.height) height = surface.height - y;

    return plutofilter_surface_make(surface.pixels + (y * surface.stride + x), width, height, surface.stride);
}

#define PLUTOFILTER_ALPHA(pixel) (((pixel) >> 24) & 0xFF)
#define PLUTOFILTER_RED(pixel) (((pixel) >> 16) & 0xFF)
#define PLUTOFILTER_GREEN(pixel) (((pixel) >> 8) & 0xFF)
#define PLUTOFILTER_BLUE(pixel) (((pixel) >> 0) & 0xFF)

#define PLUTOFILTER_UNPACK_PIXEL(pixel, r, g, b, a) \
    do { \
        (r) = PLUTOFILTER_RED(pixel); \
        (g) = PLUTOFILTER_GREEN(pixel); \
        (b) = PLUTOFILTER_BLUE(pixel); \
        (a) = PLUTOFILTER_ALPHA(pixel); \
    } while(0)

#define PLUTOFILTER_GET_PIXEL(surface, x, y) \
    ((surface).pixels[(y) * (surface).stride + (x)])

#define PLUTOFILTER_LOAD_PIXEL(in, x, y, r, g, b, a) \
    do { \
        uint32_t pixel = PLUTOFILTER_GET_PIXEL(in, x, y); \
        PLUTOFILTER_UNPACK_PIXEL(pixel, r, g, b, a); \
    } while(0)

#define PLUTOFILTER_INIT_LOAD_PIXEL(in, x, y, r, g, b, a) \
    uint32_t r, g, b, a; \
    PLUTOFILTER_LOAD_PIXEL(in, x, y, r, g, b, a)

#define PLUTOFILTER_PACK_PIXEL(r, g, b, a) \
    (((uint32_t)(a) << 24) | ((uint32_t)(r) << 16) | ((uint32_t)(g) << 8) | (uint32_t)(b))

#define PLUTOFILTER_STORE_PIXEL(out, x, y, r, g, b, a) \
    (PLUTOFILTER_GET_PIXEL(out, x, y) = PLUTOFILTER_PACK_PIXEL(r, g, b, a))

#define PLUTOFILTER_PREMULTIPLY_PIXEL(r, g, b, a) \
    do { \
        (r) = ((r) * ((a) + 1)) >> 8; \
        (g) = ((g) * ((a) + 1)) >> 8; \
        (b) = ((b) * ((a) + 1)) >> 8; \
    } while(0)

#define PLUTOFILTER_UNPREMULTIPLY_PIXEL(r, g, b, a) \
    do { \
        if((a)) { \
            (r) = (255 * (r)) / (a); \
            (g) = (255 * (g)) / (a); \
            (b) = (255 * (b)) / (a); \
        } else { \
            (r) = (g) = (b) = 0; \
        } \
    } while(0)

#define PLUTOFILTER_CLAMP(v, lo, hi) \
    ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

#define PLUTOFILTER_CLAMP_PIXEL(pixel) \
    ((uint8_t)(PLUTOFILTER_CLAMP(pixel, 0, 255)))

#define PLUTOFILTER_OVERLAP_SURFACE(a, b) \
    do { \
        uint16_t __width = (a).width; \
        uint16_t __height = (a).height; \
        if((b).width < __width) __width = (b).width; \
        if((b).height < __height) __height = (b).height; \
        (a).width = (b).width = __width; \
        (a).height = (b).height = __height; \
    } while(0)

#define PLUTOFILTER_OVERLAP_SURFACE3(a, b, c) \
    do { \
        uint16_t __width = (a).width; \
        uint16_t __height = (a).height; \
        if((b).width < __width) __width = (b).width; \
        if((c).width < __width) __width = (c).width; \
        if((b).height < __height) __height = (b).height; \
        if((c).height < __height) __height = (c).height; \
        (a).width = (b).width = (c).width = __width; \
        (a).height = (b).height = (c).height = __height; \
    } while(0)

void plutofilter_color_transform(plutofilter_surface_t in, plutofilter_surface_t out, const float matrix[20])
{
    PLUTOFILTER_OVERLAP_SURFACE(in, out);

    for(int y = 0; y < out.height; y++) {
        for(int x = 0; x < out.width; x++) {
            PLUTOFILTER_INIT_LOAD_PIXEL(in, x, y, r, g, b, a);
            PLUTOFILTER_UNPREMULTIPLY_PIXEL(r, g, b, a);

            float rr = r * matrix[ 0] + g * matrix[ 1] + b * matrix[ 2] + a * matrix[ 3] + matrix[ 4] * 255;
            float gg = r * matrix[ 5] + g * matrix[ 6] + b * matrix[ 7] + a * matrix[ 8] + matrix[ 9] * 255;
            float bb = r * matrix[10] + g * matrix[11] + b * matrix[12] + a * matrix[13] + matrix[14] * 255;
            float aa = r * matrix[15] + g * matrix[16] + b * matrix[17] + a * matrix[18] + matrix[19] * 255;

            r = PLUTOFILTER_CLAMP_PIXEL(rr);
            g = PLUTOFILTER_CLAMP_PIXEL(gg);
            b = PLUTOFILTER_CLAMP_PIXEL(bb);
            a = PLUTOFILTER_CLAMP_PIXEL(aa);

            PLUTOFILTER_PREMULTIPLY_PIXEL(r, g, b, a);
            PLUTOFILTER_STORE_PIXEL(out, x, y, r, g, b, a);
        }
    }
}

void plutofilter_color_transform_opacity(plutofilter_surface_t in, plutofilter_surface_t out, float amount)
{
    const float matrix[] = {
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, amount, 0.0f
    };

    plutofilter_color_transform(in, out, matrix);
}

void plutofilter_color_transform_brightness(plutofilter_surface_t in, plutofilter_surface_t out, float amount)
{
    const float matrix[] = {
        amount, 0.0f,   0.0f,   0.0f, 0.0f,
        0.0f,   amount, 0.0f,   0.0f, 0.0f,
        0.0f,   0.0f,   amount, 0.0f, 0.0f,
        0.0f,   0.0f,   0.0f,   1.0f, 0.0f
    };

    plutofilter_color_transform(in, out, matrix);
}

void plutofilter_color_transform_invert(plutofilter_surface_t in, plutofilter_surface_t out, float amount)
{
    const float scale = 1.0f - 2.0f * amount;
    const float matrix[] = {
        scale, 0.0f,  0.0f,  0.0f, amount,
        0.0f,  scale, 0.0f,  0.0f, amount,
        0.0f,  0.0f,  scale, 0.0f, amount,
        0.0f,  0.0f,  0.0f,  1.0f, 0.0f
    };

    plutofilter_color_transform(in, out, matrix);
}

void plutofilter_color_transform_contrast(plutofilter_surface_t in, plutofilter_surface_t out, float amount)
{
    const float offset = (1.0f - amount) * 0.5f;
    const float matrix[] = {
        amount, 0.0f,   0.0f,   0.0f, offset,
        0.0f,   amount, 0.0f,   0.0f, offset,
        0.0f,   0.0f,   amount, 0.0f, offset,
        0.0f,   0.0f,   0.0f,   1.0f, 0.0f
    };

    plutofilter_color_transform(in, out, matrix);
}

void plutofilter_color_transform_saturate(plutofilter_surface_t in, plutofilter_surface_t out, float amount)
{
    const float matrix[] = {
        0.213f + 0.787f * amount,
        0.715f - 0.715f * amount,
        0.072f - 0.072f * amount,
        0.0f,
        0.0f,

        0.213f - 0.213f * amount,
        0.715f + 0.285f * amount,
        0.072f - 0.072f * amount,
        0.0f,
        0.0f,

        0.213f - 0.213f * amount,
        0.715f - 0.715f * amount,
        0.072f + 0.928f * amount,
        0.0f,
        0.0f,

        0.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f
    };

    plutofilter_color_transform(in, out, matrix);
}

void plutofilter_color_transform_grayscale(plutofilter_surface_t in, plutofilter_surface_t out, float amount)
{
    const float inv_amount = 1.0f - amount;
    const float matrix[] = {
        inv_amount + amount * 0.2126f,
        amount * 0.7152f,
        amount * 0.0722f,
        0.0f,
        0.0f,

        amount * 0.2126f,
        inv_amount + amount * 0.7152f,
        amount * 0.0722f,
        0.0f,
        0.0f,

        amount * 0.2126f,
        amount * 0.7152f,
        inv_amount + amount * 0.0722f,
        0.0f,
        0.0f,

        0.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f
    };

    plutofilter_color_transform(in, out, matrix);
}

void plutofilter_color_transform_sepia(plutofilter_surface_t in, plutofilter_surface_t out, float amount)
{
    const float inv_amount = 1.0f - amount;
    const float matrix[] = {
        0.393f + 0.607f * inv_amount,
        0.769f - 0.769f * inv_amount,
        0.189f - 0.189f * inv_amount,
        0.0f,
        0.0f,

        0.349f - 0.349f * inv_amount,
        0.686f + 0.314f * inv_amount,
        0.168f - 0.168f * inv_amount,
        0.0f,
        0.0f,

        0.272f - 0.272f * inv_amount,
        0.534f - 0.534f * inv_amount,
        0.131f + 0.869f * inv_amount,
        0.0f,
        0.0f,

        0.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f
    };

    plutofilter_color_transform(in, out, matrix);
}

static inline float plutofilter__deg2rad(float angle) { return angle * (3.14159265358979323846f / 180.0f); }

void plutofilter_color_transform_hue_rotate(plutofilter_surface_t in, plutofilter_surface_t out, float angle)
{
    const float a1 = cos(plutofilter__deg2rad(angle));
    const float a2 = sin(plutofilter__deg2rad(angle));
    const float matrix[] = {
        0.213f + a1 * 0.787f - a2 * 0.213f,
        0.715f - a1 * 0.715f - a2 * 0.715f,
        0.072f - a1 * 0.072f + a2 * 0.928f,
        0.0f,
        0.0f,

        0.213f - a1 * 0.213f + a2 * 0.143f,
        0.715f + a1 * 0.285f + a2 * 0.140f,
        0.072f - a1 * 0.072f - a2 * 0.283f,
        0.0f,
        0.0f,

        0.213f - a1 * 0.213f - a2 * 0.787f,
        0.715f - a1 * 0.715f + a2 * 0.715f,
        0.072f + a1 * 0.928f + a2 * 0.072f,
        0.0f,
        0.0f,

        0.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f
    };

    plutofilter_color_transform(in, out, matrix);
}

void plutofilter_color_transform_luminance_to_alpha(plutofilter_surface_t in, plutofilter_surface_t out)
{
    PLUTOFILTER_OVERLAP_SURFACE(in, out);

    for(int y = 0; y < out.height; y++) {
        for(int x = 0; x < out.width; x++) {
            PLUTOFILTER_INIT_LOAD_PIXEL(in, x, y, r, g, b, a);
            PLUTOFILTER_UNPREMULTIPLY_PIXEL(r, g, b, a);

            float l = r * 0.2125f + g * 0.7154f + b * 0.0721f;

            PLUTOFILTER_STORE_PIXEL(out, x, y, 0, 0, 0, PLUTOFILTER_CLAMP_PIXEL(l));
        }
    }
}

static const uint8_t PLUTOFILTER_SRGB_TO_LINEAR_RGB_TABLE[256] = {
    0,   0,   0,   0,   0,   0,  0,    1,   1,   1,   1,   1,   1,   1,   1,   1,
    1,   1,   2,   2,   2,   2,  2,    2,   2,   2,   3,   3,   3,   3,   3,   3,
    4,   4,   4,   4,   4,   5,  5,    5,   5,   6,   6,   6,   6,   7,   7,   7,
    8,   8,   8,   8,   9,   9,  9,   10,  10,  10,  11,  11,  12,  12,  12,  13,
    13,  13,  14,  14,  15,  15,  16,  16,  17,  17,  17,  18,  18,  19,  19,  20,
    20,  21,  22,  22,  23,  23,  24,  24,  25,  25,  26,  27,  27,  28,  29,  29,
    30,  30,  31,  32,  32,  33,  34,  35,  35,  36,  37,  37,  38,  39,  40,  41,
    41,  42,  43,  44,  45,  45,  46,  47,  48,  49,  50,  51,  51,  52,  53,  54,
    55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,
    71,  72,  73,  74,  76,  77,  78,  79,  80,  81,  82,  84,  85,  86,  87,  88,
    90,  91,  92,  93,  95,  96,  97,  99, 100, 101, 103, 104, 105, 107, 108, 109,
    111, 112, 114, 115, 116, 118, 119, 121, 122, 124, 125, 127, 128, 130, 131, 133,
    134, 136, 138, 139, 141, 142, 144, 146, 147, 149, 151, 152, 154, 156, 157, 159,
    161, 163, 164, 166, 168, 170, 171, 173, 175, 177, 179, 181, 183, 184, 186, 188,
    190, 192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214, 216, 218, 220,
    222, 224, 226, 229, 231, 233, 235, 237, 239, 242, 244, 246, 248, 250, 253, 255,
};

#define PLUTOFILTER_SRGB_TO_LINEAR_RGB(r, g, b) \
    do { \
        (r) = PLUTOFILTER_SRGB_TO_LINEAR_RGB_TABLE[r]; \
        (g) = PLUTOFILTER_SRGB_TO_LINEAR_RGB_TABLE[g]; \
        (b) = PLUTOFILTER_SRGB_TO_LINEAR_RGB_TABLE[b]; \
    } while(0)

void plutofilter_color_transform_srgb_to_linear_rgb(plutofilter_surface_t in, plutofilter_surface_t out)
{
    PLUTOFILTER_OVERLAP_SURFACE(in, out);

    for(int y = 0; y < out.height; y++) {
        for(int x = 0; x < out.width; x++) {
            PLUTOFILTER_INIT_LOAD_PIXEL(in, x, y, r, g, b, a);
            PLUTOFILTER_UNPREMULTIPLY_PIXEL(r, g, b, a);

            PLUTOFILTER_SRGB_TO_LINEAR_RGB(r, g, b);

            PLUTOFILTER_PREMULTIPLY_PIXEL(r, g, b, a);
            PLUTOFILTER_STORE_PIXEL(out, x, y, r, g, b, a);
        }
    }
}

static const uint8_t PLUTOFILTER_LINEAR_RGB_TO_SRGB_TABLE[256] = {
    0,  13,  22,  28,  34,  38,  42,  46,  50,  53,  56,  59,  61,  64,  66,  69,
    71,  73,  75,  77,  79,  81,  83,  85,  86,  88,  90,  92,  93,  95,  96,  98,
    99, 101, 102, 104, 105, 106, 108, 109, 110, 112, 113, 114, 115, 117, 118, 119,
    120, 121, 122, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
    137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 148, 149, 150, 151,
    152, 153, 154, 155, 155, 156, 157, 158, 159, 159, 160, 161, 162, 163, 163, 164,
    165, 166, 167, 167, 168, 169, 170, 170, 171, 172, 173, 173, 174, 175, 175, 176,
    177, 178, 178, 179, 180, 180, 181, 182, 182, 183, 184, 185, 185, 186, 187, 187,
    188, 189, 189, 190, 190, 191, 192, 192, 193, 194, 194, 195, 196, 196, 197, 197,
    198, 199, 199, 200, 200, 201, 202, 202, 203, 203, 204, 205, 205, 206, 206, 207,
    208, 208, 209, 209, 210, 210, 211, 212, 212, 213, 213, 214, 214, 215, 215, 216,
    216, 217, 218, 218, 219, 219, 220, 220, 221, 221, 222, 222, 223, 223, 224, 224,
    225, 226, 226, 227, 227, 228, 228, 229, 229, 230, 230, 231, 231, 232, 232, 233,
    233, 234, 234, 235, 235, 236, 236, 237, 237, 238, 238, 238, 239, 239, 240, 240,
    241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246, 246, 246, 247, 247, 248,
    248, 249, 249, 250, 250, 251, 251, 251, 252, 252, 253, 253, 254, 254, 255, 255,
};

#define PLUTOFILTER_LINEAR_RGB_TO_SRGB(r, g, b) \
    do { \
        (r) = PLUTOFILTER_LINEAR_RGB_TO_SRGB_TABLE[r]; \
        (g) = PLUTOFILTER_LINEAR_RGB_TO_SRGB_TABLE[g]; \
        (b) = PLUTOFILTER_LINEAR_RGB_TO_SRGB_TABLE[b]; \
    } while(0)

void plutofilter_color_transform_linear_rgb_to_srgb(plutofilter_surface_t in, plutofilter_surface_t out)
{
    PLUTOFILTER_OVERLAP_SURFACE(in, out);

    for(int y = 0; y < out.height; y++) {
        for(int x = 0; x < out.width; x++) {
            PLUTOFILTER_INIT_LOAD_PIXEL(in, x, y, r, g, b, a);
            PLUTOFILTER_UNPREMULTIPLY_PIXEL(r, g, b, a);

            PLUTOFILTER_LINEAR_RGB_TO_SRGB(r, g, b);

            PLUTOFILTER_PREMULTIPLY_PIXEL(r, g, b, a);
            PLUTOFILTER_STORE_PIXEL(out, x, y, r, g, b, a);
        }
    }
}

#define PLUTOFILTER_BLUR_STORE_PIXEL(out, x, y, r, g, b, a, k) \
    PLUTOFILTER_STORE_PIXEL(out, x, y, (r) / (k), (g) / (k), (b) / (k), (a) / (k))

#define PLUTOFILTER_MIN(a, b) ((a) < (b) ? (a) : (b))
#define PLUTOFILTER_MAX(a, b) ((a) > (b) ? (a) : (b))

static void plutofilter__box_blur(plutofilter_surface_t in, plutofilter_surface_t out, uint32_t* intermediate, int kernel_width, int kernel_height)
{
    int x, y, offset;
    uint32_t pixel, r, g, b, a;
    uint32_t sum_r, sum_g, sum_b, sum_a;

    if(kernel_width > 0) {
        kernel_width = PLUTOFILTER_MIN(kernel_width, out.width);
        for(y = 0; y < out.height; y++) {
            sum_r = sum_g = sum_b = sum_a = 0;
            for(x = 0; x < kernel_width; x++) {
                pixel = (intermediate[x % kernel_width] = PLUTOFILTER_GET_PIXEL(in, x, y));
                PLUTOFILTER_UNPACK_PIXEL(pixel, r, g, b, a);

                sum_r += r;
                sum_g += g;
                sum_b += b;
                sum_a += a;

                offset = x - kernel_width / 2;
                if(offset >= 0 && offset < out.width) {
                    PLUTOFILTER_BLUR_STORE_PIXEL(out, offset, y, sum_r, sum_g, sum_b, sum_a, kernel_width);
                }
            }

            for(x = kernel_width; x < out.width; x++) {
                pixel = intermediate[x % kernel_width];
                PLUTOFILTER_UNPACK_PIXEL(pixel, r, g, b, a);

                sum_r -= r;
                sum_g -= g;
                sum_b -= b;
                sum_a -= a;

                pixel = (intermediate[x % kernel_width] = PLUTOFILTER_GET_PIXEL(in, x, y));
                PLUTOFILTER_UNPACK_PIXEL(pixel, r, g, b, a);

                sum_r += r;
                sum_g += g;
                sum_b += b;
                sum_a += a;

                offset = x - kernel_width / 2;
                PLUTOFILTER_BLUR_STORE_PIXEL(out, offset, y, sum_r, sum_g, sum_b, sum_a, kernel_width);
            }

            for(x = out.width; x < out.width + kernel_width; x++) {
                pixel = intermediate[x % kernel_width];
                PLUTOFILTER_UNPACK_PIXEL(pixel, r, g, b, a);

                sum_r -= r;
                sum_g -= g;
                sum_b -= b;
                sum_a -= a;

                offset = x - kernel_width / 2;
                if(offset >= 0 && offset < out.width) {
                    PLUTOFILTER_BLUR_STORE_PIXEL(out, offset, y, sum_r, sum_g, sum_b, sum_a, kernel_width);
                }
            }
        }
    }

    if(kernel_height > 0) {
        kernel_height = PLUTOFILTER_MIN(kernel_height, out.height);
        for(x = 0; x < out.width; x++) {
            sum_r = sum_g = sum_b = sum_a = 0;
            for(y = 0; y < kernel_height; y++) {
                pixel = (intermediate[y % kernel_height] = PLUTOFILTER_GET_PIXEL(in, x, y));
                PLUTOFILTER_UNPACK_PIXEL(pixel, r, g, b, a);

                sum_r += r;
                sum_g += g;
                sum_b += b;
                sum_a += a;

                offset = y - kernel_height / 2;
                if(offset >= 0 && offset < out.height) {
                    PLUTOFILTER_BLUR_STORE_PIXEL(out, x, offset, sum_r, sum_g, sum_b, sum_a, kernel_height);
                }
            }

            for(y = kernel_height; y < out.height; y++) {
                pixel = intermediate[y % kernel_height];
                PLUTOFILTER_UNPACK_PIXEL(pixel, r, g, b, a);

                sum_r -= r;
                sum_g -= g;
                sum_b -= b;
                sum_a -= a;

                pixel = (intermediate[y % kernel_height] = PLUTOFILTER_GET_PIXEL(in, x, y));
                PLUTOFILTER_UNPACK_PIXEL(pixel, r, g, b, a);

                sum_r += r;
                sum_g += g;
                sum_b += b;
                sum_a += a;

                offset = y - kernel_height / 2;
                PLUTOFILTER_BLUR_STORE_PIXEL(out, x, offset, sum_r, sum_g, sum_b, sum_a, kernel_height);
            }

            for(y = out.height; y < out.height + kernel_height; y++) {
                pixel = intermediate[y % kernel_height];
                PLUTOFILTER_UNPACK_PIXEL(pixel, r, g, b, a);

                sum_r -= r;
                sum_g -= g;
                sum_b -= b;
                sum_a -= a;

                offset = y - kernel_height / 2;
                if(offset >= 0 && offset < out.height) {
                    PLUTOFILTER_BLUR_STORE_PIXEL(out, x, offset, sum_r, sum_g, sum_b, sum_a, kernel_height);
                }
            }
        }
    }
}

#define PLUTOFILTER_KERNEL_FACTOR 1.8799712059732503f

static inline int plutofilter__calc_kernel_size(float std_deviation)
{
    return (int)(floorf(std_deviation * PLUTOFILTER_KERNEL_FACTOR + 0.5f));
}

#define PLUTOFILTER_MAX_KERNEL_SIZE 512

void plutofilter_gaussian_blur(plutofilter_surface_t in, plutofilter_surface_t out, float std_deviation_x, float std_deviation_y)
{
    PLUTOFILTER_OVERLAP_SURFACE(in, out);

    int kernel_width = plutofilter__calc_kernel_size(std_deviation_x);
    int kernel_height = plutofilter__calc_kernel_size(std_deviation_y);
    if(kernel_width <= 0 && kernel_height <= 0) {
        int size = out.width * out.height;
        for(int i = 0; i < size; i++)
            out.pixels[i] = in.pixels[i];
        return;
    }

    if(kernel_width > PLUTOFILTER_MAX_KERNEL_SIZE)
        kernel_width = PLUTOFILTER_MAX_KERNEL_SIZE;
    if(kernel_height > PLUTOFILTER_MAX_KERNEL_SIZE) {
        kernel_height = PLUTOFILTER_MAX_KERNEL_SIZE;
    }

    uint32_t intermediate[PLUTOFILTER_MAX_KERNEL_SIZE];

    plutofilter__box_blur(in, out, intermediate, kernel_width, kernel_height);
    plutofilter__box_blur(out, out, intermediate, kernel_width, kernel_height);
    plutofilter__box_blur(out, out, intermediate, kernel_width, kernel_height);
}

static inline int plutofilter__div255(int x)
{
    return (x + (x >> 8) + 0x80) >> 8;
}

#define PLUTOFILTER_CLAMP_AND_STORE_PIXEL(out, x, y, r, g, b, a) \
    do { \
        (r) = PLUTOFILTER_CLAMP_PIXEL(r); \
        (g) = PLUTOFILTER_CLAMP_PIXEL(g); \
        (b) = PLUTOFILTER_CLAMP_PIXEL(b); \
        (a) = PLUTOFILTER_CLAMP_PIXEL(a); \
        PLUTOFILTER_STORE_PIXEL(out, x, y, r, g, b, a); \
    } while(0)

#define PLUTOFILTER_DEFINE_BLEND_MODE(name) \
static inline int plutofilter__blend_##name##_op(int s, int d, int sa, int da); \
static void plutofilter__blend_##name(plutofilter_surface_t in1, plutofilter_surface_t in2, plutofilter_surface_t out) { \
    for(int y = 0; y < out.height; y++) { \
        for(int x = 0; x < out.width; x++) { \
            PLUTOFILTER_INIT_LOAD_PIXEL(in1, x, y, sr, sg, sb, sa); \
            PLUTOFILTER_INIT_LOAD_PIXEL(in2, x, y, dr, dg, db, da); \
            int r = plutofilter__blend_##name##_op(sr, dr, sa, da); \
            int g = plutofilter__blend_##name##_op(sg, dg, sa, da); \
            int b = plutofilter__blend_##name##_op(sb, db, sa, da); \
            int a = sa + da - plutofilter__div255(sa * da); \
            PLUTOFILTER_CLAMP_AND_STORE_PIXEL(out, x, y, r, g, b, a); \
        } \
    } \
} \
static inline int plutofilter__blend_##name##_op(int s, int d, int sa, int da) \

PLUTOFILTER_DEFINE_BLEND_MODE(normal)
{
    return s + plutofilter__div255(d * (255 - sa));
}

PLUTOFILTER_DEFINE_BLEND_MODE(multiply)
{
    return plutofilter__div255(s * d + s * (255 - da) + d * (255 - sa));
}

PLUTOFILTER_DEFINE_BLEND_MODE(screen)
{
    return s + d - plutofilter__div255(s * d);
}

PLUTOFILTER_DEFINE_BLEND_MODE(overlay)
{
    const int tmp = s * (255 - da) + d * (255 - sa);
    if(2 * d <= da)
        return plutofilter__div255(2 * s * d + tmp);
    return plutofilter__div255(sa * da - 2 * (da - d) * (sa - s) + tmp);
}

PLUTOFILTER_DEFINE_BLEND_MODE(darken)
{
    const int sda = s * da;
    const int dsa = d * sa;

    if(sda < dsa)
        return s + d - plutofilter__div255(dsa);
    return d + s - plutofilter__div255(sda);
}

PLUTOFILTER_DEFINE_BLEND_MODE(lighten)
{
    const int sda = s * da;
    const int dsa = d * sa;

    if(sda > dsa)
        return s + d - plutofilter__div255(dsa);
    return d + s - plutofilter__div255(sda);
}

PLUTOFILTER_DEFINE_BLEND_MODE(color_dodge)
{
    if(d == 0)
        return plutofilter__div255(s * (255 - da));
    if(s == sa) {
        return plutofilter__div255(sa * da + s * (255 - da) + d * (255 - sa));
    }

    if(da * (sa - s) < d * sa)
        return plutofilter__div255(sa * da + s * (255 - da) + d * (255 - sa));
    return plutofilter__div255(sa * ((d * sa) / (sa - s)) + s * (255 - da) + d * (255 - sa));
}

PLUTOFILTER_DEFINE_BLEND_MODE(color_burn)
{
    if(d == da)
        return plutofilter__div255(sa * da + s * (255 - da) + d * (255 - sa));
    if(s == 0) {
        return plutofilter__div255(d * (255 - sa));
    }

    if(da * s < (da - d) * sa)
        return plutofilter__div255(s * (255 - da) + d * (255 - sa));
    return plutofilter__div255(sa * (da - ((da - d) * sa / s)) + s * (255 - da) + d * (255 - sa));
}

PLUTOFILTER_DEFINE_BLEND_MODE(hard_light)
{
    const int tmp = s * (255 - da) + d * (255 - sa);
    if(2 * s <= sa)
        return plutofilter__div255(2 * s * d + tmp);
    return plutofilter__div255(sa * da - 2 * (da - d) * (sa - s) + tmp);
}

PLUTOFILTER_DEFINE_BLEND_MODE(soft_light)
{
    const int s2 = s << 1;
    const int d_np = da ? (255 * d) / da : 0;
    const int temp = (s * (255 - da) + d * (255 - sa)) * 255;

    if(s2 < sa)
        return (d * (sa * 255 + (s2 - sa) * (255 - d_np)) + temp) / 65025;
    if(4 * d <= da)
        return (d * sa * 255 + da * (s2 - sa) * ((((16 * d_np - 12 * 255) * d_np + 3 * 65025) * d_np) / 65025) + temp) / 65025;
    return ((d * sa * 255 + da * (s2 - sa) * (int)(sqrtf(d_np * 255)) - d_np) + temp) / 65025;
}

PLUTOFILTER_DEFINE_BLEND_MODE(difference)
{
    const int sda = s * da;
    const int dsa = d * sa;

    if(sda < dsa)
        return d + s - 2 * plutofilter__div255(sda);
    return s + d - 2 * plutofilter__div255(dsa);
}

PLUTOFILTER_DEFINE_BLEND_MODE(exclusion)
{
    return plutofilter__div255(255 * (s + d) - 2 * s * d);
}

void plutofilter_blend(plutofilter_surface_t in1, plutofilter_surface_t in2, plutofilter_surface_t out, plutofilter_blend_mode_t mode)
{
    PLUTOFILTER_OVERLAP_SURFACE3(in1, in2, out);

    switch(mode) {
    case PLUTOFILTER_BLEND_MODE_NORMAL:
        plutofilter__blend_normal(in1, in2, out);
        break;
    case PLUTOFILTER_BLEND_MODE_MULTIPLY:
        plutofilter__blend_multiply(in1, in2, out);
        break;
    case PLUTOFILTER_BLEND_MODE_SCREEN:
        plutofilter__blend_screen(in1, in2, out);
        break;
    case PLUTOFILTER_BLEND_MODE_OVERLAY:
        plutofilter__blend_overlay(in1, in2, out);
        break;
    case PLUTOFILTER_BLEND_MODE_DARKEN:
        plutofilter__blend_darken(in1, in2, out);
        break;
    case PLUTOFILTER_BLEND_MODE_LIGHTEN:
        plutofilter__blend_lighten(in1, in2, out);
        break;
    case PLUTOFILTER_BLEND_MODE_COLOR_DODGE:
        plutofilter__blend_color_dodge(in1, in2, out);
        break;
    case PLUTOFILTER_BLEND_MODE_COLOR_BURN:
        plutofilter__blend_color_burn(in1, in2, out);
        break;
    case PLUTOFILTER_BLEND_MODE_HARD_LIGHT:
        plutofilter__blend_hard_light(in1, in2, out);
        break;
    case PLUTOFILTER_BLEND_MODE_SOFT_LIGHT:
        plutofilter__blend_soft_light(in1, in2, out);
        break;
    case PLUTOFILTER_BLEND_MODE_DIFFERENCE:
        plutofilter__blend_difference(in1, in2, out);
        break;
    case PLUTOFILTER_BLEND_MODE_EXCLUSION:
        plutofilter__blend_exclusion(in1, in2, out);
        break;
    }
}

static void plutofilter__composite_over(plutofilter_surface_t in1, plutofilter_surface_t in2, plutofilter_surface_t out)
{
    for(int y = 0; y < out.height; y++) {
        for(int x = 0; x < out.width; x++) {
            PLUTOFILTER_INIT_LOAD_PIXEL(in1, x, y, sr, sg, sb, sa);
            PLUTOFILTER_INIT_LOAD_PIXEL(in2, x, y, dr, dg, db, da);

            uint32_t inv_sa = 255 - sa;

            uint32_t r = sr + plutofilter__div255(dr * inv_sa);
            uint32_t g = sg + plutofilter__div255(dg * inv_sa);
            uint32_t b = sb + plutofilter__div255(db * inv_sa);
            uint32_t a = sa + plutofilter__div255(da * inv_sa);

            PLUTOFILTER_STORE_PIXEL(out, x, y, r, g, b, a);
        }
    }
}

static void plutofilter__composite_in(plutofilter_surface_t in1, plutofilter_surface_t in2, plutofilter_surface_t out)
{
    for(int y = 0; y < out.height; y++) {
        for(int x = 0; x < out.width; x++) {
            PLUTOFILTER_INIT_LOAD_PIXEL(in1, x, y, sr, sg, sb, sa);

            uint32_t da = PLUTOFILTER_ALPHA(PLUTOFILTER_GET_PIXEL(in2, x, y));

            uint32_t r = plutofilter__div255(sr * da);
            uint32_t g = plutofilter__div255(sg * da);
            uint32_t b = plutofilter__div255(sb * da);
            uint32_t a = plutofilter__div255(sa * da);

            PLUTOFILTER_STORE_PIXEL(out, x, y, r, g, b, a);
        }
    }
}

static void plutofilter__composite_out(plutofilter_surface_t in1, plutofilter_surface_t in2, plutofilter_surface_t out)
{
    for(int y = 0; y < out.height; y++) {
        for(int x = 0; x < out.width; x++) {
            PLUTOFILTER_INIT_LOAD_PIXEL(in1, x, y, sr, sg, sb, sa);

            uint32_t inv_da = 255 - PLUTOFILTER_ALPHA(PLUTOFILTER_GET_PIXEL(in2, x, y));

            uint32_t r = plutofilter__div255(sr * inv_da);
            uint32_t g = plutofilter__div255(sg * inv_da);
            uint32_t b = plutofilter__div255(sb * inv_da);
            uint32_t a = plutofilter__div255(sa * inv_da);

            PLUTOFILTER_STORE_PIXEL(out, x, y, r, g, b, a);
        }
    }
}

static void plutofilter__composite_atop(plutofilter_surface_t in1, plutofilter_surface_t in2, plutofilter_surface_t out)
{
    for(int y = 0; y < out.height; y++) {
        for(int x = 0; x < out.width; x++) {
            PLUTOFILTER_INIT_LOAD_PIXEL(in1, x, y, sr, sg, sb, sa);
            PLUTOFILTER_INIT_LOAD_PIXEL(in2, x, y, dr, dg, db, da);

            uint32_t inv_sa = 255 - sa;

            uint32_t r = plutofilter__div255(sr * da) + plutofilter__div255(dr * inv_sa);
            uint32_t g = plutofilter__div255(sg * da) + plutofilter__div255(dg * inv_sa);
            uint32_t b = plutofilter__div255(sb * da) + plutofilter__div255(db * inv_sa);

            PLUTOFILTER_STORE_PIXEL(out, x, y, r, g, b, da);
        }
    }
}

static void plutofilter__composite_xor(plutofilter_surface_t in1, plutofilter_surface_t in2, plutofilter_surface_t out)
{
    for(int y = 0; y < out.height; y++) {
        for(int x = 0; x < out.width; x++) {
            PLUTOFILTER_INIT_LOAD_PIXEL(in1, x, y, sr, sg, sb, sa);
            PLUTOFILTER_INIT_LOAD_PIXEL(in2, x, y, dr, dg, db, da);

            uint32_t inv_sa = 255 - sa;
            uint32_t inv_da = 255 - da;

            uint32_t r = plutofilter__div255(sr * inv_da) + plutofilter__div255(dr * inv_sa);
            uint32_t g = plutofilter__div255(sg * inv_da) + plutofilter__div255(dg * inv_sa);
            uint32_t b = plutofilter__div255(sb * inv_da) + plutofilter__div255(db * inv_sa);
            uint32_t a = plutofilter__div255(sa * inv_da) + plutofilter__div255(da * inv_sa);

            PLUTOFILTER_STORE_PIXEL(out, x, y, r, g, b, a);
        }
    }
}

void plutofilter_composite(plutofilter_surface_t in1, plutofilter_surface_t in2, plutofilter_surface_t out, plutofilter_composite_operator_t op)
{
    PLUTOFILTER_OVERLAP_SURFACE3(in1, in2, out);

    switch(op) {
    case PLUTOFILTER_COMPOSITE_OPERATOR_OVER:
        plutofilter__composite_over(in1, in2, out);
        break;
    case PLUTOFILTER_COMPOSITE_OPERATOR_IN:
        plutofilter__composite_in(in1, in2, out);
        break;
    case PLUTOFILTER_COMPOSITE_OPERATOR_OUT:
        plutofilter__composite_out(in1, in2, out);
        break;
    case PLUTOFILTER_COMPOSITE_OPERATOR_ATOP:
        plutofilter__composite_atop(in1, in2, out);
        break;
    case PLUTOFILTER_COMPOSITE_OPERATOR_XOR:
        plutofilter__composite_xor(in1, in2, out);
        break;
    }
}

void plutofilter_composite_arithmetic(plutofilter_surface_t in1, plutofilter_surface_t in2, plutofilter_surface_t out, float k1, float k2, float k3, float k4)
{
    PLUTOFILTER_OVERLAP_SURFACE3(in1, in2, out);

    for(int y = 0; y < out.height; y++) {
        for(int x = 0; x < out.width; x++) {
            PLUTOFILTER_INIT_LOAD_PIXEL(in1, x, y, sr, sg, sb, sa);
            PLUTOFILTER_INIT_LOAD_PIXEL(in2, x, y, dr, dg, db, da);

            float fdr = k1 * ((sr * dr) / 255.f) + k2 * sr + k3 * dr + k4 * 255.f;
            float fdg = k1 * ((sg * dg) / 255.f) + k2 * sg + k3 * dg + k4 * 255.f;
            float fdb = k1 * ((sb * db) / 255.f) + k2 * sb + k3 * db + k4 * 255.f;
            float fda = k1 * ((sa * da) / 255.f) + k2 * sa + k3 * da + k4 * 255.f;

            PLUTOFILTER_CLAMP_AND_STORE_PIXEL(out, x, y, fdr, fdg, fdb, fda);
        }
    }
}

#endif // PLUTOFILTER_IMPLEMENTATION
