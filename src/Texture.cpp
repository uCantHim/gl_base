#include "Texture.h"

#include <array>
#include <random>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

#include <IL/il.h>



glb::Texture::Texture()
    : Texture(UNINITIALIZED_COLOR)
{
}

glb::Texture::Texture(vec4 color)
{
    loadColor(color);
}

glb::Texture::Texture(uvec2 size, GLenum internalFormat, vec4 color)
{
    create(size, internalFormat);

    color *= UCHAR_MAX;
    auto byteColor = static_cast<tvec4<GLubyte>>(color);
    glClearTexImage(*textureHandle, 0, GL_RGBA, GL_UNSIGNED_BYTE, &byteColor);
}

glb::Texture::Texture(const std::string& imagePath)
{
	loadImage(imagePath);
}

glb::Texture::Texture(GLuint texHandle)
    :
    textureHandle(texHandle)
{
    if (!glIsTexture(texHandle))
        throw std::runtime_error("Passed handle is not a texture!");
}

glb::Texture::Texture(const ArrayTexture& src, size_t layer)
{
    create(src.getSize(), src.getInternalFormat());

    glCopyImageSubData(
        *src,
        GL_TEXTURE_2D_ARRAY,
        0, // level
        0, 0, layer, // src offset
        *textureHandle,
        GL_TEXTURE_2D,
        0, // level
        0, 0, 0, // dst offset
        size.x, size.y, 1 // size
    );
}

auto glb::Texture::operator*() const noexcept -> GLuint
{
    return *textureHandle;
}

void glb::Texture::loadImage(const std::string& imagePath)
{
    if (!fs::is_regular_file(imagePath))
    {
        loadColor(UNINITIALIZED_COLOR);
        std::cout << "Not a valid image path: " << imagePath << "!\n";
    }

	// Load image data
	ILboolean success;
	ILuint imageID;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT); // match image origin with OpenGL�s

	success = ilLoadImage(imagePath.c_str());
	if (!success)
	{
		ilDeleteImage(imageID);
		throw std::runtime_error("Error in ilLoadImage(): " + std::to_string(ilGetError()));
	}
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    uvec2 size{ ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT) };
    create(size, GL_RGBA8);
    copyRawData(ilGetData(), size, uvec2{0, 0}, GL_RGBA, GL_UNSIGNED_BYTE);

	ilDeleteImage(imageID);
}

void glb::Texture::loadColor(vec4 color)
{
    create(uvec2{1, 1}, GL_RGBA8);

	color *= UCHAR_MAX;
    auto byteVec = static_cast<tvec4<GLubyte>>(color);
    glClearTexImage(*textureHandle, 0, GL_RGBA, GL_UNSIGNED_BYTE, &byteVec);
}

void glb::Texture::pack() const
{
    glBindTexture(GL_TEXTURE_2D, *textureHandle);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

void glb::Texture::unpack(uvec2 dstOffset, uvec2 copySize, size_t bufferOffset) const
{
    copySize = min(size, copySize);
    glTextureSubImage2D(
        *textureHandle,
        0, dstOffset.x, dstOffset.y,
        copySize.x, copySize.y,
        GL_RGBA, GL_UNSIGNED_BYTE,
        &bufferOffset
    );
}

void glb::Texture::bind(unsigned int bindingPoint) const
{
    glActiveTexture(GL_TEXTURE0 + bindingPoint);
    glBindTexture(GL_TEXTURE_2D, *textureHandle);
}

auto glb::Texture::getSize() const noexcept -> uvec2
{
    return size;
}

auto glb::Texture::getTextureHandle() const noexcept -> GLuint
{
    return *textureHandle;
}

void glb::Texture::copyRawData(
    void* buf,
    uvec2 size,
    uvec2 dstOffset,
    GLenum srcFormat,
    GLenum srcType)
{
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0); // No accidental copying from buffer
	glTextureSubImage2D(
		*textureHandle,
		0, // level
        dstOffset.x, dstOffset.y, // offset
        size.x, size.y, // size
		srcFormat, srcType, // src formats
		buf
	);
}

void glb::Texture::create(uvec2 size, GLenum internalFormat)
{
    textureHandle.release();
    glCreateTextures(GL_TEXTURE_2D, 1, &textureHandle);
    glTextureStorage2D(*textureHandle, 1, internalFormat, size.x, size.y);

    this->size = size;
    this->internalFormat = internalFormat;
}





glb::ArrayTexture::ArrayTexture(uvec2 size, size_t layers, GLenum format)
    :
    internalFormat(format)
{
    create(size, layers, format);
}

glb::ArrayTexture::ArrayTexture(const std::vector<std::string>& imagePaths)
{
    uvec2 maxSize(0);
    std::vector<Texture> textures;
    textures.reserve(imagePaths.size());
    for (auto& path : imagePaths)
    {
        auto& tex = textures.emplace_back(path);
        maxSize = max(maxSize, tex.getSize());
    }

    create(maxSize, static_cast<GLsizei>(textures.size()), GL_RGBA8);

    for (size_t i = 0; i < textures.size(); i++)
    {
        const auto& texture = textures[i];
        copyImage(texture, texture.getSize(), uvec2(0, 0), i);
    }
}

glb::ArrayTexture::ArrayTexture(const std::vector<Texture>& textures, GLenum format)
    :
    internalFormat(format)
{
    uvec2 maxSize(0);
    for (const auto& texture : textures)
        maxSize = max(maxSize, texture.getSize());

    create(maxSize, static_cast<GLsizei>(textures.size()), format);

    for (size_t i = 0; i < numLayers; i++)
    {
        const auto& texture = textures[i];
        copyImage(texture, texture.getSize(), uvec2(0, 0), i);
    }
}

glb::ArrayTexture::ArrayTexture(std::vector<vec4> colors)
    :
    internalFormat(GL_RGBA8)
{
    create(uvec2(1, 1), colors.size(), GL_RGBA8);

    for (size_t i = 0; i < colors.size(); i++)
    {
        colors[i] *= UCHAR_MAX;
        auto byteVec = static_cast<tvec4<GLubyte>>(colors[i]);
        copyRawData(&byteVec, uvec2(1, 1), uvec2(0, 0), i);
    }
}

auto glb::ArrayTexture::operator*() const noexcept -> GLuint
{
    return *textureHandle;
}

void glb::ArrayTexture::bind(GLsizei unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D_ARRAY, *textureHandle);
}

void glb::ArrayTexture::loadImage(const std::string& imagePath, size_t layer)
{
    Texture tex(imagePath);
    copyImage(tex, min(tex.getSize(), layerSize), uvec2(0, 0), layer);
}

void glb::ArrayTexture::copyImage(
    const Texture& src,
    uvec2 size,
    uvec2 dstOffset,
    size_t layer)
{
    assert(layer < numLayers);
    assert(size.x <= layerSize.x && size.y <= layerSize.y);

    glCopyImageSubData(
        *src,
        GL_TEXTURE_2D,
        0,
        0, 0, 0, // src offset
        *textureHandle,
        GL_TEXTURE_2D_ARRAY,
        0,
        dstOffset.x, dstOffset.y, layer, // dst offset
        size.x, size.y, 1 // size
    );
}

void glb::ArrayTexture::copyRawData(
    void* buf,
    uvec2 size,
    uvec2 dstOffset,
    size_t layer,
    GLenum externalFormat,
    GLenum srcType)
{
    assert(layer < numLayers);
    assert(size.x <= layerSize.x && size.y <= layerSize.y);

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glTextureSubImage3D(
        *textureHandle,
        0,
        dstOffset.x, dstOffset.y, static_cast<GLint>(layer),
        size.x, size.y, 1,
        externalFormat, srcType,
        buf
    );
}

auto glb::ArrayTexture::extractLayer(size_t layer) -> Texture
{
    assert(layer < numLayers);
    return Texture(*this, layer);
}

auto glb::ArrayTexture::getSize() const noexcept -> uvec2
{
    return layerSize;
}

auto glb::ArrayTexture::getNumLayers() const noexcept -> size_t
{
    return numLayers;
}

auto glb::ArrayTexture::getInternalFormat() const noexcept -> GLenum
{
    return internalFormat;
}

auto glb::ArrayTexture::getTextureHandle() const noexcept -> GLuint
{
    return *textureHandle;
}

void glb::ArrayTexture::create(uvec2 size, size_t layers, GLenum format)
{
    textureHandle.release();
    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &textureHandle);
    glTextureStorage3D(*textureHandle, 1, format, size.x, size.y, static_cast<GLsizei>(layers));

    // Clear the texture to black. This prevents ugly artifacts for
    // textures with size smaller than the array texture size.
    std::array<GLuint, 4> pixel = { 0, 0, 0, 1 };
    glClearTexImage(
        *textureHandle,
        0,
        GL_RGBA, GL_UNSIGNED_BYTE,
        pixel.data()
    );

    glTextureParameteri(*textureHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(*textureHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(*textureHandle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(*textureHandle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    layerSize = size;
    numLayers = layers;
    internalFormat = format;
}



// ---------------- //
// Helper functions //
// ---------------- //

auto glb::makeNoiseTexture2D(size_t xDim, size_t yDim) -> std::unique_ptr<Texture>
{
    const size_t bufSize = xDim * yDim * 2; // *2 because the texture has two channels
	std::vector<GLubyte> texBuffer(bufSize);

	// Generate values and write them to the buffer
	std::random_device rn;
	std::mt19937 engine(rn());
	std::uniform_int_distribution<int> number(0, UCHAR_MAX);
	for (size_t i = 0; i < bufSize;)
	{
		texBuffer[i++] = static_cast<GLubyte> (number(engine));
		texBuffer[i++] = static_cast<GLubyte> (number(engine));
	}

	GLuint tex;
	glCreateTextures(GL_TEXTURE_2D, 1, &tex);

	glTextureParameterf(tex, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameterf(tex, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameterf(tex, GL_TEXTURE_MAG_FILTER, GL_NONE);
	glTextureParameterf(tex, GL_TEXTURE_MIN_FILTER, GL_NONE);

	glTextureStorage2D(tex, 1, GL_RG8, static_cast<GLsizei>(xDim), static_cast<GLsizei>(yDim));
	glTextureSubImage2D(
        tex, 0, 0, 0,
        static_cast<GLsizei>(xDim), static_cast<GLsizei>(yDim),
        GL_RG, GL_UNSIGNED_BYTE,
        texBuffer.data()
    );

	return std::make_unique<Texture>(tex);
}