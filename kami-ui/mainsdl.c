/* nuklear - v1.17 - public domain */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <mvu.h>

#define NK_ZERO_COMMAND_MEMORY
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#include <nuklear.h>
#include <nuklear_sdl_gl3.h>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 1100

#define MAX_VERTEX_MEMORY 2048 * 1024 *2
#define MAX_ELEMENT_MEMORY 2048 * 1024 *2

#define UNUSED(a) (void)a
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define LEN(a) (sizeof(a)/sizeof(a)[0])
#define TRIANGLE_BUTTON 3
#define X_BUTTON 0
#define R_BUMPER 5
#define R_TRIGGER 5
#define X_AXIS 3
#define Y_AXIS 4
#define R_TRIGGER_DEADZONE 12000
#define MAX_THROTTLE 200
#define MAX_ANGLE 10

#define STREAM_DELAY 4

/* ===============================================================
 *
 *                          EXAMPLE
 *
 * ===============================================================*/
/* This are some code examples to provide a small overview of what can be
 * done with this library. To try out an example uncomment the include
 * and the corresponding function. */
/*#include "../style.c"*/
/*#include "../calculator.c"*/
/*#include "../overview.c"*/
/*#include "../node_editor.c"*/

/* ===============================================================
 *
 *                          DEMO
 *
 * ===============================================================*/
int main(int argc, char* argv[])
{
  /* Platform */
  SDL_Window *win;
  SDL_GLContext glContext;
  SDL_Joystick* x360_gamepad = NULL;

  struct nk_color background;
  int win_width, win_height;
  int running = 1;
  void *last_render_cmds;
  size_t last_mem_alloc;
  size_t time_delay = 0;

  /* GUI */
  struct nk_context *ctx;

  mvu_model_t model = mvu_createModel();
  mvu_msg_t msg = mvu_createMessage();

  /* SDL setup */
  SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
  SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS| SDL_INIT_JOYSTICK);
  SDL_GL_SetAttribute (SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  win = SDL_CreateWindow("Demo",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI|
      SDL_WINDOW_RESIZABLE);
  glContext = SDL_GL_CreateContext(win);
  SDL_GetWindowSize(win, &win_width, &win_height);

  /* OpenGL setup */
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  glewExperimental = 1;
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to setup GLEW\n");
    exit(1);
  }

  ctx = nk_sdl_init(win);

  if(mvu_initModel(model, ctx) == false)
    return 1;

  /* Load Fonts: if none of these are loaded a default font will be used  */
  /* Load Cursor: if you uncomment cursor loading please hide the cursor */
  {struct nk_font_atlas *atlas;
    nk_sdl_font_stash_begin(&atlas);
    /*struct nk_font *droid = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 14, 0);*/
    struct nk_font *roboto = nk_font_atlas_add_from_file(atlas, "fonts/Roboto-Regular.ttf", 32, 0);
    /*struct nk_font *future = nk_font_atlas_add_from_file(atlas, "../../../extra_font/kenvector_future_thin.ttf", 13, 0);*/
    /*struct nk_font *clean = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyClean.ttf", 12, 0);*/
    /*struct nk_font *tiny = nk_font_atlas_add_from_file(atlas, "../../../extra_font/ProggyTiny.ttf", 10, 0);*/
    /*struct nk_font *cousine = nk_font_atlas_add_from_file(atlas, "../../../extra_font/Cousine-Regular.ttf", 13, 0);*/
    nk_sdl_font_stash_end();
    /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
    nk_style_set_font(ctx, &roboto->handle);}

  /* style.c */
  /*set_style(ctx, THEME_WHITE);*/
  /*set_style(ctx, THEME_RED);*/
  /*set_style(ctx, THEME_BLUE);*/
  /*set_style(ctx, THEME_DARK);*/

  last_render_cmds = calloc(1, ctx->memory.allocated);
  last_mem_alloc = ctx->memory.allocated;

  background = nk_rgb(28,48,62);

  if ( SDL_NumJoysticks() < 1) {
    printf("No Joy connected\n");
  }
  else {
    x360_gamepad = SDL_JoystickOpen(0);
    if ( x360_gamepad == NULL )
      printf("Error openning gamepad %s\n", SDL_GetError() );
    else
      printf("Gamepad Connected\n");

  }

  while (running) {
    /* Input */
    SDL_Event evt;

    nk_input_begin(ctx);
    while (SDL_PollEvent(&evt)) {
      if (evt.type == SDL_QUIT) goto cleanup;
      nk_sdl_handle_event(&evt);
      if (evt.type == SDL_KEYDOWN)
        if (evt.key.keysym.sym == SDLK_SPACE)
          mvu_setSpace(model, true);

      if (evt.type == SDL_KEYUP)
        if (evt.key.keysym.sym == SDLK_SPACE)
          mvu_setSpace(model, false);

      if (evt.type == SDL_JOYBUTTONDOWN) {
          printf("%d button pressed \n", evt.jbutton.button);

          if (evt.jbutton.button == TRIANGLE_BUTTON)
            mvu_sendHeaderMsg(model, PKT_QUERY);

          if (evt.jbutton.button == X_BUTTON || evt.jbutton.button == R_BUMPER)
            mvu_sendHeaderMsg(model, PKT_TAKEOFF);
      }
      if (evt.type == SDL_JOYBUTTONUP) {
          printf("%d button up \n", evt.jbutton.button);

          if (evt.jbutton.button == X_BUTTON || evt.jbutton.button == R_BUMPER)
            mvu_sendHeaderMsg(model, PKT_KILL);
      }

      if (time_delay++ > STREAM_DELAY) {
        if (evt.type == SDL_JOYAXISMOTION) {
          static int32_t thrust_val = 0;
          static float roll_val = 0;
          static float pitch_val = 0;
          static float prev_roll = 0;
          static float prev_pitch = 0;
          if (evt.jaxis.axis == R_TRIGGER) {
            thrust_val = evt.jaxis.value + 32768;
            thrust_val = thrust_val > R_TRIGGER_DEADZONE? (thrust_val - R_TRIGGER_DEADZONE) : 0;
            thrust_val = (float) thrust_val/(65535 - R_TRIGGER_DEADZONE) * MAX_THROTTLE;
            //printf("axis %d value %d\n", evt.jaxis.axis, val);
            mvu_sendCommandMsg(model, roll_val, pitch_val, thrust_val);
            time_delay = 0;
          }
          else if (evt.jaxis.axis == X_AXIS) {
            roll_val = evt.jaxis.value;
            roll_val = abs(roll_val) > R_TRIGGER_DEADZONE? copysignf((abs(roll_val) - R_TRIGGER_DEADZONE),roll_val) : 0;
            roll_val = (float) roll_val/(32767 - R_TRIGGER_DEADZONE) * MAX_ANGLE;
            if (!((prev_roll == 0) && (roll_val == 0))) {
              printf("axis %f roll_value %f\n", evt.jaxis.axis, roll_val);
              mvu_sendCommandMsg(model, roll_val, pitch_val, thrust_val);
              prev_roll = roll_val;
            }
          }
          else if (evt.jaxis.axis == Y_AXIS) {
            pitch_val = evt.jaxis.value;
            pitch_val = abs(pitch_val) > R_TRIGGER_DEADZONE? copysignf((abs(pitch_val) - R_TRIGGER_DEADZONE),pitch_val) : 0;
            pitch_val = (float) pitch_val/(32767 - R_TRIGGER_DEADZONE) * MAX_ANGLE;
            if (!((prev_pitch == 0) && (pitch_val == 0))) {
              printf("axis %f pitch_value %f\n", evt.jaxis.axis, pitch_val);
              mvu_sendCommandMsg(model, roll_val, pitch_val, thrust_val);
              prev_pitch = pitch_val;
            }
          }

          //printf("axis %d value %d\n", evt.jaxis.axis, evt.jaxis.value);
        }
      }
    }
    nk_input_end(ctx);


    /* GUI */
    char *window_name = "Drone UI";
    if (nk_begin(ctx, window_name, nk_rect(0, 0, WINDOW_WIDTH,
            WINDOW_HEIGHT),
          NK_WINDOW_BORDER|
          NK_WINDOW_CLOSABLE|NK_WINDOW_TITLE)) {

      nk_window_set_size(ctx, nk_vec2(win_width, win_height));
      mvu_view(model, msg);
    } //end if
    nk_end(ctx);
    if (nk_window_is_closed(ctx, window_name)) break;

    /* Draw */
    void *cmds = nk_buffer_memory(&ctx->memory);
    bool mem_alloc_is_diff = last_mem_alloc != ctx->memory.allocated;
    int memcmp_result = memcmp(cmds, last_render_cmds, ctx->memory.allocated);

    if ((mem_alloc_is_diff) || (memcmp_result != 0))
    {
      //dynamic resize of last_render_cmds
      if (mem_alloc_is_diff) {
        free(last_render_cmds);
        last_render_cmds = calloc(1, ctx->memory.allocated);
        last_mem_alloc = ctx->memory.allocated;
      }

      memcpy(last_render_cmds, cmds, ctx->memory.allocated);

      float bg[4];
      nk_color_fv(bg, background);
      SDL_GetWindowSize(win, &win_width, &win_height);
      glViewport(0, 0, win_width, win_height);

      glClear(GL_COLOR_BUFFER_BIT);
      glClearColor(bg[0], bg[1], bg[2], bg[3]);
      /* IMPORTANT: `nk_sdl_render` modifies some global OpenGL state
       * with blending, scissor, face culling, depth test and viewport and
       * defaults everything back into a default state.
       * Make sure to either a.) save and restore or b.) reset your own state after
       * rendering the UI. */
      nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
      SDL_GL_SwapWindow(win);
    }
    else
      nk_clear(ctx);

    mvu_update(msg, model);
  } //end while

cleanup:
  nk_sdl_shutdown();
  SDL_JoystickClose(x360_gamepad);
  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(win);
  SDL_Quit();
  mvu_destroyModel(model);
  mvu_destroyMessage(msg);
  return 0;
}

