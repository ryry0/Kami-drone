#ifndef MVU_H_
#define MVU_H_


#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include <nuklear.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct mvu_model_s *mvu_model_t;
typedef struct mvu_msg_s *mvu_msg_t;

/** TODO: fix docs. */

mvu_msg_t mvu_createMessage();
void mvu_destroyMessage(mvu_msg_t msg);

/**
 * \brief This function takes a mvu object and initializes it
 * \param mvu takes a mvu object
 * \param ctx nuklear context
 */
mvu_model_t mvu_createModel();
bool mvu_initModel(mvu_model_t model, struct nk_context *ctx);

/**
 * \brief This function destroys any parts allocated for the mvu
 * \param mvu takes a mvu object
 */
void mvu_destroyModel(mvu_model_t model);

/**
 * \brief This function takes a mvu object and updates it
 * \param mvu takes a mvu object
 */
/** update : msg -> model -> model */
void mvu_update(const mvu_msg_t msg, mvu_model_t model);


/** view : model -> msg */
void  mvu_view(const mvu_model_t model, mvu_msg_t msg);

void mvu_setSpace(mvu_model_t model, bool space_pressed);
#endif
