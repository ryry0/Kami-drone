#include <nk_utility.h>
#include <stdlib.h>
#include <stdio.h>

void plot(struct nk_context *ctx,
    float *draw_buffer,
    size_t length,
    char *title,
    int32_t height,
    float ymin,
    float ymax) {

  /* Draw plots */
  int32_t hover_index = -1;

  /* todo: make into chart object */

  nk_layout_row_dynamic(ctx, 40, 1);
  nk_label(ctx, title, NK_TEXT_LEFT);

  nk_layout_row_dynamic(ctx, height, 1);
  nk_chart_begin(ctx, NK_CHART_LINES, length, ymin, ymax);

  for (int i = 0; i < length; ++i) {
    nk_flags res = nk_chart_push(ctx, (float) draw_buffer[i]);
    if (res & NK_CHART_HOVERING)
      hover_index = i;
  }

  /* Hover tooltip */
  if (hover_index != -1) {
    char buffer[NK_MAX_NUMBER_BUFFER];
    sprintf(buffer, "Value: %f", draw_buffer[hover_index]);
    nk_tooltip(ctx, buffer);
  }
}

void plotObject(struct nk_context *ctx, struct plot_object_s *obj) {
  plot(ctx, obj->draw_buffer, obj->length, obj->title, obj->height, obj->ymin, obj->ymax);
}

void plotTwo(struct nk_context *ctx,
    float *draw_buffer1,
    float *draw_buffer2,
    struct nk_color buffer1_color,
    struct nk_color buffer2_color,
    size_t length,
    char *title,
    int32_t height,
    float ymin,
    float ymax) {

  /* Draw plots */
  int32_t hover_index1 = -1;
  int32_t hover_index2 = -1;

  /* todo: make into chart object */

  nk_layout_row_dynamic(ctx, 40, 1);
  nk_label(ctx, title, NK_TEXT_LEFT);

  nk_layout_row_dynamic(ctx, height, 1);
  nk_chart_begin_colored(ctx, NK_CHART_LINES, nk_rgb(255, 0 , 0), nk_rgb(150, 0, 0), length, ymin, ymax);

  nk_chart_add_slot_colored(ctx, NK_CHART_LINES, buffer1_color, buffer1_color,
      length, ymin, ymax);

  nk_chart_add_slot_colored(ctx, NK_CHART_LINES, buffer2_color, buffer2_color,
      length, ymin, ymax);

  for (int i = 0; i < length; ++i) {
    nk_flags res1 = nk_chart_push_slot(ctx, draw_buffer1[i], 0);
    nk_flags res2 = nk_chart_push_slot(ctx, draw_buffer2[i], 1);

    if (res1 & NK_CHART_HOVERING)
      hover_index1 = i;

    if (res2 & NK_CHART_HOVERING)
      hover_index2 = i;
  }

  /* Hover tooltip */
  if (hover_index1 != -1) {
    char buffer[NK_MAX_NUMBER_BUFFER];
    sprintf(buffer, "Value: %f", draw_buffer1[hover_index1]);
    nk_tooltip(ctx, buffer);
  }

  if (hover_index2 != -1) {
    char buffer[NK_MAX_NUMBER_BUFFER];
    sprintf(buffer, "Value: %f", draw_buffer2[hover_index2]);
    nk_tooltip(ctx, buffer);
  }
}

/* uses first line title and dimensions */
void plotN(struct nk_context *ctx, struct plot_object_s *obj_arr,
    size_t num_objects) {

  /* Draw plots */
  int32_t *hover_indices = malloc(num_objects * sizeof(int32_t));
  for (size_t i = 0; i < num_objects; ++i) {
    hover_indices[i] = -1;
  }

  /*
  nk_layout_row_dynamic(ctx, 40, 1);
  nk_label(ctx, obj_arr[0].title, NK_TEXT_LEFT);

  nk_layout_row_dynamic(ctx, obj_arr[0].height, 1);
  */
  nk_chart_begin_colored(ctx, NK_CHART_LINES, nk_rgb(255, 0 , 0),
      nk_rgb(150, 0, 0), obj_arr[0].length, obj_arr[0].ymin, obj_arr[0].ymax);

  for (size_t i = 0; i < num_objects; ++i) {
    nk_chart_add_slot_colored(ctx, NK_CHART_LINES, obj_arr[i].color, obj_arr[i].color,
        obj_arr[i].length, obj_arr[i].ymin, obj_arr[i].ymax);

    for (size_t k = 0; k < obj_arr[i].length; ++k) {
      nk_flags res1 = nk_chart_push_slot(ctx, obj_arr[i].draw_buffer[k], i);

      if (res1 & NK_CHART_HOVERING)
        hover_indices[i] = k;
    }

    // Hover tooltip
    if (hover_indices[i] != -1) {
      char buffer[NK_MAX_NUMBER_BUFFER];
      sprintf(buffer, "Value: %f", obj_arr[i].draw_buffer[hover_indices[i]]);
      nk_tooltip(ctx, buffer);
    }
  }

  free(hover_indices);
}

size_t editFieldTitle(struct nk_context *ctx, const char *title,
    char *buff, int *input_len, size_t buff_len) {

  nk_label(ctx, title, NK_TEXT_LEFT);
  nk_edit_string(ctx,
      NK_EDIT_FIELD | NK_TEXT_EDIT_SINGLE_LINE,
      buff,
      input_len,
      buff_len,
      nk_filter_default);

  buff[*input_len] = 0;
}
