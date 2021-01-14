//------------------------------------------------------------------------------
//  glmcube.cpp
//  Texture creation, rendering with texture, packed vertex components.
//------------------------------------------------------------------------------

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "dbgui.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "03-glmcube.shader.h"

struct {
    glm::vec2 rotate;
    sg_pass_action pass_action;
    sg_pipeline pip;
    sg_bindings bind;
} state;

struct vertex_t {
    float x, y, z;
    uint32_t color;
    int16_t u, v;
};

void init(void) {
    sg_desc desc = {};
    desc.context = sapp_sgcontext();
    sg_setup(&desc);
    __dbgui_setup(sapp_sample_count());

    /*
        Cube vertex buffer with packed vertex formats for color and texture coords.
        Note that a vertex format which must be portable across all
        backends must only use the normalized integer formats
        (BYTE4N, UBYTE4N, SHORT2N, SHORT4N), which can be converted
        to floating point formats in the vertex shader inputs.

        The reason is that D3D11 cannot convert from non-normalized
        formats to floating point inputs (only to integer inputs),
        and WebGL2 / GLES2 don't support integer vertex shader inputs.
    */
    vertex_t vertices[] = {
        /* pos                  color       uvs */
        { -1.0f, -1.0f, -1.0f,  0xFF0000FF,     0,     0 },
        {  1.0f, -1.0f, -1.0f,  0xFF0000FF, 32767,     0 },
        {  1.0f,  1.0f, -1.0f,  0xFF0000FF, 32767, 32767 },
        { -1.0f,  1.0f, -1.0f,  0xFF0000FF,     0, 32767 },

        { -1.0f, -1.0f,  1.0f,  0xFF00FF00,     0,     0 },
        {  1.0f, -1.0f,  1.0f,  0xFF00FF00, 32767,     0 },
        {  1.0f,  1.0f,  1.0f,  0xFF00FF00, 32767, 32767 },
        { -1.0f,  1.0f,  1.0f,  0xFF00FF00,     0, 32767 },

        { -1.0f, -1.0f, -1.0f,  0xFFFF0000,     0,     0 },
        { -1.0f,  1.0f, -1.0f,  0xFFFF0000, 32767,     0 },
        { -1.0f,  1.0f,  1.0f,  0xFFFF0000, 32767, 32767 },
        { -1.0f, -1.0f,  1.0f,  0xFFFF0000,     0, 32767 },

        {  1.0f, -1.0f, -1.0f,  0xFFFF007F,     0,     0 },
        {  1.0f,  1.0f, -1.0f,  0xFFFF007F, 32767,     0 },
        {  1.0f,  1.0f,  1.0f,  0xFFFF007F, 32767, 32767 },
        {  1.0f, -1.0f,  1.0f,  0xFFFF007F,     0, 32767 },

        { -1.0f, -1.0f, -1.0f,  0xFFFF7F00,     0,     0 },
        { -1.0f, -1.0f,  1.0f,  0xFFFF7F00, 32767,     0 },
        {  1.0f, -1.0f,  1.0f,  0xFFFF7F00, 32767, 32767 },
        {  1.0f, -1.0f, -1.0f,  0xFFFF7F00,     0, 32767 },

        { -1.0f,  1.0f, -1.0f,  0xFF007FFF,     0,     0 },
        { -1.0f,  1.0f,  1.0f,  0xFF007FFF, 32767,     0 },
        {  1.0f,  1.0f,  1.0f,  0xFF007FFF, 32767, 32767 },
        {  1.0f,  1.0f, -1.0f,  0xFF007FFF,     0, 32767 },
    };

    sg_buffer_desc cube_vdesc{};
    cube_vdesc.size = sizeof(vertices);
    cube_vdesc.content = vertices;
    cube_vdesc.label = "cube-vertices";


    state.bind.vertex_buffers[0] = sg_make_buffer(&cube_vdesc);

    /* create an index buffer for the cube */
    uint16_t indices[] = {
        0, 1, 2,  0, 2, 3,
        6, 5, 4,  7, 6, 4,
        8, 9, 10,  8, 10, 11,
        14, 13, 12,  15, 14, 12,
        16, 17, 18,  16, 18, 19,
        22, 21, 20,  23, 22, 20
    };

    sg_buffer_desc cube_idesc{};
    cube_idesc.type = SG_BUFFERTYPE_INDEXBUFFER;
    cube_idesc.size = sizeof(indices);
    cube_idesc.content = indices;
    cube_idesc.label = "cube-indices";

    state.bind.index_buffer = sg_make_buffer(&cube_idesc);

    /* create a checkerboard texture */
    uint32_t pixels[4*4] = {
        0xFFFFFFFF, 0xFF000000, 0xFFFFFFFF, 0xFF000000,
        0xFF000000, 0xFFFFFFFF, 0xFF000000, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFF000000, 0xFFFFFFFF, 0xFF000000,
        0xFF000000, 0xFFFFFFFF, 0xFF000000, 0xFFFFFFFF,
    };
    /* NOTE: tex_slot is provided by shader code generation */
    sg_image_content cube_t_content{};
    cube_t_content.subimage[0][0] = {pixels, sizeof(pixels)};

    sg_image_desc cube_image{};
    cube_image.width = 4;
    cube_image.height = 4;
    cube_image.content = cube_t_content;
    cube_image.label = "cube-texture";

    state.bind.fs_images[SLOT_tex] = sg_make_image(&cube_image);

    /* a shader */
    sg_shader shd = sg_make_shader(texcube_shader_desc());

    /* a pipeline state object */
    sg_layout_desc pip_layout{};
    pip_layout.attrs[ATTR_vs_pos].format = SG_VERTEXFORMAT_FLOAT3;
    pip_layout.attrs[ATTR_vs_color0].format = SG_VERTEXFORMAT_UBYTE4N;
    pip_layout.attrs[ATTR_vs_texcoord0].format = SG_VERTEXFORMAT_SHORT2N;

    sg_depth_stencil_state ds_state{};
    ds_state.depth_compare_func = SG_COMPAREFUNC_LESS_EQUAL;
    ds_state.depth_write_enabled = true;

    sg_rasterizer_state rs_state{};
    rs_state.cull_mode = SG_CULLMODE_BACK;

    sg_pipeline_desc pip_desc{};
    pip_desc.layout = pip_layout;
    pip_desc.shader = shd;
    pip_desc.index_type = SG_INDEXTYPE_UINT16;
    pip_desc.depth_stencil = ds_state;
    pip_desc.rasterizer = rs_state;
    pip_desc.label = "cube-pipeline";

    state.pip = sg_make_pipeline(&pip_desc);

    /* default pass action */
    sg_color_attachment_action sg_action1{};
    sg_action1 = { SG_ACTION_CLEAR, { 0.25f, 0.5f, 0.75f, 1.0f }};
    sg_pass_action sg_action{};
    sg_action.colors[0] = sg_action1;
    state.pass_action = sg_action;
}

void frame(void) {
    /* compute model-view-projection matrix for vertex shader */
    auto _proj = glm::perspective(glm::radians(60.f), (float)sapp_width()/(float)sapp_height(), 0.01f, 10.0f);
    auto _view = glm::lookAt(glm::vec3(0.f, 1.5f, 6.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    auto _vp = _proj * _view;

    state.rotate += glm::vec2(1.f, 2.f);
    auto _model = glm::rotate(glm::identity<glm::mat4>(), glm::radians(state.rotate.x), glm::vec3(1.f, 0.f, 0.f));
    _model = glm::rotate(_model, glm::radians(state.rotate.y), glm::vec3(0.f, 1.f, 0.f));
    auto _mvp = _vp * _model;

    cube_vs_params_t vs_params{};
    vs_params.mvp = _mvp;

    sg_begin_default_pass(&state.pass_action, sapp_width(), sapp_height());
    sg_apply_pipeline(state.pip);
    sg_apply_bindings(&state.bind);
    sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_cube_vs_params, &vs_params, sizeof(vs_params));
    sg_draw(0, 36, 1);
    __dbgui_draw();
    sg_end_pass();
    sg_commit();
}

void cleanup(void) {
    __dbgui_shutdown();
    sg_shutdown();
}

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    sapp_desc desc = {};
    desc.init_cb = init;
    desc.frame_cb = frame;
    desc.cleanup_cb = cleanup;
    desc.event_cb = __dbgui_event;
    desc.width = 800;
    desc.height = 600;
    desc.sample_count = 2;
    desc.window_title = "Cube";
    desc.gl_force_gles2 = true;
    return desc;
}
