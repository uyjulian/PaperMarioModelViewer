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

#ifndef TEXCODEC_H_
#define TEXCODEC_H_

#include <iostream>
#include "common.h"
#include "Color.h"
#include "Image.h"


class TexCodec {
public:

	enum Encoding {
		I4, I8, IA4, IA8,
		RGB0555, RGB565, RGB888,
		RGB4A3, RGB5A3, RGBA8,
		CMPR,
	};

public:

	Encoding type;
	int texWidth;
	int texHeight;
	int dataOffset;
	int cacheLineSize;
	int cacheLinesPerTile;

public:

	TexCodec () { }

	virtual ~TexCodec () { }

	Image DecodeTiled (const std::vector<u8>& buffer, int bufferOffset) const {
		int bpp = EncodingBPP (type);
		int tilebpp = bpp / cacheLinesPerTile;
		int tileWidth = TileWidth(cacheLineSize, tilebpp);
		int tileHeight = (cacheLineSize * 8) / (tileWidth * tilebpp);

		// Create tile-aligned virtual width and height
		int virWidth = texWidth;
		int virHeight = texHeight;
		if (virWidth % tileWidth != 0) {
			virWidth += tileWidth - (virWidth % tileWidth);
		}
		if (virHeight % tileHeight != 0) {
			virHeight += tileHeight - (virHeight % tileHeight);
		}

		Image img(texWidth, texHeight, Image::RGBA8);
		int tilesWide = virWidth / tileWidth;
		int tilesHigh = virHeight / tileHeight;

		for (int y = 0; y < tilesHigh; y++) {
			for (int x = 0; x < tilesWide; x++) {
				int tileData = bufferOffset + (y * tilesWide * cacheLineSize * cacheLinesPerTile) + (x * cacheLineSize * cacheLinesPerTile);

				for (int b = 0; b < tileHeight; b++) {
					for (int a = 0; a < tileWidth; a++) {
						int pxData = tileData + (b * tileWidth * tilebpp / 8) + (a * tilebpp / 8);
						int imgX = (x * tileWidth) + a;
						int imgY = (y * tileHeight) + b;

						if (imgX > texWidth || imgY > texHeight) {
							continue;
						}

						switch (type) {
							case I4: img.setPixel(imgX, imgY, UnpackI4(buffer[pxData])); break;
							case I8: img.setPixel(imgX, imgY, UnpackI8(buffer[pxData])); break;
							case IA4: img.setPixel(imgX, imgY, UnpackIA4(buffer[pxData])); break;
							case IA8: img.setPixel(imgX, imgY, UnpackIA8(getU16(buffer, pxData))); break;
							case RGB0555: img.setPixel(imgX, imgY, UnpackRGB0555(getU16(buffer, pxData))); break;
							case RGB4A3: img.setPixel(imgX, imgY, UnpackRGB4A3(getU16(buffer, pxData))); break;
							case RGB565: img.setPixel(imgX, imgY, UnpackRGB565(getU16(buffer, pxData))); break;
							case RGB5A3: img.setPixel(imgX, imgY, UnpackRGB5A3(getU16(buffer, pxData))); break;
							case RGBA8: img.setPixel(imgX, imgY, UnpackRGBA8(getU16(buffer, pxData), getU16(buffer, pxData+cacheLineSize))); break;
							default: break;
						}
					}
				}
			}
		}

		return img;
	}

	int avg(int w0, int w1, int c0, int c1) const
	{
	    int a0 = c0 >> 11;
	    int a1 = c1 >> 11;
	    int a = (w0 * a0 + w1 * a1) / (w0 + w1);
	    int c = (a << 11) & 0xffff;

	    a0 = (c0 >> 5) & 63;
	    a1 = (c1 >> 5) & 63;
	    a = (w0 * a0 + w1 * a1) / (w0 + w1);
	    c = c | ((a << 5) & 0xffff);

	    a0 = c0 & 31;
	    a1 = c1 & 31;
	    a = (w0 * a0 + w1 * a1) / (w0 + w1);
	    c = c | a;

	    return c;
	}

	u16 ToUInt16(const std::vector<u8>& array) const {
		u16 i=0, arrToInt=0;
		for(i=0;i<=1;i++)
		    arrToInt =(arrToInt<<8) | array[i];
		return arrToInt;
	}

	u32 ToUInt32(const std::vector<u8>& array) const {
		return array[0] | (array[1] << 8) | (array[2] << 16) | (array[3] << 24);
	}

	Image DecodeCMPR (const std::vector<u8>& buffer, int bufferOffset) const {
		int Width = texWidth;
		int Height = texHeight;
		int Offset = bufferOffset;
		int ww = AddPadding(Width,8) / 2;
		std::vector<int> Color(4);
        
		Image img(texWidth, texHeight, Image::RGBA8);

		for (int y = 0; y < Height; y += 4)
		{
		    for (int x = 0; x < Width; x += 4)
		    {
		    	int Pos = (((x>>2)&1) + (2*((y>>2)&1)) + (4*(x>>3)) + (ww*(y>>3))) * 8;
		    	
		    	// int c0 = buffer[Offset+Pos:Offset+Pos+2];
		    	// int c1 = buffer[Offset+Pos+2:Offset+Pos+4];
		        std::vector<u8> tmp1(2);
		        tmp1[1] = buffer[Offset + Pos];
		        tmp1[0] = buffer[Offset + Pos + 1];
                int c0 = ToUInt16(tmp1);
		        tmp1[1] = buffer[Offset + Pos + 2];
		        tmp1[0] = buffer[Offset + Pos + 3];
		        int c1 = ToUInt16(tmp1);
		    	
		    	Color[0] = ((c0 & 0xf800) << 16)  | ((c0 & 0xe000) << 11)  | ((c0 & 0x07e0) << 13)  | ((c0 & 0x0600) <<  7)  | ((c0 & 0x001f) << 11)  | ((c0 & 0x001c) <<  6) | 0xFF;
		    	Color[1] = ((c1 & 0xf800) << 16)  | ((c1 & 0xe000) << 11)  | ((c1 & 0x07e0) << 13)  | ((c1 & 0x0600) <<  7)  | ((c1 & 0x001f) << 11)  | ((c1 & 0x001c) <<  6) | 0xFF;
		    	if (c0 > c1)
		    	{
		    		Color[2] = ((((Color[0]>>24)*2+( Color[1]>>24))/3) << 24) | (((((Color[0]>>16)&0xFF)*2+((Color[1]>>16)&0xFF))/3) << 16) | (((((Color[0]>> 8)&0xFF)*2+((Color[1]>> 8)&0xFF))/3) << 8) | 0xFF;
		    		Color[3] = ((((Color[0]>>24)+( Color[1]>>24)*2)/3) << 24) | (((((Color[0]>>16)&0xFF)+((Color[1]>>16)&0xFF)*2)/3) << 16) | (((((Color[0]>> 8)&0xFF)+((Color[1]>> 8)&0xFF)*2)/3) << 8) | 0xFF;
		    	}
		    	else
		    	{
		    		Color[2] = ((((Color[0]>>24)+( Color[1]>>24))/2) << 24) | (((((Color[0]>>16)&0xFF)+((Color[1]>>16)&0xFF))/2) << 16) | (((((Color[0]>>8)&0xFF)+((Color[1]>>8)&0xFF))/2) << 8) | 0xFF;
		    		Color[3] = 0;
		    	}

		        std::vector<u8> pixeldata(4);
		        pixeldata[3] = buffer[Offset + Pos + 4];
		        pixeldata[2] = buffer[Offset + Pos + 5];
		        pixeldata[1] = buffer[Offset + Pos + 6];
		        pixeldata[0] = buffer[Offset + Pos + 7];
		        u32 Indexes = ToUInt32(pixeldata);
		    	//int Indexes = buffer[Offset+Pos+4:Offset+Pos+8];
		        for (int y1 = 0; y1 < 4; y1++) {
		       		for (int x1 = 0; x1 < 4; x1++) {
                        if (!(((x+x1) >= Width) || ((y+y1) >= Height))) {
                            u32 pizx = Color[(Indexes >> (30 - (x1*2 + y1*8))) & 0x03];
                            u8 r = (((pizx>>24) & 0xFF));
                            u8 g = (((pizx>>16) & 0xFF));
                            u8 b = ((pizx>>8) & 0xFF);
                            u8 a = (pizx & 0xFF);
                            Color8 colorpix;
                            colorpix.set(b, g, r, a);
                            img.setPixel(((x+x1) ), ((y+y1) ), colorpix);
                        }
		        	}
		        }
                
		    }
            
		}

//        
//        int width = texWidth;
//        int height = texHeight;
//        int offset = bufferOffset;
//        
//        int inp = 0;
//        
//        Image img(texWidth, texHeight, Image::RGBA8);
//        for (int y = 0; y < height; y++)
//        {
//            for (int x = 0; x < width; x++)
//            {
//		     	std::vector<u16> c(4);
//		 		std::vector<int> colorz(4);
//                int ww = AddPadding(width, 8);
//                
//                int x0 = x & 0x03;
//                int x1 = (x >> 2) & 0x01;
//                int x2 = x >> 3;
//                
//                int y0 = y & 0x03;
//                int y1 = (y >> 2) & 0x01;
//                int y2 = y >> 3;
//                int off = (8 * x1) + (16 * y1) + (32 * x2) + (4 * ww * y2);
//                
//                std::vector<u8> tmp1(2);
//                tmp1[1] = buffer[offset + off];
//                tmp1[0] = buffer[offset + off + 1];
//                c[0] = ToUInt16(tmp1);
//                tmp1[1] = buffer[offset + off + 2];
//                tmp1[0] = buffer[offset + off + 3];
//                c[1] = ToUInt16(tmp1);
//                
//                if (c[0] > c[1])
//                {
//                    c[2] = (u16)avg(2, 1, c[0], c[1]);
//                    c[3] = (u16)avg(1, 2, c[0], c[1]);
//                }
//                else
//                {
//                    c[2] = (u16)avg(1, 1, c[0], c[1]);
//                    c[3] = 0;
//                }
//                
//                std::vector<u8> pixeldata(4);
//                pixeldata[3] = buffer[offset + off + 4];
//                pixeldata[2] = buffer[offset + off + 5];
//                pixeldata[1] = buffer[offset + off + 6];
//                pixeldata[0] = buffer[offset + off + 7];
//                u32 pixel = ToUInt32(pixeldata);
//                
//                int ix = x0 + (4 * y0);
//                int raw = c[(pixel >> (30 - (2 * ix))) & 0x03];
//                
//                std::vector<u8> pix(3);
//                pix[0] = (raw >> 8) & 0xf8;
//                pix[1] = (raw >> 3) & 0xf8;
//                pix[2] = (raw << 3) & 0xf8;
//                
//                Color8 colorpix;
//                
//                //x, width = >
//                //y, height = V
//                
//                
//                int tempx = 0;
//                int tempy = 0;
//                if (inp >= width) {
//		         	tempx = (inp % width);
//		         	tempy = (inp / width);
//                }
//                else {
//		         	tempx = inp;
//		         	tempy = 0;
//                }
//                
//                for (int x1 = 0; x1 < 4; x1++) {
//                    for (int y1 = 0; y1 < 4; y1++) {
//		        		
//                        
//		         	}
//                }
//                colorpix.set(pix[2], pix[1], pix[0], 255);
//                img.setPixel(tempx, tempy, colorpix);
//                
//                inp++;
//            }
//        }
//        
//        
//        
//        
//
//		int bpp = EncodingBPP (type);
//		int tilebpp = bpp / cacheLinesPerTile;
//		int tileWidth = TileWidth(cacheLineSize, tilebpp);
//		int tileHeight = (cacheLineSize * 8) / (tileWidth * tilebpp);
//
//		// Create tile-aligned virtual width and height
//		int virWidth = texWidth;
//		int virHeight = texHeight;
//		if (virWidth % tileWidth != 0) {
//			virWidth += tileWidth - (virWidth % tileWidth);
//		}
//		if (virHeight % tileHeight != 0) {
//			virHeight += tileHeight - (virHeight % tileHeight);
//		}
//
//		int tilesWide = virWidth / tileWidth;
//		int tilesHigh = virHeight / tileHeight;
//
//		Image img(texWidth, texHeight, Image::RGBA8);
//
//		// Decode tiles
//		for (int y = 0; y < tilesHigh; y++) {
//			for (int x = 0; x < tilesWide; x++) {
//				int tileData = bufferOffset + (y * tilesWide * cacheLineSize) + (x * cacheLineSize);
//				std::vector<Color8> pixels(tileWidth * tileHeight);
//
//				// Decode blocks
//				for (int i = 0; i < 4; i++) {
//					int blockData = tileData + (i * 8);
//					DecodeCMPRBlock(x, y, i, buffer, blockData, pixels);
//				}
//
//				img.setPixelBlock(x * tileWidth, y * tileHeight, tileWidth, tileHeight, pixels, 0, tileWidth);
//			}
//		}

		return img;
	}

	int AddPadding(int value)
	{
	    return AddPadding(value, 64);
	}

	int AddPadding(int value, int padding) const
	{
	    if (value % padding != 0)
	    {
	        value = value + (padding - (value % padding));
	    }

	    return value;
	}


	void DecodeCMPRBlock (int x, int y, int block, const std::vector<u8>& buffer, int blockData, std::vector<Color8>& pixels) const {

		// Determine sub-tile coordinate
		int pxOffset = 0;
		int pyOffset = 0;
		switch (block) {
			case 0: break;
			case 1: pxOffset = 4; break;
			case 2: pyOffset = 4; break;
			case 3: pxOffset = 4; pyOffset = 4; break;
		}

		// Generate colors
		std::vector<Color8> colorset(4);
		colorset[0] = UnpackRGB565(getU16(buffer, blockData + 0));
		colorset[1] = UnpackRGB565(getU16(buffer, blockData + 2));

		if (PackRGB888(colorset[0]) > PackRGB888(colorset[1])) {
			colorset[2].blend(colorset[0], colorset[1], 85);
			colorset[3].blend(colorset[0], colorset[1], 171);
		}
		else {
			colorset[2].blend(colorset[0], colorset[1], 127);
			colorset[3].set(0, 0, 0, 0);
		}

		// Read bits
		std::vector<u8> bits(16);
		u32 bitset = getU32(buffer, blockData + 4);

		for (int i = 0; i < 16; i++) {
			bits[15-i] = (bitset & (0x03 << (i * 2))) >> (i * 2);
		}

		for (int yi = 0; yi < 4; yi++) {
			for (int xi = 0; xi < 4; xi++) {
				int px = pxOffset + xi;
				int py = pyOffset + yi;

				pixels[px + (py * 8)] = colorset[bits[(yi * 4) + xi]];
			}
		}
	}

	int EncodingBPP (Encoding _type) const {
		switch (_type) {
			case CMPR: return 4;
			case I4: return 4;
			case I8: return 8;
			case IA4: return 8;
			case IA8: return 16;
			case RGB0555: return 16;
			case RGB565: return 16;
			case RGB888: return 24;
			case RGB4A3: return 16;
			case RGB5A3: return 16;
			case RGBA8: return 32;
			default: return 16;
		}
	}

	int TileWidth (int cacheLine, int tilebpp) const {
		switch (tilebpp) {
			case 1: return cacheLine / 2;
			case 2: return cacheLine / 2;
			case 4: return cacheLine / 4;
			case 8: return cacheLine / 4;
			case 16: return cacheLine / 8;
			case 32: return cacheLine / 8;
			default: return cacheLine / 8;
		}
	}

	u32 PackRGB888 (const Color8& c) const {
		return (c.red() << 16) | (c.green() << 8) | c.blue();
	}

	u32 PackRGBA8 (const Color8& c) const {
		return (c.alpha() << 24) | (c.red() << 16) | (c.green() << 8) | c.blue();
	}

	Color8 UnpackI4 (u8 c) const {
		c &= 0x0F;
		c += c << 4;
		return Color8(c, c, c, 255);
	}

	Color8 UnpackI8 (u8 c) const {
		return Color8(c, c, c, 255);
	}

	Color8 UnpackIA4 (u8 c) const {
		char a = c & 0x0F;
		c = c >> 4;
		return Color8(c, c, c, a);
	}

	Color8 UnpackIA8 (u16 c) const {
		char a = c & 0x00FF;
		c = c >> 8;
		return Color8(a, a, a, c);
	}

	Color8 UnpackRGB0555 (u16 c) const {
		char r = ((c >> 10) & 0x1F) << 3;
		char g = ((c >> 5) & 0x1F) << 3;
		char b = (c & 0x1F) << 3;
		r += (r >> 5);
		g += (g >> 5);
		b += (b >> 5);
		return Color8(r, g, b, 255);
	}

	Color8 UnpackRGB4A3 (u16 c) const {
		char a = ((c >> 12) & 0x7) << 5;
		char r = ((c >> 8) & 0xF) << 4;
		char g = ((c >> 4) & 0xF) << 4;
		char b = (c & 0xF) << 4;
		a += (a >> 3);
		r += (r >> 4);
		g += (g >> 4);
		b += (b >> 4);
		return Color8(r, g, b, a);
	}

	Color8 UnpackRGB565 (u16 c) const {
		char r = ((c >> 11) & 0x1F) << 3;
		char g = ((c >> 5) & 0x3F) << 2;
		char b = (c & 0x1F) << 3;
		r += (r >> 5);
		g += (g >> 6);
		b += (b >> 5);
		return Color8(r, g, b, 255);
	}

	Color8 UnpackRGB5A3 (u16 c) const {
		if ((c >> 15) == 1) {
			return UnpackRGB0555(c);
		}
		return UnpackRGB4A3(c);
	}

	Color8 UnpackRGBA8 (u16 c0, u16 c1) const {
		char a = c0 >> 8;
		char r = c0 & 0xFF;
		char g = c1 >> 8;
		char b = c1 & 0xFF;
		return Color8(r, g, b, a);
	}

	Color8 UnpackRGBA8 (u32 c) const {
		return UnpackRGBA8(c >> 16, c & 0xFFFF);
	}
};

#endif /* TEXCODEC_H_ */
