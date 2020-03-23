#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>
#include <string>
#include <cassert>

#include <GL/glew.h>
#include <glm/glm.hpp>
using namespace glm;

#include "OpenglResource.h"

namespace glb
{
    constexpr GLenum TEXTURE_DEFAULT_FORMAT{ GL_RGBA8 };

    class ArrayTexture;

    /**
     * @brief A texture object that holds an OpenGL texture.
     */
    class Texture
    {
    public:
        static constexpr vec4 UNINITIALIZED_COLOR{ 0.8, 0.3, 0.7, 1.0 };

        /**
         * @brief Intialize the texture to a default color with size 1x1
         */
        Texture();

        /**
         * @brief Initialize the texture to a single color
         *
         * Initializes the textures with an internal format of GL_RGBA8 of the
         * size 1x1.
         *
         * @param vec4 color The color, values in interval I=[0, 1], I€R.
         */
        explicit Texture(vec4 color);

        /**
         * @brief Initialize the texture with a specific size and format
         *
         * It is advised to make the size a power-of-two value. OpenGL might
         * throw an error if you don't.
         *
         * The color is copied into the texture as GL_RGBA with a channel size of
         * GL_UNSIGNED_BYTE, regardless of the specified internal format.
         *
         * @param uvec2  size           Size of the texture in pixels
         * @param GLenum internalFormat The texture's internal format
         * @param vec4   color          Initial texture color
         */
        explicit Texture(uvec2 size, GLenum internalFormat = GL_RGBA8, vec4 color = UNINITIALIZED_COLOR);

        /**
         * @brief Load an image file into the texture
         *
         * @param std::string imagePath Path to the image
         */
        explicit Texture(const std::string& imagePath);

        /**
         * @brief Create the texture from an existing OpenGL texture handle.
         *
         * Use with care.
         *
         * The handle must point to a valid OpenGL object. Passing an invalid
         * handle will throw an exception. Manually deleting the handle after
         * the texture has been created results in undefined behavious, most
         * likely an OpenGL memory error or something similarly terrifying.
         *
         * @param GLuint texHandle The texture
         *
         * @throw std::runtime_error Thrown if the passed handle is not a
         *                           texture according to glIsTexture().
         */
        explicit Texture(GLuint texHandle);

        /**
         * @brief Create a texture from an array texture layer
         *
         * Creates a texture with size and internal format of the specified
         * array texture. Copies pixel data from a specific layer of the array
         * texture.
         *
         * @param ArrayTexture& src   Source texture
         * @param size_t        layer Layer in the source texture
         */
        Texture(const ArrayTexture& src, size_t layer);

        /**
         * @return The internal OpenGL texture handle
         */
        auto operator*() const noexcept -> GLuint;

        /**
         * @brief Create a new texture and load an image from a file into it
         *
         * Initializes the texture to the default color if the specified path does
         * not exist.
         *
         * @param const std::string& imagePath Path to the image
         *
         * @throw std::runtime_error if loading fails
         */
        void loadImage(const std::string& imagePath);

        /**
         * @brief Create a new texture with size 1x1 and a single color
         *
         * @param vec4 color The color, values in interval I=[0, 1], I€R.
         */
        void loadColor(vec4 color);

        /**
         * @brief Load raw pixel data into the texture
         *
         * @param void* data The data
         */
        void copyRawData(
            void* data,
            uvec2 size,
            uvec2 dstOffset = uvec2{ 0u, 0u },
            GLenum srcFormat = GL_RGBA,
            GLenum srcType = GL_UNSIGNED_BYTE);

        /**
         * @brief Load the texture to the buffer bound to GL_PIXEL_PACK_BUFFER
         *
         * Data is unpacked in the format GL_RGBA with a size of GL_UNSIGNED_BYTE.
         */
        void pack() const;

        /**
         * @brief Load data from the buffer bound to GL_PIXEL_UNPACK_BUFFER
         *
         * Data is interpreted as the format GL_RGBA with a size of GL_UNSIGNED_BYTE
         *
         * @param uvec2  dstOffset    Offset into the texture where the data will
         *                            be placed
         * @param uvec2  copySize     Size of the data to be copied. This defines a
         *                            rectangle of the texture that will be replaced
         *                            together with dstOffset
         * @param size_t bufferOffset Offset into the unpack buffer
         */
        void unpack(uvec2 dstOffset, uvec2 copySize, size_t bufferOffset) const;

        /**
         * @brief Bind the texture to an OpenGL texture unit.
         *
         * @param unsinged int unit The OpenGL texture unit that the texture
         *                          will be bound to. Limited by the graphics card's
         *                          texture unit limit.
         */
        void bind(unsigned int unit) const;

        /**
         * @return ivec2 The texture's size in texels
         */
        [[nodiscard]]
        auto getSize() const noexcept -> uvec2;

        /**
         * @return GLenum The texture's internal format, e.g. GL_RGBA8 or GL_R32F
         */
        [[nodiscard]]
        auto getInternalFormat() const noexcept -> GLenum;

        /**
         * @return GLuint The OpenGL resource handle
         */
        [[nodiscard]]
        auto getTextureHandle() const noexcept -> GLuint;

    private:
        /** Create a new texture, set size and internal format */
        void create(uvec2 size, GLenum internalFormat);

        uvec2 size{ 0, 0 };
        GLenum internalFormat{ 0 };
        glSharedTexture textureHandle;
    };


    /**
     * @brief An OpenGL array texture
     */
    class ArrayTexture
    {
    public:
        /**
         * @brief Create an array texture with a specific size and number of layers
         *
         * @param uvec2  size   The size of each array layer
         * @param size_t layers The number of array layers
         */
        explicit ArrayTexture(uvec2 size, size_t layers, GLenum format = TEXTURE_DEFAULT_FORMAT);

        /**
         * @brief Load an array of images as an array texture
         *
         * Uses the size of the largest image as the size of every layer. This
         * means that images with varying sizes will have black borders to fit the
         * size of larger images in the array.
         *
         * Initializes the texture with an internal format of GL_RGBA8.
         *
         * @param std::vector<std::string> iamgePaths Paths to the images to load.
         */
        explicit ArrayTexture(const std::vector<std::string>& imagePaths);

        /**
         * @brief Load an array of textures as an array texture
         *
         * Uses the size of the largest texture as the size of every layer. This
         * means that textures with varying sizes will have black borders to fit the
         * size of larger textures in the array.
         *
         * The data of specified textures is copied. This means that the array
         * texture is unaffected if one of the textures is destroyed.
         *
         * @param std::vector<Texture> textures The texture to load into the array
         *                                      texture
         */
        explicit ArrayTexture(const std::vector<Texture>& textures, GLenum format = TEXTURE_DEFAULT_FORMAT);

        /**
         * @brief Create an array of monochrome textures
         *
         * Initializes the texture with an internal format of GL_RGBA8.
         *
         * @param std::vector<vec4> colors An array of color that will each be
         *                                 loaded as an array texture layer
         */
        explicit ArrayTexture(std::vector<vec4> colors);

        /**
         * @return The internal OpenGL texture handle
         */
        auto operator*() const noexcept -> GLuint;

        /**
         * @brief Bind the array texture to a texture unit
         *
         * The texture can be accessed in the shader through a sampler2DArray.
         *
         * @param GLsizei unit The texture unit to bind the array texture to
         */
        void bind(GLsizei unit) const;

        /**
         * @brief Load an image file into a texture layer
         *
         * The loaded image will be cropped to fit the array texture size if it is
         * larger.
         *
         * Initializes the texture with an internal format of GL_RGBA8.
         *
         * @param const std::string& imagePath Path to the image file
         * @param size_t             layer     The layer to load the image into
         */
        void loadImage(const std::string& imagePath, size_t layer);

        /**
         * @brief Copy a texture into a texture layer
         *
         * The internal formats of the source and destination textures must be
         * compatible as specified in the official OpenGL specification for
         * glCopyImageSubData.
         *
         * @param Texture src       The source texture. Data is copied from this
         *                          texture
         * @param uvec2   size      The size of the data to copy. Cannot be more
         *                          than the size of src
         * @param uvec2   dstOffset An offset into the texture layer where the data
         *                          will be copied
         * @param size_t  layer     The layer to copy the data to
         */
        void copyImage(
            const Texture& src,
            uvec2 size,
            uvec2 dstOffset,
            size_t layer);

        /**
         * @brief Copy raw data into a texture layer
         *
         * @param void*  src            The source data. Data is copied from
         *                              this texture
         * @param uvec2  size           The size of the data to copy. Cannot be
         *                              more than the size of src
         * @param uvec2  dstOffset      An offset into the texture layer where
         *                              the data will be copied
         * @param size_t layer          The layer to copy the data to
         * @param GLenum externalFormat Format of the input data, external format
         * @param GLenum srcType        Size and signedness per channel
         */
        void copyRawData(
            void* buf,
            uvec2 size,
            uvec2 dstOffset,
            size_t layer,
            GLenum externalFormat = GL_RGBA,
            GLenum srcType = GL_UNSIGNED_BYTE);

        /**
         * @brief Get a layer as a single texture
         *
         * Creates a texture and copies a layer to the new texture.
         *
         * @param size_t layer The layer to extract
         */
        auto extractLayer(size_t layer) -> Texture;

        /**
         * @return uvec2 Size of the array texture layers
         *
         * All layers have the same size
         */
        [[nodiscard]]
        auto getSize() const noexcept -> uvec2;

        /**
         * @return size_t The number of array layers in the array texture
         */
        [[nodiscard]]
        auto getNumLayers() const noexcept -> size_t;

        /**
         * @return The texture's internal format
         */
        [[nodiscard]]
        auto getInternalFormat() const noexcept -> GLenum;

        /**
         * It is not recommended to use this. Use methods of the texture class to
         * interact with the texture.
         *
         * @return GLuint The OpenGL handle to the array texture
         */
        [[nodiscard]]
        auto getTextureHandle() const noexcept -> GLuint;

    private:
        void create(uvec2 size, size_t layers, GLenum format);

        uvec2 layerSize{ 0u };
        size_t numLayers{ 0 };
        GLenum internalFormat{ 0 };
        glSharedTexture textureHandle;
    };


    /* Generate a 2D noise image. */
    auto makeNoiseTexture2D(size_t xDim, size_t yDim) -> std::unique_ptr<Texture>;

} // namespace glb

#endif