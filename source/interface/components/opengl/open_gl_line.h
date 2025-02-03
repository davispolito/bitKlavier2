//
// Created by Davis Polito on 1/31/25.
//

#ifndef BITKLAVIER2_OPEN_GL_INSTANCED_LINES_H
#define BITKLAVIER2_OPEN_GL_INSTANCED_LINES_H
#include "open_gl_component.h"

class OpenGlLine : public OpenGlComponent
{
public:
    OpenGlLine(juce::Component* start_component, juce::Component* end_component, juce::Component* target,
               Shaders::FragmentShader shader = Shaders::kColorFragment) :
               OpenGlComponent("line"), active_(true), start_component_(start_component), end_component_(end_component),
               target_component_(target)
    {
        data_ = std::make_unique<float[]>(4);
        indices_ = std::make_unique<int[]>(2);
        vertex_buffer_ = 0;
        indices_[0] = 0;
        indices_[1] = 1;
        data_[0] = start_component->getX() / target_component_->getWidth();
        data_[1] =1.0f -  start_component->getY()/ target_component_->getHeight() ;
        data_[2] = end_component->getX()/ target_component_->getWidth();
        data_[3] = 1.0f - end_component->getY()/ target_component_->getHeight();
    }
    void init(OpenGlWrapper &open_gl)
    {
        //set up the points
        open_gl.context.extensions.glGenBuffers(1, &vertex_buffer_);
        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ARRAY_BUFFER, vertex_buffer_);

        GLsizeiptr vert_size = static_cast<GLsizeiptr> (4 *sizeof(float));
        open_gl.context.extensions.glBufferData(juce::gl::GL_ARRAY_BUFFER, vert_size, data_.get(), juce::gl::GL_STATIC_DRAW);

        //set up how they connect
        open_gl.context.extensions.glGenBuffers(1, &indices_buffer_);
        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ELEMENT_ARRAY_BUFFER, indices_buffer_);

        GLsizeiptr bar_size = static_cast<GLsizeiptr>(4 * sizeof(int));
        open_gl.context.extensions.glBufferData(juce::gl::GL_ELEMENT_ARRAY_BUFFER, bar_size, indices_.get(), juce::gl::GL_STATIC_DRAW);

        shader_ = open_gl.shaders->getShaderProgram(Shaders::kPassthroughVertex, fragment_shader_);
        shader_->use();
    }
    void render(OpenGlWrapper &open_gl, bool animate)
    {
        juce::Component *component = target_component_ ? target_component_ : this;
        if (!active_ || (!draw_when_not_visible_ && !component->isVisible()) || !setViewPort(component, open_gl))
            return;

        if (shader_ == nullptr)
            init(open_gl);

        if(dirty_)
        {
            dirty_ = false;
//            // Dynamically update the vertex positions (move the points)
//            GLfloat updatedVertices[] = {
//                    -0.8f, -0.8f, 0.0f,  // New first endpoint (move left)
//                    0.8f,  0.8f, 0.0f   // New second endpoint (move right)
//            };
//
//            // Bind the VBO and update the vertex data
//            glBindBuffer(GL_ARRAY_BUFFER, vbo);
//            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(updatedVertices), updatedVertices); // Update VBO with new vertices
        }

        shader_->use();

        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ARRAY_BUFFER, vertex_buffer_);
        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ELEMENT_ARRAY_BUFFER, indices_buffer_);

        juce::gl::glDrawElements(juce::gl::GL_LINE,2, juce::gl::GL_UNSIGNED_INT, nullptr);
        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ARRAY_BUFFER, 0);
        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    bool dirty_;
    bool active_;
    bool draw_when_not_visible_ = false;
    Shaders::FragmentShader fragment_shader_;
    juce::OpenGLShaderProgram *shader_;
    std::unique_ptr<int[]> indices_;
    GLuint vertex_buffer_;
    GLuint indices_buffer_;
    std::unique_ptr<float[]> data_;
    juce::Component *target_component_;
    juce::Component *scissor_component_;
    juce::Component *start_component_;
    juce::Component *end_component_;
};
#endif //BITKLAVIER2_OPEN_GL_INSTANCED_LINES_H
