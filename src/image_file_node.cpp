#include "../include/image_file_node.h"

#include "../include/rgb_space.h"
#include "../include/standard_allocator.h"
#include "../include/vector_map_2d.h"
#include "../include/path.h"

#include <SDL_image.h>
#include <assert.h>

manta::ImageFileNode::ImageFileNode() {
    m_filename = "";
    m_correctGamma = true;

    m_correctGammaInput = nullptr;
    m_filenameInput = nullptr;
}

manta::ImageFileNode::~ImageFileNode() {
    /* void */
}

void manta::ImageFileNode::_initialize() {
    m_output.initialize();
}

void manta::ImageFileNode::_evaluate() {
    if (m_correctGammaInput != nullptr) {
        m_correctGammaInput->fullCompute((void *)&m_correctGamma);
    }

    if (m_filenameInput != nullptr) {
        std::string rawFilename;
        static_cast<piranha::NodeOutput *>(m_filenameInput)->fullCompute((void *)&rawFilename);

        Path finalPath;
        const Path rawFilePath(rawFilename);
        resolvePath(&rawFilePath, &finalPath);

        m_filename = finalPath.toString();
    }

    SDL_Surface *image;
    image = IMG_Load(m_filename.c_str());

    if (image == nullptr) {
        throwError("Image: " + m_filename + " could not be opened or was not found");
        return;
    }

    // Create a temporary pixel buffer
    Pixel **pixelData = StandardAllocator::Global()->allocate<Pixel *>(image->h);
    for (int i = 0; i < image->h; i++) {
        pixelData[i] = StandardAllocator::Global()->allocate<Pixel>(image->w);;
    }

    for (int i = 0; i < image->w; i++) {
        for (int j = 0; j < image->h; j++) {
            getPixel(image, i, j, &pixelData[j][i]);
        }
    }

    m_imageMap.initialize(image->w, image->h);

    for (int i = 0; i < image->w; i++) {
        for (int j = 0; j < image->h; j++) {
            Pixel &pixel = pixelData[j][i];

            math::real r = pixel.r / (math::real)255.0;
            math::real g = pixel.g / (math::real)255.0;
            math::real b = pixel.b / (math::real)255.0;

            if (m_correctGamma) {
                // Default to SRGB
                // TODO: make gamma correction generic
                r = (math::real)RgbSpace::srgb.inverseGammaSrgb(r);
                g = (math::real)RgbSpace::srgb.inverseGammaSrgb(g);
                b = (math::real)RgbSpace::srgb.inverseGammaSrgb(b);
            }

            m_imageMap.set(math::loadVector(r, g, b), i, j);
        }
    }

    // Free pixel data
    for (int i = 0; i < image->h; i++) {
        StandardAllocator::Global()->free(pixelData[i], image->w);
    }
    StandardAllocator::Global()->free(pixelData, image->h);

    SDL_FreeSurface(image);

    m_output.setMap(&m_imageMap);
}

void manta::ImageFileNode::_destroy() {
    m_imageMap.destroy();
}

void manta::ImageFileNode::registerInputs() {
    registerInput(&m_correctGammaInput, "correct_gamma");
    registerInput(&m_filenameInput, "filename");
}

void manta::ImageFileNode::registerOutputs() {
    setPrimaryOutput("__out");
    registerOutput(&m_output, "__out");
}

void manta::ImageFileNode::getPixel(const SDL_Surface *surface, int x, int y, Pixel *pixelOut) {
    Uint8 *pixel = (Uint8 *)surface->pixels;
    pixel += (y * (size_t)surface->pitch) + (x * sizeof(Uint8) * surface->format->BytesPerPixel);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    pixelOut->r = pixel[0];
    pixelOut->g = pixel[1];
    pixelOut->b = pixel[2];
#else
    pixelOut->r = pixel[0];
    pixelOut->g = pixel[1];
    pixelOut->b = pixel[2];
#endif /* SDL_BYTEORDER == SDL_BIG_ENDIAN */
}
