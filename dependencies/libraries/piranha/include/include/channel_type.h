#ifndef PIRANHA_CHANNEL_TYPE_H
#define PIRANHA_CHANNEL_TYPE_H

#include <cstring>

namespace piranha {

    class ChannelType {
    public:
        ChannelType();
        ChannelType(const char *type, const ChannelType *parent = nullptr);
        ~ChannelType();

        void initialize(const char *type, const ChannelType *parent);

    public:
        const char *getType() const { return m_typeString; }
        bool isCompatibleWith(const ChannelType &t) const;
        bool isEqual(const ChannelType &t) const;

    private:
        static int generateHash(const char *string);
        int getHash() const { return m_hash; }

    private:
        const ChannelType *m_parent;
        int m_hash;
        const char *m_typeString;
    };

} /* namespace piranha */

#endif /* PIRANHA_CHANNEL_TYPE_H */
