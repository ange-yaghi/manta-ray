#ifndef MANTARAY_SESSION_H
#define MANTARAY_SESSION_H

#include <string>
#include <vector>
#include <mutex>
#include <map>

namespace manta {

    class Console;
    class ImagePlane;
    class VectorMap2D;
    class PreviewNode;
    class KDTree;
    class Mesh;

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

        KDTree *getCachedKdTree(const std::string &key);
        void putCachedKdTree(const std::string &key, KDTree *tree);

        Mesh *getCachedMesh(const std::string &key);
        void putCachedMesh(const std::string &key, Mesh *mesh);

    protected:
        Console *m_console;

        std::mutex m_previewLock;
        std::vector<PreviewNode *> m_previews;
        std::map<std::string, KDTree *> m_kdTreeCache;
        std::map<std::string, Mesh *> m_meshCache;
    };

} /* namespace manta */

#endif /* MANTARAY_SESSION_H */
