/**
 * \file
 *
 * \brief This file contains utility objects for using nuklear library things
 * easier.
 */

#ifndef NK_UTILITY_H_
#define NK_UTILITY_H_

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include <nuklear.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define nk_stringify(arg) (#arg)

/**
 * \brief This structure represents a plot object. Can be plotted using
 * plotObject.
 */
struct plot_object_s {
  float *draw_buffer;
  size_t length;
  char *title;
  struct nk_color color;
  int32_t height;
  float ymin;
  float ymax;
};

/**
 * \brief Draw a plot in immediate mode.
 *
 * \param ctx         The nuklear context.
 * \param draw_buffer Array of floats to draw.
 * \param length      Size of draw_buffer.
 * \param title       Title of the plot.
 * \param height      Height in pixels of the plot.
 * \param ymin        Y axis minimum.
 * \param ymax        Y axis maximum.
 */
void plot(struct nk_context *ctx,
    float *draw_buffer,
    size_t length,
    char *title,
    int32_t height,
    float ymin,
    float ymax);

/**
 * \brief Draw a plot by passing in a plot object.
 *
 * \param ctx The nuklear context.
 * \param obj The plot object.
 */
void plotObject(struct nk_context *ctx, struct plot_object_s *obj);

/**
 * \brief Draw a plot with two colored lines in immediate mode.
 *
 * \param ctx           The nuklear context.
 * \param draw_buffer1  Array of floats to draw.
 * \param draw_buffer2  Array of floats to draw.
 * \param buffer1_color Color of line 1.
 * \param buffer2_color Color of line 2.
 * \param length        Size of draw_buffer.
 * \param title         Title of the plot.
 * \param height        Height in pixels of the plot.
 * \param ymin          Y axis minimum.
 * \param ymax          Y axis maximum.
 */
void plotTwo(struct nk_context *ctx,
    float *draw_buffer1,
    float *draw_buffer2,
    struct nk_color buffer1_color,
    struct nk_color buffer2_color,
    size_t length,
    char *title,
    int32_t height,
    float ymin,
    float ymax);

/**
 * \brief Draw a plot with N colored lines (up to 3?)
 * Uses first line title and dimensions.
 *
 * \param ctx         The nuklear context.
 * \param obj         The plot object array
 * \param num_objects Number of objects in array.
 */
void plotN(struct nk_context *ctx, struct plot_object_s *obj_arr,
    size_t num_objects);


/**
 * \brief Draw a toggle button which will show different labels based on its
 * state
 *
 * \param ctx         The nuklear context.
 * \param labels      Array of labels
 * \param state       The state (integer) of the button corresponding to a
 * particular label.
 */
inline bool multiLabelToggle(struct nk_context *ctx, const char *const *labels, size_t state) {
  if (nk_button_label(ctx, labels[state]))
    return true;
  else
    return false;
}

size_t editFieldTitle(struct nk_context *ctx, const char *title,
    char *buff, int *input_len, size_t buff_len);
#endif
