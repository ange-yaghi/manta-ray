#ifndef MANTARAY_SESSION_H
#define MANTARAY_SESSION_H

#include <string>
#include <vector>
#include <mutex>

namespace manta {

    class Console;
    class ImagePlane;
    class VectorMap2D;
    class PreviewNode;

    class Session {
    public:
        Session();
        ~Session();

        static Session &get();

        void setConsole(Console *console) { m_console = console; }
        Console *getConsole() const { return m_console; }

        void registerPreview(PreviewNode *preview);
        void deregisterPreview(PreviewNode *preview);
        int getPreviewCount() const { return (int)m_previews.size(); }
        std::vector<PreviewNode *> getPreviews();

    protected:
        Console *m_console;

        std::mutex m_previewLock;
        std::vector<PreviewNode *> m_previews;
    };

} /* namespace manta */

#endif /* MANTARAY_SESSION_H */
