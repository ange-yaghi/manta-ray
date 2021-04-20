#ifndef MANTARAY_OBJ_FILE_LOADER_H
#define MANTARAY_OBJ_FILE_LOADER_H

#include "manta_math.h"

#include <istream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>

namespace manta {

    class ObjFileLoader {
    public:
        struct ObjMaterial {
            std::string name;
        };

        struct ObjFace {
            union {
                struct {
                    int v1, v2, v3;
                    int vn1, vn2, vn3;
                    int vt1, vt2, vt3;
                };
                struct {
                    int v[3];
                    int vn[3];
                    int vt[3];
                };
            };
            int material;
        };

    public:
        ObjFileLoader();
        ~ObjFileLoader();

        bool loadObjFile(const char *fname);

        unsigned int getVertexCount() const { return (unsigned int)m_vertices.size(); }
        unsigned int getFaceCount() const { return (unsigned int)m_faces.size(); }
        unsigned int getNormalCount() const { return (unsigned int)m_normals.size(); }
        unsigned int getTexCoordCount() const { return (unsigned int)m_texCoords.size(); }

        ObjFace getFace(unsigned int i) const { return m_faces[i]; }
        math::Vector3 getVertex(unsigned int i) const { return m_vertices[i]; }
        math::Vector3 getNormal(unsigned int i) const { return m_normals[i]; }
        math::Vector2 getTexCoords(unsigned int i) const { return m_texCoords[i]; }
        ObjMaterial getMaterial(int i) const { return m_materials[i]; }

        void destroy();

    protected:
        bool loadObjFile(std::istream &stream);
        //bool readVertexPosition(std::stringstream &s, math::Vector3 *vertex) const;
        //bool readVertexNormal(std::stringstream &s, math::Vector3 *normal) const;
        //bool readVertexTextureCoords(std::stringstream &s, math::Vector2 *texCoords) const;
        //bool readFace(std::stringstream &s, ObjFace *face);
        //bool readMaterial(std::stringstream &s);

        static size_t getLine(std::istream &stream, char **buffer, size_t bufferSize);
        static int readType(char *s, int index);
        int readMaterial(char *s, int index);
        static int readWhitespace(char *s, int index);
        static int readUntil(char *s, char terminator, int index);
        static int readUntilWhitespace(char *s, int index, bool includeEnd=true);
        static int readVector3(char *s, math::Vector3 *target, int index);
        static int readVector2(char *s, math::Vector2 *target, int index);
        static int readFace(char *s, ObjFace *target, int index);
        static int readSlashSeparatedSet(char *s, int *target, int *n, int index);
        static bool isWhitespace(char c);
        static bool clipCompare(char *s, const char *ref, int start, int end);
        static std::string clip(char *s, int start, int end);
        static bool parseReal(char *s, math::real *target, int start, int end);
        static bool parseInt(char *s, int *target, int start, int end);

    protected:
        std::vector<ObjFace> m_faces;
        std::vector<math::Vector3> m_vertices;
        std::vector<math::Vector2> m_texCoords;
        std::vector<math::Vector3> m_normals;
        std::vector<ObjMaterial> m_materials;

        unsigned int m_currentLine;
        int m_currentMaterial;
    };

} /* namespace manta */

#endif /* MANTARAY_OBJ_FILE_LOADER_H */
