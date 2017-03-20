#ifndef STANDARDSHADER_H
#define STANDARDSHADER_H

#include "pch.h"

enum StandardVertexAttributeLocation
{
    ATTRIB_POSITION = 0,
    ATTRIB_NORMAL = 1,
    ATTRIB_TEXCOORD = 2,
    ATTRIB_COLOR = 3,
    ATTRIB_INDEX = 4
};

/**
 * @brief The StandardVertexAttribute class
 * The StandardVertexAttribute class defines an input dataset for the
 * StandardShader class.
 *
 * The StandardVertexAttribute class is basically one OpenGL vertex array
 * object and five OpenGL buffer objects. Position, normal, texcoord, per-
 * vertex-color and index can be automatically mapped into the StandardShader.
 *
 * Note that you don't need to call Bind() before any function calls.
 *
 * To use it, first call Create() to initialize the object. Next, upload data
 * by calling BufferData(). You don't need to provide every data type. Only those
 * non-empty-vector inputs are enabled(by internally calling glEnableVertexAttribArray()).
 * You can use glVertexAttrib* to update the non-array attributes at locations defined in
 * enum StandardVertexAttributeLocation. Color3f is provided as a easy way to update
 * non-array color value because it is common to draw everything with a certain color when
 * we draw points and lines as data visualizations.
 */
class StandardVertexAttribute
{
public:
    StandardVertexAttribute();

    /// Creates a full-screen quad with texcoords for full-screen image passes.
    static StandardVertexAttribute CreateQuadAttribute();

    void Create();
    void Destroy();
    void Bind();
    void BufferData(std::vector<float> &positions,
                    std::vector<float> &normals,
                    std::vector<float> &texcoords,
                    std::vector<float> &colors,
                    std::vector<unsigned int> &indices);
    void Color3f(float r, float g, float b);
    inline GLsizei VertexCount() { return vertexCount; }
    inline GLsizei IndexCount() { return indexCount; }

private:
    GLuint vao;
    GLuint buffers[5];
    GLsizei vertexCount;
    GLsizei indexCount;
};

enum StandardShaderRenderMode
{
    RENDER_PHONG = 0,
    RENDER_NORMAL = 1,
    RENDER_DEPTH = 2,
    RENDER_COLORED = 3,
    RENDER_TEXTURED = 4,
    RENDER_PHONG_COLORED = 5
};

/**
 * @brief The StandardShader class
 * The StandardShader class implements a flexible OpenGL shader for various rendering tasks.
 *
 * Note that you don't need to call Bind() before any function calls.
 *
 * To use StandardShader to draw something in your OpenGL view, you must define inputs
 * by creating a StandardVertexAttribute object, and call Create() on the instance of
 * StandardShader object. Before the rendering, you must provide necessary settings by
 * calling Set*() functions. Calling Draw() will cause the rendering to happen.
 *
 * Example: Draw a Phong-shaded model
 * {
 *     StandardVertexAttribute attrib = CreateStandardVertexAttribute("my_model.obj");
 *     StandardShader shader;
 *     shader.Create();
 *     shader.SetModelMatrix(GetModelMatrix());
 *     shader.SetMVPMarix(GetMVPMatrix());
 *     shader.SetLightDirection(QVector3D(1, 1, 1));
 *     shader.SetEyePosition(QVector3D(0, 0, 5));
 *     shader.SetRenderMode(RENDER_PHONG);
 *     shader.Draw(GL_TRIANGLES, attrib);
 * }
 *
 * Example: Draw a full-screen image
 * {
 *     QImage image = LoadImage("my_image.png");
 *     StandardVertexAttribute attrib = StandardVertexAttribute::CreateQuadAttribute();
 *     StandardShader shader;
 *     shader.Create();
 *     // if you have rendered something before,
 *     // don't forget to restore M and MVP to identiy matrices!
 *     // shader.SetMVPMatrix(QMatrix4x4());
       // shader.SetModelMatrix(QMatrix4x4());
 *     shader.SetTexture(image);
 *     shader.SetRenderMode(RENDER_TEXTURED);
 *     shader.Draw(GL_TRIANGLES, attrib);
 * }
 *
 * Example: Draw some red points
 * {
 *     StandardVertexAttribute attrib;
 *     attrib.Create();
 *     vector<float> points = GetPoints();
 *     vector<float> empty;
 *     vector<unsigned int> emptyIdx;
 *     attrib.BufferData(points, empty, empty, empty, emptyIdx);
 *     attrib.Color3f(1.0f, 0, 0);
 *     StandardShader shader;
 *     shader.Create();
 *     shader.SetMVPMarix(GetMVPMatrix());
 *     shader.SetRenderMode(RENDER_COLORED);
 *     shader.Draw(GL_TRIANGLES, attrib);
 * }
 */
class StandardShader
{
public:
    StandardShader();

    void Create();
    void Destroy();
    void Bind();
    void SetModelMatrix(QMatrix4x4 matrix);
    void SetMVPMatrix(QMatrix4x4 matrix);
    void SetLightDirection(QVector3D vec);
    void SetEyePosition(QVector3D vec);
    void SetTexture(QImage texture);
    void SetRenderMode(StandardShaderRenderMode mode);
    void Draw(GLenum mode, StandardVertexAttribute &attribute);
private:
    QOpenGLShaderProgram program;
    QOpenGLTexture *texture;
};

#endif // STANDARDSHADER_H
