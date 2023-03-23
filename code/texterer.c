// NOTE(fonsi): freetype header has to go before any #include <windows>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <cglm/cglm.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include "texterer.h"
#define SHADER_IMPLEMENTATION
#define ARENA_IMPLEMENTATION
#include "texterer_memory.h"
#include "texterer_renderer.h"

internal void ResizeWindowCallback(GLFWwindow* win, i32 w, i32 h);
internal void RenderText(Shader* s, const char* text, float x, float y, float scale, vec3 color);

uint VAO, VBO;

typedef struct Character {
    uint texture_id;
    vec2 size;
    vec2 bearing;
    uint advance;
} Character;

typedef struct HashMapCharacter {
    char key;
    Character value;
} HashMapCharacter;

HashMapCharacter* hmc = NULL;

int
main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Texterer", 0, 0);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, ResizeWindowCallback);

    glfwSwapInterval(1);

    assert(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "Failed to initialize GLAD");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    FT_Library ft;
    assert(!FT_Init_FreeType(&ft), "Could not init Freetype Library");
    FT_Face face;
    FT_New_Face(ft, "W:\\text_renderer\\fonts\\liberation_mono.ttf", 0, &face);
    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(unsigned char c = 0; c < 128; c++) {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            fprintf(stderr, "[ERROR] Failed to load Glyph\n");
            continue;
        }

        uint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        vec2 size;
        size[0] = (float) face->glyph->bitmap.width;
        size[1] = (float) face->glyph->bitmap.rows;

        vec2 bearing;
        bearing[0] = (float) face->glyph->bitmap_left;
        bearing[1] = (float) face->glyph->bitmap_top;


        Character character;
        character.texture_id = texture;
        glm_vec2_copy(size, character.size);
        glm_vec2_copy(bearing, character.bearing);
        character.advance = face->glyph->advance.x;

        hmput(hmc, c, character);
    }

    mat4 projection;
    glm_ortho(0.0f, 1920.0f, 0.0f, 1080.0f, 0.0f, 100.0f, projection);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader s = shader_create("W:\\text_renderer\\shaders\\texvert.vert",
                             "W:\\text_renderer\\shaders\\texfrag.frag");

    while(!glfwWindowShouldClose(window)) {

        glClearColor(.5f, .8f, .9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_use(&s);
        shader_set_uniform_mat4(&s, "projection", 1, GL_FALSE, projection[0]);

        vec3 color;
        color[0] = 1.0f;
        color[1] = 0.0f;
        color[2] = 0.0f;
        RenderText(&s, "Hola Mundo del Texto", 0.0f, 40.0f, 1.0f, color);

        if(glfwGetKey(window, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window, 1);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    glfwTerminate();
    return 0;
}

internal void
ResizeWindowCallback(GLFWwindow* win, i32 w, i32 h)
{
    glViewport(0, 0, w, h);
}

internal void
RenderText(Shader* s, const char* text, float x, float y, float scale, vec3 color)
{
    shader_use(s);
    shader_set_uniform_vec3(s, "textColor", 1, color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for(uint c = 0; c < strlen(text); c++) {
        char cha = text[c];
        Character ch = hmget(hmc, cha);

        float xpos = x + ch.bearing[0] * scale;
        float ypos = y - (ch.size[1] - ch.bearing[1]) * scale;

        float w = ch.size[0] * scale;
        float h = ch.size[1] * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.texture_id);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
