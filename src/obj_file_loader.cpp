#include "../include/obj_file_loader.h"

#include "../include/standard_allocator.h"

#include <fstream>
#include <sstream>
#include <cctype>
#include <functional>
#include <string>

// Taken from https://www.oreilly.com/library/view/c-cookbook/0596007612/ch04s07.html
template<typename T>
void split(const std::basic_string<T>& s, T c, std::vector<std::basic_string<T> >& v) {
    typename std::basic_string<T>::size_type i = 0;
    typename std::basic_string<T>::size_type j = s.find(c);

    if (j == std::basic_string<T>::npos) {
        v.push_back(std::string(s));
        return;
    }

    while (j != std::basic_string<T>::npos) {
        v.push_back(s.substr(i, j - i));
        i = ++j;
        j = s.find(c, j);

        if (j == std::basic_string<T>::npos)
            v.push_back(s.substr(i, s.length()));
    }
}

bool isWhitespace(const std::string &s) {
    for (int i = 0; i < s.length(); i++) {
        if (!std::isspace(s[i])) {
            return false;
        }
    }
    return true;
}

manta::ObjFileLoader::ObjFileLoader() {
    m_currentLine = 0;
    m_currentMaterial = -1;
}

manta::ObjFileLoader::~ObjFileLoader() {
    // TODO: check that object is actually destroyed
}

bool manta::ObjFileLoader::loadObjFile(const char *fname) {
    std::fstream file(fname, std::ios::in);

    if (file.is_open()) {
        const bool result = loadObjFile(file);
        file.close();
        return result;
    }
    else {
        // The file could not be loaded
        return false;
    }
}

bool manta::ObjFileLoader::loadObjFile(std::istream &stream) {
#define CHECK_INDEX(index) if ((index) == -1) { delete[] buffer; return false; } 

    m_currentLine = 0;
    m_currentMaterial = -1;

    size_t bufferSize = 4;
    char *buffer = new char[bufferSize];

    std::string line;
    
    while (stream.peek() != -1) {
        ++m_currentLine;

        /*
        for (int i = 0;; ++i) {
            if (i >= bufferSize) {
                char *oldBuffer = buffer;
                buffer = new char[(size_t)bufferSize * 2];

                memcpy(buffer, oldBuffer, (size_t)bufferSize * sizeof(char));

                delete[] oldBuffer;
                bufferSize *= 2;
            }

            const char c = fgetc(fp);
            if (c == '\n' || feof(fp)) {
                buffer[i] = '\0';
                break;
            }
            buffer[i] = c;
        }*/

        bufferSize = getLine(stream, &buffer, bufferSize);

        /*
        if (line.size() >= bufferSize) {
            delete[] buffer;

            bufferSize = (int)line.size() + 1;
            buffer = new char[(size_t)bufferSize];
        }

        strcpy_s(buffer, bufferSize, line.c_str());*/

        int index = readWhitespace(buffer, 0);
        CHECK_INDEX(index);

        int e0 = index, e1;
        index = readType(buffer, index);
        CHECK_INDEX(index);
        e1 = index;

        // Skip this line if it's a comment
        if (buffer[e0] == '#') continue;
        else if (clipCompare(buffer, "v", e0, e1)) {
            // Create a new vertex
            math::Vector3 newVertex;
            index = readVector3(buffer, &newVertex, index);
            CHECK_INDEX(index);
            m_vertices.push_back(newVertex);
        }
        else if (clipCompare(buffer, "f", e0, e1)) {
            // Read a face
            ObjFace newFace;
            newFace.material = m_currentMaterial;
            index = readFace(buffer, &newFace, index);
            CHECK_INDEX(index);
            m_faces.push_back(newFace);
        }
        else if (clipCompare(buffer, "vt", e0, e1)) {
            // Read a texture coordinate
            math::Vector2 texCoords;
            index = readVector2(buffer, &texCoords, index);
            CHECK_INDEX(index);
            m_texCoords.push_back(texCoords);
        }
        else if (clipCompare(buffer, "vn", e0, e1)) {
            // Read a normal value
            math::Vector3 normal;
            index = readVector3(buffer, &normal, index);
            CHECK_INDEX(index);
            m_normals.push_back(normal);
        }
        else if (clipCompare(buffer, "usemtl", e0, e1)) {
            // Use a new material
            index = readMaterial(buffer, index);
            CHECK_INDEX(index);
        }
    }

    delete[] buffer;

    return true;
}

/*
bool manta::ObjFileLoader::readVertexPosition(std::stringstream &s, math::Vector3 *vertex) const {
    s >> vertex->x;
    if (s.fail()) {
        return false;
    }

    s >> vertex->y;
    if (s.fail()) {
        return false;
    }

    s >> vertex->z;
    if (s.fail()) {
        return false;
    }

    if (!s.eof()) {
        return false;
    }
    else {
        return true;
    }
}

bool manta::ObjFileLoader::readVertexNormal(std::stringstream &s, math::Vector3 *vertex) const {
    s >> vertex->x;
    if (s.fail()) {
        return false;
    }

    s >> vertex->y;
    if (s.fail()) {
        return false;
    }

    s >> vertex->z;
    if (s.fail()) {
        return false;
    }

    if (!s.eof()) {
        return false;
    }
    else {
        return true;
    }
}

bool manta::ObjFileLoader::readVertexTextureCoords(std::stringstream &s, math::Vector2 *texCoords) const {
    s >> texCoords->x;
    if (s.fail()) {
        return false;
    }

    s >> texCoords->y;
    if (s.fail()) {
        return false;
    }

    if (!s.eof()) {
        return false;
    }
    else {
        return true;
    }
}

bool manta::ObjFileLoader::readFace(std::stringstream &s, ObjFace *face) {
    size_t nParameters = 0;
    int vertexIndex = 0;

    // Initialize all indices
    // NOTE: 0 corresponds to -1 since the indices will be transformed later
    for (int i = 0; i < 3; i++) {
        face->vt[i] = 0;
        face->vn[i] = 0;
        face->v[i] = 0;
    }

    while (!s.eof()) {
        std::string group;
        s >> group;

        std::vector<std::string> elements;
        split(group, '/', elements);

        if (nParameters == 0) nParameters = elements.size();
        else {
            if (nParameters != elements.size()) {
                return false;
            }
        }

        if (nParameters >= 1) {        
            std::stringstream ss = std::stringstream(elements[0]);
            ss >> face->v[vertexIndex];
            if (ss.fail()) {
                return false;
            }
        }
        if (nParameters >= 2) {
            if (!isWhitespace(elements[1])) {
                std::stringstream ss = std::stringstream(elements[1]);
                ss >> face->vt[vertexIndex];
                if (ss.fail()) {
                    return false;
                }
            }
        }
        if (nParameters == 3) {
            if (!isWhitespace(elements[2])) {
                std::stringstream ss = std::stringstream(elements[2]);
                ss >> face->vn[vertexIndex];
                if (ss.fail()) {
                    return false;
                }
            }
        }
        if (nParameters > 3) { return false; }

        vertexIndex++;
    }

    return true;
}

bool manta::ObjFileLoader::readMaterial(std::stringstream &s) {
    std::string materialName;
    s >> materialName;

    if (s.fail()) {
        return false;
    }

    if (materialName == "None") {
        m_currentMaterial = -1;
    }
    else {
        ObjMaterial newMaterial;
        newMaterial.name = materialName;
        m_materials.push_back(newMaterial);
        m_currentMaterial = m_materials.size() - 1;
    }
    
    return true;
}
*/

/*
void manta::ObjFileLoader::getLine(std::istream &stream, std::string &target) {
    target.clear();

    std::istream::sentry se(stream, true);
    std::streambuf *sb = stream.rdbuf();

    if (se) {
        for (;;) {
            const int c = sb->sbumpc();
            switch (c) {
            case '\n':
                return;
            case '\r':
                if (sb->sgetc() == '\n') sb->sbumpc();
                return;
            case EOF:
                if (target.empty()) stream.setstate(std::ios::eofbit);
                return;
            default:
                target += static_cast<char>(c);
            }
        }
    }
}*/

size_t manta::ObjFileLoader::getLine(std::istream &stream, char **buffer, size_t bufferSize) {
    std::istream::sentry se(stream, true);
    std::streambuf *sb = stream.rdbuf();

    char *&target = *buffer;

    if (se) {
        for (int i = 0;; ++i) {
            if (i == bufferSize - 2) {
                char *newBuffer = new char[bufferSize * 2];
                memcpy(newBuffer, target, bufferSize);
                delete[] target;

                target = newBuffer;
                bufferSize *= 2;
            }

            const int c = sb->sbumpc();
            switch (c) {
            case '\n':
                target[i] = 0;
                return bufferSize;
            case '\r':
                target[i] = 0;
                if (sb->sgetc() == '\n') sb->sbumpc();
                return bufferSize;
            case EOF:
                target[i] = 0;
                if (i == 0) stream.setstate(std::ios::eofbit);
                return bufferSize;
            default:
                (*buffer)[i] = static_cast<char>(c);
            }
        }
    }
    else {
        return bufferSize;
    }
}

int manta::ObjFileLoader::readType(char *s, int index) {
    index = readWhitespace(s, index);
    if (index == -1) return -1;

    index = readUntilWhitespace(s, index);
    if (index == -1) return -1;

    return index;
}

int manta::ObjFileLoader::readMaterial(char *s, int index) {
    index = readWhitespace(s, index);
    if (index == -1) return -1;

    int e0 = index, e1;
    index = readUntilWhitespace(s, index);
    if (index == -1) return -1;
    e1 = index;

    if (clipCompare(s, "None", e0, e1)) {
        m_currentMaterial = -1;
    }
    else {
        ObjMaterial newMaterial;
        newMaterial.name = clip(s, e0, e1);
        m_materials.push_back(newMaterial);
        m_currentMaterial = (int)m_materials.size() - 1;
    }

    return index;
}

int manta::ObjFileLoader::readWhitespace(char *s, int index) {
    int i = index;
    for (; s[i] != 0; ++i) {
        if (!isWhitespace(s[i])) {
            return i;
        }
    }

    return i;
}

int manta::ObjFileLoader::readUntil(char *s, char terminator, int index) {
    for (int i = index; s[i] != 0; ++i) {
        if (s[i] == terminator) {
            return i;
        }
    }

    return -1;
}

int manta::ObjFileLoader::readUntilWhitespace(char *s, int index, bool includeEnd) {
    int i = index;
    for (; s[i] != 0; ++i) {
        if (isWhitespace(s[i])) {
            return i;
        }
    }

    return includeEnd
        ? i
        : -1;
}

int manta::ObjFileLoader::readVector3(char *s, math::Vector3 *target, int index) {
    int e0, e1;

    index = readWhitespace(s, index);
    if (index == -1) return -1;

    for (int i = 0; i < 3; ++i) {
        e0 = index;
        index = readUntilWhitespace(s, index);
        if (index == -1) return -1;
        e1 = index;

        if (!parseReal(s, &target->vec[i], e0, e1)) return -1;
        index = readWhitespace(s, index);
        if (index == -1) return -1;
    }

    return index;
}

int manta::ObjFileLoader::readVector2(char *s, math::Vector2 *target, int index) {
    int e0, e1;

    index = readWhitespace(s, index);
    if (index == -1) return -1;

    for (int i = 0; i < 2; ++i) {
        e0 = index;
        index = readUntilWhitespace(s, index);
        if (index == -1) return -1;
        e1 = index;

        if (!parseReal(s, &target->vec[i], e0, e1)) return -1;
        index = readWhitespace(s, index);
        if (index == -1) return -1;
    }

    return index;
}

int manta::ObjFileLoader::readFace(char *s, ObjFace *target, int index) {
    for (int i = 0; i < 3; ++i) {
        int v[3] = {-1, -1, -1};
        int n;
        index = readSlashSeparatedSet(s, v, &n, index);
        if (index == -1) return -1;

        target->v[i] = v[0];
        target->vt[i] = v[1];
        target->vn[i] = v[2];
    }

    return index;
}

int manta::ObjFileLoader::readSlashSeparatedSet(char *s, int *target, int *n, int index) {
    int e0, e1;

    index = readWhitespace(s, index);
    if (index == -1) return -1;

    *n = 0;

    for (int i = 0;; ++i) {
        e0 = index;
        const int s0 = readUntil(s, '/', index);
        const int s1 = readUntilWhitespace(s, index);
        
        bool done = false;
        if (s0 == -1 || s0 > s1) {
            done = true;
            index = s1;
        }
        else {
            index = s0;
        }

        if (index == -1) return -1;
        e1 = index;

        ++(*n);
        if (!parseInt(s, &target[i], e0, e1)) return -1;

        // Read '/'
        if (!done) ++index;
        else break;

        index = readWhitespace(s, index);
        if (index == -1) return -1;
    }

    return index;
}

bool manta::ObjFileLoader::isWhitespace(char c) {
    switch (c) {
    case ' ':
    case '\n':
    case '\r':
    case '\t':
        return true;
    default:
        return false;
    }
}

bool manta::ObjFileLoader::clipCompare(char *s, const char *ref, int start, int end) {
    const char prev = s[end];
    s[end] = '\0';
    const bool eq = (strcmp(s, ref) == 0);
    s[end] = prev;

    return eq;
}

std::string manta::ObjFileLoader::clip(char *s, int start, int end) {
    const char prev = s[end];
    s[end] = '\0';
    const std::string result = std::string(&s[start]);
    s[end] = prev;

    return result;
}

bool manta::ObjFileLoader::parseReal(char *s, math::real *target, int start, int end) {
    const char prev = s[end];
    s[end] = '\0';

    char *e;
    double result = std::strtod(s + start, &e);
    char last = *e;

    s[end] = prev;
    *target = (math::real)result;

    if (last != '\0') {
        return false;
    }
    else {
        return true;
    }
}

bool manta::ObjFileLoader::parseInt(char *s, int *target, int start, int end) {
    const char prev = s[end];
    s[end] = '\0';

    char *e;
    long result = std::strtol(s + start, &e, 10);
    char last = *e;

    s[end] = prev;
    *target = (int)result;

    if (last != '\0') {
        return false;
    }
    else {
        return true;
    }
}

void manta::ObjFileLoader::destroy() {
    /* void */
}
