//
// Created by Davis Polito on 1/31/25.
//

#ifndef BITKLAVIER2_OPEN_GL_INSTANCED_LINES_H
#define BITKLAVIER2_OPEN_GL_INSTANCED_LINES_H
#include "open_gl_component.h"

class OpenGlLine : public OpenGlComponent
{
public:
//    OpenGlLine(juce::Component* start_component, juce::Component* end_component, juce::Component* target,
//               Shaders::FragmentShader shader = Shaders::kBlackFragment) :
//               OpenGlComponent("line"), active_(true), start_component_(start_component), end_component_(end_component),
//               target_component_(target),fragment_shader_(shader)
//    {
//        data_ = std::make_unique<float[]>(4);
//        indices_ = std::make_unique<int[]>(2);
//        vertex_buffer_ = 0;
//        indices_[0] = 0;
//        indices_[1] = 1;
//        data_[0] = static_cast<float>(start_component->getX() )/ static_cast<float>(target_component_->getWidth());
//        data_[1] =1.0f -   static_cast<float>(start_component->getY())/  static_cast<float>(target_component_->getHeight()) ;
//        data_[2] =  static_cast<float>(end_component->getX())/  static_cast<float>(target_component_->getWidth());
//        data_[3] = 1.0f -  static_cast<float>(end_component->getY())/  static_cast<float>(target_component_->getHeight());
//// Debugging data at construction
////
////        float vertices[] = {
////                -0.5f, 0.0f, 0.25f, 0.0f
////
////        };
////        float vertices[] = {
////                -1.0f, -1.0f, // Bottom-left of clip space
////                1.0f,  1.0f , // Top-right of clip space
////                0.5f, 0.5,
////        };
//
////        float triangleVertices[] = {
////                1.f, 1.f,  // top right  corner
////                -0.5f,-0.5f // Bottomleft corner
//////               , 0.0f,0.0f,
////        };
////        data_.reset(triangleVertices);
//// Compute centroid (average x and y)
////y        float centroidX = 0.0f, centroidY = 0.0f;
////        for (int i = 0; i < 6; i += 2) { // Step by 2 because we want (x, y) pairs
////            centroidX += triangleVertices[i];     // Sum up all x values
////            centroidY += triangleVertices[i + 1]; // Sum up all y values
////        }
////        centroidX /= 3; // Divide by number of vertices (3)
////        centroidY /= 3; // Divide by number of vertices (3)
////
////// Adjust each vertex position to center the triangle at (0, 0)
////        for (int i = 0; i < 6; i += 2) {
////            triangleVertices[i]     -= centroidX; // Subtract centroid x from each x
////            triangleVertices[i + 1] -= centroidY; // Subtract centroid y from each y
////        }
//
//// Copy the vertex data into 'data_' using memcpy
////        memcpy(data_.get(), triangleVertices, sizeof(triangleVertices));
//
//
//        DBG("Indices: " + juce::String(indices_[0]) + ", " + juce::String(indices_[1]));
//        DBG("Vertex Data: X1 = " + juce::String(data_[0]) + ", Y1 = " + juce::String(data_[1])
//            + ", X2 = " + juce::String(data_[2]) + ", Y2 = " + juce::String(data_[3]));
//
////        DBG("Start Component Position: (" + juce::String(start_component->getX()) + ", "
////            + juce::String(start_component->getY()) + ")");
////        DBG("End Component Position: (" + juce::String(end_component->getX()) + ", "
//////            + juce::String(end_component->getY()) + ")");
////        DBG("Target Component Size: (" + juce::String(target_component_->getWidth()) + ", "
////            + juce::String(target_component_->getHeight()) + ")");
//
//    }
    OpenGlLine(juce::Component* start_component, juce::Component* end_component, juce::Component* target,
               Shaders::FragmentShader shader = Shaders::kBlackFragment) :
            OpenGlComponent("line"),
            active_(true),
            start_component_(start_component),
            end_component_(end_component),
            target_component_(target),
            fragment_shader_(shader) {

        // Check that components are valid
        if (!start_component_ || !end_component_ || !target_component_) {
            DBG("Error: Invalid components supplied to OpenGlLine");
            return;
        }

        // Utility lambda to calculate clip-space coordinates
        auto getClipSpaceCoordinates = [](juce::Component* component, juce::Component* target) -> juce::Point<float> {
            if (!component || !target) {
                jassertfalse; // Shouldn't happen in a working system
                return {0.0f, 0.0f};
            }

            // Get the position of the component relative to its parent (target)
            juce::Point<int> localPosition = component->getBounds().getCentre();

            // Normalize the coordinates to the range [0, 1]
            float normX = static_cast<float>(localPosition.getX()) / static_cast<float>(target->getWidth());
            float normY = static_cast<float>(localPosition.getY()) / static_cast<float>(target->getHeight());

            // Map normalized coordinates to OpenGL clip space [-1, 1]
            float clipX = normX * 2.0f - 1.0f;
            float clipY = 1.0f - normY * 2.0f; // Flip Y-axis for OpenGL clip space

            return {clipX, clipY};
        };

        // Calculate the clip-space coordinates for the start and end components
        juce::Point<float> startClip = getClipSpaceCoordinates(start_component_, target_component_);
        juce::Point<float> endClip = getClipSpaceCoordinates(end_component_, target_component_);

        // Debug outputs to verify the calculated coordinates
        DBG("------------ OpenGlLine Created ------------");
        DBG("Start Component Clip Space Coordinates: (" + juce::String(startClip.getX()) +
            ", " + juce::String(startClip.getY()) + ")");
        DBG("End Component Clip Space Coordinates: (" + juce::String(endClip.getX()) +
            ", " + juce::String(endClip.getY()) + ")");
        DBG("-------------------------------------------");

        // Store the data in the vertex buffer format (clip-space coordinates)
        data_ = std::make_unique<float[]>(4);
        data_[0] = startClip.getX();
        data_[1] = startClip.getY();
        data_[2] = endClip.getX();
        data_[3] = endClip.getY();

        // Set default indices for drawing the line
        indices_ = std::make_unique<int[]>(2);
        indices_[0] = 0;
        indices_[1] = 1;

        vertex_buffer_ = 0;

        // Debugging additional information
        DBG("Line Vertex Data: X1 = " + juce::String(data_[0]) + ", Y1 = " + juce::String(data_[1]) +
            ", X2 = " + juce::String(data_[2]) + ", Y2 = " + juce::String(data_[3]));
    }
    void paint(juce::Graphics &g) override
    {

    }
    void paintBackground(juce::Graphics &g) override
    {

    }
    void init(OpenGlWrapper &open_gl)
    {
        //set up the points
        open_gl.context.extensions.glGenBuffers(1, &vertex_buffer_);
        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ARRAY_BUFFER, vertex_buffer_);
        jassert(vertex_buffer_ != 0); // Ensure the buffer was successfully created

        GLsizeiptr vert_size = static_cast<GLsizeiptr> (4 *sizeof(float));
        open_gl.context.extensions.glBufferData(juce::gl::GL_ARRAY_BUFFER, vert_size, data_.get(), juce::gl::GL_STATIC_DRAW);

//        //set up how they connect
        open_gl.context.extensions.glGenBuffers(1, &indices_buffer_);
        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ELEMENT_ARRAY_BUFFER, indices_buffer_);
//
        GLsizeiptr bar_size = static_cast<GLsizeiptr>(2* sizeof(int));
        open_gl.context.extensions.glBufferData(juce::gl::GL_ELEMENT_ARRAY_BUFFER, bar_size, indices_.get(), juce::gl::GL_STATIC_DRAW);

        shader_ = open_gl.shaders->getShaderProgram(Shaders::kSimpleLineVertex, fragment_shader_);
        shader_->use();
        if (!shader_->getLastError().isEmpty()) {
            DBG(shader_->getLastError());
            jassertfalse; // Shader compilation/linking failed
        }

        // Unbind the buffer (optional safety)
        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ARRAY_BUFFER, 0);

        float debugData[4]; // Update the size based on your vertex count and data format (6 floats for 3 vertices with 2D positions)
        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ARRAY_BUFFER, vertex_buffer_);
        juce::gl::glGetBufferSubData(  juce::gl::GL_ARRAY_BUFFER, 0, sizeof(debugData), debugData);

// Format the vertex data into a single debug string
        juce::String vertexBufferData;
        for (int i = 0; i < 4; ++i) {
            vertexBufferData += juce::String(debugData[i]) + ", ";
        }
        DBG("Vertex Buffer Data: " + vertexBufferData);

        // Unbind the buffer (optional safety)
        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ARRAY_BUFFER, 0);
        _init = true;
        }
        bool _init = false;
    bool isInit() override
    {
        return _init;
    }
    void render(OpenGlWrapper &open_gl, bool animate)
    {

        juce::Component *component = target_component_ ? target_component_ : this;
        if (!active_ || (!draw_when_not_visible_ && !component->isVisible()) || !setViewPort(component, open_gl))
            return;
//         i = i + 1;
//       if (i > component->getHeight())
//           i = component->getHeight() * -1;
//        juce::gl::glViewport(20, 0, component->getWidth(), component->getHeight());

DBG(i);
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
//        juce::gl::glClearColor(1.f,0.0f,0.0f, 1.0f); // Black background
//        juce::gl::glClear(juce::gl::GL_COLOR_BUFFER_BIT | juce::gl::GL_DEPTH_BUFFER_BIT);
//
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

        open_gl.context.extensions.glBindBuffer(juce::gl::GL_ELEMENT_ARRAY_BUFFER, indices_buffer_);
//        juce::gl::glDrawArrays(juce::gl::GL_LINES, 0, 3);
        juce::gl::glDrawElements(juce::gl::GL_LINES,2, juce::gl::GL_UNSIGNED_INT, nullptr);
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
int i = 0;
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
