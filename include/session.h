#ifndef MANTARAY_SESSION_H
#define MANTARAY_SESSION_H

#include <string>
#include <vector>

namespace manta {

    class Console;
    class ImagePlane;
    class VectorMap2D;

    struct ImagePlanePreview {
        std::string name;
        VectorMap2D *map;
    };

    struct ImagePreview {
        std::string name;
        VectorMap2D *map;
    };

    class Session {
    public:
        Session();
        ~Session();

        static Session &get();

        void setConsole(Console *console) { m_console = console; }
        Console *getConsole() const { return m_console; }

        ImagePlanePreview *createImagePlanePreview(const std::string &name);
        int getImagePlanePreviewCount() const { return (int)m_imagePlanePreviews.size(); }
        ImagePlanePreview *getImagePlanePreview(int index) { return m_imagePlanePreviews[index]; }

    protected:
        Console *m_console;

        std::vector<ImagePlanePreview *> m_imagePlanePreviews;
    };

} /* namespace manta */

#endif /* MANTARAY_SESSION_H */
