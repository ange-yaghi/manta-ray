#ifndef MANTARAY_SESSION_H
#define MANTARAY_SESSION_H

#include <string>
#include <vector>
#include <mutex>

namespace manta {

    class Console;
    class ImagePlane;
    class VectorMap2D;

    struct ImagePreview {
        std::string name = "";
        VectorMap2D *map = nullptr;
        std::atomic_bool finalized = false;
    };

    struct ImagePreviewContainer {
        std::string name;
        VectorMap2D *map;
        bool finalized;
    };

    class Session {
    public:
        Session();
        ~Session();

        static Session &get();

        void setConsole(Console *console) { m_console = console; }
        Console *getConsole() const { return m_console; }

        ImagePreview *createImagePreview(const std::string &name);
        void destroyImagePreview(ImagePreview *preview);
        void attachTargetToImagePreview(ImagePreview *preview, VectorMap2D *target);
        void detachTargetFromImagePreview(ImagePreview *preview);
        void clearImagePreviews();
        void getImagePreviews(std::vector<ImagePreviewContainer> &target);

    protected:
        Console *m_console;

        std::mutex m_imagePreviewLock;
        std::vector<ImagePreview *> m_imagePreviews;
    };

} /* namespace manta */

#endif /* MANTARAY_SESSION_H */
