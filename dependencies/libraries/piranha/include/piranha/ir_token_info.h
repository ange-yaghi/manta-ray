#ifndef PIRANHA_IR_TOKEN_INFO_H
#define PIRANHA_IR_TOKEN_INFO_H

#include <ostream>
#include <string>

namespace piranha {

    struct IrTokenInfo {
        IrTokenInfo(int lineStart, int lineEnd, int colStart, int colEnd) :
            lineStart(lineStart), lineEnd(lineEnd), colStart(colStart), colEnd(colEnd) {
            valid = true;
            specified = true;
            empty = false;
        }

        IrTokenInfo() :
            lineStart(INT_MAX), lineEnd(-1), colStart(INT_MAX), colEnd(-1) {
            valid = true;
            specified = false;
            empty = true;
        }

        ~IrTokenInfo() {
            /* void */
        }

        int lineStart;
        int lineEnd;
        int colStart;
        int colEnd;
        bool valid;
        bool empty;
        bool specified;

        void combine(const IrTokenInfo *info) {
            valid = valid && info->valid;

            if (info->lineStart < lineStart) {
                lineStart = info->lineStart;
                colStart = info->colStart;
            }
            else if (info->lineStart == lineStart) {
                if (info->colStart < colStart) {
                    colStart = info->colStart;
                }
            }

            if (info->lineEnd > lineEnd) {
                lineEnd = info->lineEnd;
                colEnd = info->colEnd;
            }
            else if (info->lineEnd == lineEnd) {
                if (info->colEnd > colEnd) {
                    colEnd = info->colEnd;
                }
            }
        }

        friend std::ostream& operator<<(std::ostream& os, const IrTokenInfo& dt) {
            os << dt.colStart << " (L" << dt.lineStart << ")" 
                << " - " << dt.colEnd << " (L" << dt.lineEnd << ")" << std::endl;
            return os;
        }
    };

    template <typename T>
    struct T_IrTokenInfo : public IrTokenInfo {
        T_IrTokenInfo() {
            /* void */
        }

        T_IrTokenInfo(const T &data) : data(data) {
            /* void */
        }

        T_IrTokenInfo(const T &data, int lineStart, int lineEnd, int colStart, int colEnd)
            : data(data), IrTokenInfo(lineStart, lineEnd, colStart, colEnd) {
            /* void */
        }

        ~T_IrTokenInfo() {
            /* void */
        }

        T data;
    };

    template <typename T, int size>
    struct IrTokenInfoSet {
        IrTokenInfoSet() {
            /* void */
        }

        ~IrTokenInfoSet() {
            /* void */
        }

        T_IrTokenInfo<T> data[size];
    };

    typedef T_IrTokenInfo<int> IrTokenInfo_int;
    typedef T_IrTokenInfo<std::string> IrTokenInfo_string;
    typedef T_IrTokenInfo<double> IrTokenInfo_float;
    typedef T_IrTokenInfo<bool> IrTokenInfo_bool;

} /* namespace piranha */

#endif /* PIRANHA_IR_TOKEN_INFO_H */
