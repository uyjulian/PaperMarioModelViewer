/**
 * Copyright (c) 2009 Justin Aquadro
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <vector>
#include <iostream>
#include <string>

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned long		u32;
typedef unsigned long long	u64;
typedef	signed char			s8;
typedef signed short		s16;
typedef signed long			s32;
typedef signed long long	s64;
typedef float				f32;
typedef	double				f64;

inline float halfFloat (u16 hf) {
	u32 f = (hf & 0x8000) << 16;
	u16 e = (hf >> 10) & 0x001F;

	f |= (e + 112) << 23;
	f |= (hf & 0x03FF) << 13;

	if (e == 0x00) f &= 0x807FFFFF;
	if (e == 0x1F) f |= 0x7F800000;

	return (*(f32*) &f);
}

/*inline void byteSwap (u16 &s)
{
	u8 v1 = s & 0xFF;
	u8 v2 = s >> 8;
	s = (v1 << 8) | v2;
}*/

inline u16 byteSwap (u16 s)
{
	u8 v1 = s & 0xFF;
	u8 v2 = s >> 8;
	return (v1 << 8) | v2;
}

inline void byteSwap (u32 &s)
{
	u8 v1 = s & 0xFF;
	u8 v2 = (s >> 8) & 0xFF;
	u8 v3 = (s >> 16) & 0xFF;
	u8 v4 = (s >> 24) & 0xFF;
	s = (v1 << 24) | (v2 << 16) | (v3 << 8) | v4;
}

inline void byteSwap (s16 &s)
{
	//byteSwap(*(u16*) &s);
}

inline void byteSwap (s32 &s)
{
	byteSwap(*(u32*) &s);
}

inline void byteSwap (f32 &f)
{
	byteSwap(*(u32*) &f);
}

inline u8 getComponent (u32 src, int idx) {
	int mask = (0xFF << (idx * 8));
	return (u8) ((src & mask) >> (idx * 8));
}

inline s8 getSComponent (u32 src, int idx) {
	return (s8) getComponent(src, idx);
}

inline u16 getU16 (const std::vector<u8>& v, int index) {
	return v[index] << 8 | v[index+1];
}

inline u32 getU32 (const std::vector<u8>& v, int index) {
	return v[index] << 24 | v[index+1] << 16 | v[index+2] << 8 | v[index+3];
}

inline s16 getS16 (const std::vector<u8>& v, int index) {
	return (s16) getU16(v, index);
}

inline s32 getS32 (const std::vector<u8>& v, int index) {
	return (s32) getU32(v, index);
}

inline f32 getF16 (const std::vector<u8>& v, int index) {
	u16 raw = getU16(v, index);
	return halfFloat(raw);
}

inline f32 getF32 (const std::vector<u8>& v, int index) {
	u32 raw = getU32(v, index);
	return (*(f32*) &raw);
}

inline std::string getString (const std::vector<u8>& v, int index, int maxLen) {
	std::string str = "";
	for (int i = 0; i < maxLen; i++) {
		if (v[index + i] == 0) break;
		str.append(1, v[index + i]);
	}
	return str;
}

inline float PI () {
	return 3.14159265f;
}

inline float radians (float degrees) {
	return (degrees * PI()) / 180.f;
}

inline std::string pathname (const std::string& str) {
	unsigned lastsep = str.find_last_of("/\\");
	return str.substr(0, lastsep);
}

inline void toggle (int &v) {
	(v) ? v = 0 : v = 1;
}

inline bool fileExists (const std::string &filename) {
	FILE *fh = fopen(filename.c_str(), "rb");
	if (fh != NULL) {
		fclose(fh);
		return true;
	}
	return false;
}

#endif /* COMMON_H_ */
