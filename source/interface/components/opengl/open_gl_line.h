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
               Shaders::FragmentShader shader = Shaders::kBlackFragment) :
               OpenGlComponent("line"), active_(true), start_component_(start_component), end_component_(end_component),
               target_component_(target),fragment_shader_(shader)
    {
        data_ = std::make_unique<float[]>(6);
        indices_ = std::make_unique<int[]>(2);
        vertex_buffer_ = 0;
        indices_[0] = 0;
        indices_[1] = 1;
//        data_[0] = static_cast<float>(start_component->getX() )/ static_cast<float>(target_component_->getWidth());
//        data_[1] =1.0f -   static_cast<float>(start_component->getY())/  static_cast<float>(target_component_->getHeight()) ;
//        data_[2] =  static_cast<float>(end_component->getX())/  static_cast<float>(target_component_->getWidth());
//        data_[3] = 1.0f -  static_cast<float>(end_component->getY())/  static_cast<float>(target_component_->getHeight());
// Debugging data at construction

//        float vertices[] = {
//                -0.5f, 0.0f, 0.25f, 0.0f
//
//        };
//        float vertices[] = {
//                -1.0f, -1.0f, // Bottom-left of clip space
//                1.0f,  1.0f , // Top-right of clip space
//                0.5f, 0.5,
//        };

        float triangleVertices[] = {
                -0.5f, -0.5f,  // Bottom-left corner
                0.5f, -0.5f,  // Bottom-right corner
                0.0f,  0.5f   // Top-center
        };
        data_.reset(triangleVertices);

        DBG("Indices: " + juce::String(indices_[0]) + ", " + juce::String(indices_[1]));
        DBG("Vertex Data: X1 = " + juce::String(data_[0]) + ", Y1 = " + juce::String(data_[1])
            + ", X2 = " + juce::String(data_[2]) + ", Y2 = " + juce::String(data_[3]));

//        DBG("Start Component Position: (" + juce::String(start_component->getX()) + ", "
//            + juce::String(start_component->getY()) + ")");
//        DBG("End Component Position: (" + juce::String(end_component->getX()) + ", "
//            + juce::String(end_component->getY()) + ")");
        DBG("Target Component Size: (" + juce::String(target_component_->getWidth()) + ", "
            + juce::String(target_component_->getHeight()) + ")");

    }
    void init(OpenGlWrapper &open_gl)
    {
        //set up the points
        open_gl.context.extensions.glGenBuffers(1, &vertex_buffer_);
        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ARRAY_BUFFER, vertex_buffer_);
        jassert(vertex_buffer_ != 0); // Ensure the buffer was successfully created

        GLsizeiptr vert_size = static_cast<GLsizeiptr> (6 *sizeof(float));
        // Upload the vertex data to the GPU
        GLfloat vertices[] = {
                -0.5f, -0.5f,  // Bottom-left corner
                0.5f, -0.5f,  // Bottom-right corner
                0.0f,  0.5f   // Top-center
        };

        open_gl.context.extensions.glBufferData(juce::gl::GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                 juce::gl::GL_STATIC_DRAW);

//        //set up how they connect
//        open_gl.context.extensions.glGenBuffers(1, &indices_buffer_);
//        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ELEMENT_ARRAY_BUFFER, indices_buffer_);
//
//        GLsizeiptr bar_size = static_cast<GLsizeiptr>(2* sizeof(int));
//        open_gl.context.extensions.glBufferData(juce::gl::GL_ELEMENT_ARRAY_BUFFER, bar_size, indices_.get(), juce::gl::GL_STATIC_DRAW);

        shader_ = open_gl.shaders->getShaderProgram(Shaders::kSimpleLineVertex, fragment_shader_);
        shader_->use();
        if (!shader_->getLastError().isEmpty()) {
            DBG(shader_->getLastError());
            jassertfalse; // Shader compilation/linking failed
        }
        // Unbind the buffer (optional safety)
        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ARRAY_BUFFER, 0);

        float debugData[6]; // Update the size based on your vertex count and data format (6 floats for 3 vertices with 2D positions)
        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ARRAY_BUFFER, vertex_buffer_);
        juce::gl::glGetBufferSubData(  juce::gl::GL_ARRAY_BUFFER, 0, sizeof(debugData), debugData);

// Format the vertex data into a single debug string
        juce::String vertexBufferData;
        for (int i = 0; i < 6; ++i) {
            vertexBufferData += juce::String(debugData[i]) + ", ";
        }
        DBG("Vertex Buffer Data: " + vertexBufferData);
        // Unbind the buffer (optional safety)
        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ARRAY_BUFFER, 0);

        }
    void render(OpenGlWrapper &open_gl, bool animate)
    {
        juce::Component *component = this;// target_component_ ? target_component_ : this;
        if (!active_ || (!draw_when_not_visible_ && !component->isVisible()) || !setViewPort(component, open_gl))
            return;
//        juce::gl::glViewport(0, 0, component->getWidth(), component->getHeight());


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
        // Clear the screen
//        juce::gl::glClearColor(1.f,1.f,1.f, 1.0f); // Black background
//        juce::gl::glClear(juce::gl::GL_COLOR_BUFFER_BIT | juce::gl::GL_DEPTH_BUFFER_BIT);

//        juce::gl::glDisable(juce::gl::GL_BLEND);
//        juce::gl::glDisable(juce::gl::GL_SCISSOR_TEST);

        shader_->use();
        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ARRAY_BUFFER, vertex_buffer_);


        open_gl.context.extensions.glVertexAttribPointer(
                0,                // Attribute index (matches `position`)
                2,                // 2 components per vertex (x, y)
                juce::gl::GL_FLOAT, // Data type
                juce::gl::GL_FALSE, // Do not normalize
                2 * sizeof(float),  // Stride (two floats per vertex)
                nullptr             // Offset to the first component
        );

//        open_gl.context.extensions. glVertexAttribPointer(0, 2, juce::gl::GL_FLOAT, juce::gl::GL_FALSE, 2 * sizeof(float), nullptr); // Adjust stride if needed
        open_gl.context.extensions. glEnableVertexAttribArray(0); // Index 0 corresponds to the attribute

//        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ELEMENT_ARRAY_BUFFER, indices_buffer_);
        juce::gl::glDrawArrays(juce::gl::GL_TRIANGLES, 0, 3);
        //juce::gl::glDrawElements(juce::gl::GL_LINES,2, juce::gl::GL_UNSIGNED_INT, nullptr);
        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ARRAY_BUFFER, 0);
//        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ELEMENT_ARRAY_BUFFER, 0);
        GLint positionLocation = open_gl.context.extensions.glGetAttribLocation(shader_->getProgramID(), "position");
        //DBG("Position Attribute Location: " + juce::String(positionLocation));
        jassert(positionLocation == 0); // Ensure location matches vertex attribute index
//        float debugData[6];
//        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ARRAY_BUFFER, vertex_buffer_);
//        juce::gl::glGetBufferSubData(  juce::gl::GL_ARRAY_BUFFER, 0, sizeof(debugData), debugData);
//        juce::Array<float> aa(debugData,6);
//        juce::StringArray a(aa) ;
//        DBG("Vertex Buffer Data: " + juce::StringArray(aa).joinIntoString(", "));



    }
    void resized() override
    {
        OpenGlComponent::resized();
        dirty_ = true;
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
