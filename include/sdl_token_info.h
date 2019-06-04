#ifndef SDL_TOKEN_INFO_H
#define SDL_TOKEN_INFO_H

#include <ostream>
#include <string>

namespace manta {

	struct SdlTokenInfo {
		SdlTokenInfo(int lineStart, int lineEnd, int colStart, int colEnd) :
			lineStart(lineStart), lineEnd(lineEnd), colStart(colStart), colEnd(colEnd) {
			valid = true;
		}
		SdlTokenInfo() :
			lineStart(INT_MAX), lineEnd(-1), colStart(INT_MAX), colEnd(-1) {
			valid = true;
		}
		~SdlTokenInfo() {
			/* void */
		}

		int lineStart;
		int lineEnd;
		int colStart;
		int colEnd;
		bool valid;

		void combine(const SdlTokenInfo *info) {
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

		friend std::ostream& operator<<(std::ostream& os, const SdlTokenInfo& dt) {
			os << dt.colStart << " (L" << dt.lineStart << ")" 
				<< " - " << dt.colEnd << " (L" << dt.lineEnd << ")" << std::endl;
			return os;
		}
	};

	template <typename T>
	struct T_SdlTokenInfo : public SdlTokenInfo {
		T_SdlTokenInfo() {
			/* void */
		}
		T_SdlTokenInfo(const T &data) : data(data) {
			/* void */
		}
		T_SdlTokenInfo(const T &data, int lineStart, int lineEnd, int colStart, int colEnd)
			: data(data), SdlTokenInfo(lineStart, lineEnd, colStart, colEnd) {
			/* void */
		}
		~T_SdlTokenInfo() {
			/* void */
		}

		T data;
	};

	typedef T_SdlTokenInfo<int> SdlTokenInfo_int;
	typedef T_SdlTokenInfo<std::string> SdlTokenInfo_string;
	typedef T_SdlTokenInfo<double> SdlTokenInfo_float;
	typedef T_SdlTokenInfo<bool> SdlTokenInfo_bool;

} /* namespace manta */

#endif /* SDL_TOKEN_INFO_H */
