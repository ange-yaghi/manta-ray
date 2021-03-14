#ifndef MANTARAY_UI_PREVIEW_H
#define MANTARAY_UI_PREVIEW_H

#include "../../include/preview_node.h"

#include "delta.h"

#include <mutex>

namespace mantaray_ui {

    class Preview {
    public:
        struct PreviewSnapshot {
            ysTexture *texture;
            int index;
            bool outdated = false;
        };

    public:
        Preview();
        ~Preview();

        void initialize(dbasic::DeltaEngine *engine, int index);

        ysTexture *getCurrentPreview();

        void setPreviewNode(manta::PreviewNode *node) { m_previewNode = node; }
        manta::PreviewNode *getPreviewNode() const { return m_previewNode; }

        void update();
        void clean();

        void pan(const ysVector &pan) { m_pan = ysMath::Add(m_pan, pan); }
        void resetPan() { m_pan = ysMath::Constants::Zero; }
        ysVector getPan() const { return m_pan; }

        void zoom(int zoom) { m_zoom += zoom; }
        void resetZoom() { m_zoom = 0; }
        int getZoom() const { return m_zoom; }

        std::string getTitle() const { return m_title; }

        bool isUpdating() const { return m_updateQueue > 0; }

        int getIndex() const { return m_index; }

    protected:
        ysTexture *createTexture(const manta::VectorMap2D *vectorMap);
        void updateThread(int index);

        manta::PreviewNode *m_previewNode;
        int m_updateIndex;

        std::mutex m_textureLock;
        std::vector<PreviewSnapshot> m_previewSnapshots;

        dbasic::DeltaEngine *m_engine;

        std::atomic<int> m_updateQueue;

        std::string m_title;

        int m_zoom;
        ysVector m_pan;

        int m_index;
    };

} /* namespace mantaray_ui */

#endif /* MANTARAY_UI_PREVIEW_H */
