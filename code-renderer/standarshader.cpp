#include "standarshader.h"

using namespace std;

StandardVertexAttribute::StandardVertexAttribute()
{
    memset(this, 0, sizeof(StandardVertexAttribute));
}

StandardVertexAttribute StandardVertexAttribute::CreateQuadAttribute()
{
    const float QUAD_VERTEX[12] =
    {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
    };
    const float QUAD_TEXCOORD[8] =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };
    const unsigned int QUAD_INDEX[6] =
    {
        0, 1, 2,
        2, 3, 0
    };
    StandardVertexAttribute attribute;
    attribute.Create();
    vector<float> positions(QUAD_VERTEX, QUAD_VERTEX + 12);
    vector<float> empty;
    vector<float> texcoords(QUAD_TEXCOORD, QUAD_TEXCOORD + 8);
    vector<unsigned int> indices(QUAD_INDEX, QUAD_INDEX + 6);
    attribute.BufferData(positions, empty, texcoords, empty, indices);
    return attribute;
}

void StandardVertexAttribute::Create()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(5, buffers);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[ATTRIB_POSITION]);
    glVertexAttribPointer(ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[ATTRIB_NORMAL]);
    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[ATTRIB_TEXCOORD]);
    glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[ATTRIB_COLOR]);
    glVertexAttribPointer(ATTRIB_COLOR, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[ATTRIB_INDEX]);
    glVertexAttribPointer(ATTRIB_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
}

void StandardVertexAttribute::Destroy()
{
    if(vao != 0)
    {
        glBindVertexArray(vao);
        glDeleteBuffers(5, buffers);
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &vao);
    }
}

void StandardVertexAttribute::Bind()
{
    glBindVertexArray(vao);
}

void StandardVertexAttribute::BufferData(vector<float> &positions,
                                         vector<float> &normals,
                                         vector<float> &texcoords,
                                         vector<float> &colors,
                                         vector<unsigned int> &indices)
{
    glBindVertexArray(vao);

    vertexCount = (GLsizei)positions.size() / 3;
    indexCount = (GLsizei)indices.size();

    if(positions.size() != 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[ATTRIB_POSITION]);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(ATTRIB_POSITION);
    }
    if(normals.size() != 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[ATTRIB_NORMAL]);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(ATTRIB_NORMAL);
    }
    if(texcoords.size() != 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[ATTRIB_TEXCOORD]);
        glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(float), texcoords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(ATTRIB_TEXCOORD);
    }
    if(colors.size() != 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, buffers[ATTRIB_COLOR]);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(ATTRIB_COLOR);
    }
    if(indices.size() != 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[ATTRIB_INDEX]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(ATTRIB_INDEX);
    }
    glBindVertexArray(0);
}

void StandardShader::Draw(GLenum mode, StandardVertexAttribute &attribute)
{
    if(attribute.IndexCount() == 0)
    {
        if(attribute.VertexCount() == 0)
        {
            return;
        }
        program.bind();
        attribute.Bind();
        glDrawArrays(mode, 0, attribute.VertexCount());
        return;
    }
    program.bind();
    attribute.Bind();
    glDrawElements(mode, attribute.IndexCount(), GL_UNSIGNED_INT, 0);
    //std::cout << "gl error code : " << glGetError() << std::endl;
}

void StandardVertexAttribute::Color3f(float r, float g, float b)
{
    glBindVertexArray(vao);
    glVertexAttrib3f(ATTRIB_COLOR, r, g, b);
    glBindVertexArray(0);
}

StandardShader::StandardShader()
{
    texture = 0;
}

void StandardShader::Create()
{
    program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/glsl/standarshader.vs.glsl");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/glsl/standarshader.fs.glsl");
    program.link();
    printf("%s\n", program.log().toStdString().c_str());
}

void StandardShader::Destroy()
{
    program.removeAllShaders();
}

void StandardShader::Bind()
{
    program.bind();
}

void StandardShader::SetModelMatrix(QMatrix4x4 matrix)
{
    program.bind();
    program.setUniformValue("m", matrix);
}

void StandardShader::SetMVPMatrix(QMatrix4x4 matrix)
{
    program.bind();
    program.setUniformValue("mvp", matrix);
}

void StandardShader::SetLightDirection(QVector3D vec)
{
    program.bind();
    program.setUniformValue("lightDirection", vec);
}

void StandardShader::SetEyePosition(QVector3D vec)
{
    program.bind();
    program.setUniformValue("eyePosition", vec);
}

void StandardShader::SetRenderMode(StandardShaderRenderMode mode)
{
    program.bind();
    program.setUniformValue("renderMode", (int) mode);
}

void StandardShader::SetTexture(QImage image)
{
    program.bind();
    glActiveTexture(GL_TEXTURE0);
    program.setUniformValue("myTexture", 0);
    if(texture != 0)
    {
        delete texture;
    }
    texture = new QOpenGLTexture(image, QOpenGLTexture::GenerateMipMaps);
    texture->bind();
}




//fine the piblic of viewMngr and attribute
