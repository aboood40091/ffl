#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.


################################################################
################################################################


import itertools
import json
import math
import os
import struct
import sys
import zlib

from ninTexUtils.gx2 import GX2SurfaceDim
from ninTexUtils.gx2 import GX2SurfaceFormat
from ninTexUtils.gx2 import GX2TileMode
from ninTexUtils.gx2 import GX2CompSel
from ninTexUtils.gx2 import GX2Texture
from ninTexUtils.gx2 import GX2TextureToDDS, GX2TextureToPNG
from ninTexUtils.gx2 import DDSToGX2Texture  # , PNGToGX2Texture

import pygltflib


FS_IO_BUFFER_ALIGN = 0x40
TEXTURE_DATA_MAX_ALIGNMENT = 0x800
ATTRIBUTE_DATA_ALIGNMENT = 0x40
INDEX_DATA_ALIGNMENT = 0x20


# FFLiResourceCompressLevel
FFLI_RESOURCE_COMPRESS_LEVEL_NO_COMPRESSION         = 0
FFLI_RESOURCE_COMPRESS_LEVEL_BEST_SPEED             = 1
FFLI_RESOURCE_COMPRESS_LEVEL_BEST_COMPRESSION       = 9
FFLI_RESOURCE_COMPRESS_LEVEL_DEFAULT_COMPRESSION    = 10
FFLI_RESOURCE_COMPRESS_LEVEL_MAX                    = 11

# FFLiResourceWindowBits
FFLI_RESOURCE_WINDOW_BITS_ZLIB_8    =  0    # Window size = 0x0100 (256 B)
FFLI_RESOURCE_WINDOW_BITS_ZLIB_9    =  1    # Window size = 0x0200 (512 B)
FFLI_RESOURCE_WINDOW_BITS_ZLIB_10   =  2    # Window size = 0x0400 (1 KB)
FFLI_RESOURCE_WINDOW_BITS_ZLIB_11   =  3    # Window size = 0x0800 (2 KB)
FFLI_RESOURCE_WINDOW_BITS_ZLIB_12   =  4    # Window size = 0x1000 (4 KB)
FFLI_RESOURCE_WINDOW_BITS_ZLIB_13   =  5    # Window size = 0x2000 (8 KB)
FFLI_RESOURCE_WINDOW_BITS_ZLIB_14   =  6    # Window size = 0x4000 (16 KB)
FFLI_RESOURCE_WINDOW_BITS_ZLIB_15   =  7    # Window size = 0x8000 (32 KB)

FFLI_RESOURCE_WINDOW_BITS_GZIP_8    =  8    # Window size = 0x0100 (256 B)
FFLI_RESOURCE_WINDOW_BITS_GZIP_9    =  9    # Window size = 0x0200 (512 B)
FFLI_RESOURCE_WINDOW_BITS_GZIP_10   = 10    # Window size = 0x0400 (1 KB)
FFLI_RESOURCE_WINDOW_BITS_GZIP_11   = 11    # Window size = 0x0800 (2 KB)
FFLI_RESOURCE_WINDOW_BITS_GZIP_12   = 12    # Window size = 0x1000 (4 KB)
FFLI_RESOURCE_WINDOW_BITS_GZIP_13   = 13    # Window size = 0x2000 (8 KB)
FFLI_RESOURCE_WINDOW_BITS_GZIP_14   = 14    # Window size = 0x4000 (16 KB)
FFLI_RESOURCE_WINDOW_BITS_GZIP_15   = 15    # Window size = 0x8000 (32 KB)

FFLI_RESOURCE_WINDOW_BITS_ZLIB_OR_GZIP_15   = 16    # Window size = 0x8000 (32 KB)

FFLI_RESOURCE_WINDOW_BITS_MAX   = 17    # Equivalent to FFLI_RESOURCE_WINDOW_BITS_ZLIB_15, although most likely unintentional

# FFLiResourceMemoryLevel
FFLI_RESOURCE_MEMORY_LEVEL_1    = 0
FFLI_RESOURCE_MEMORY_LEVEL_2    = 1
FFLI_RESOURCE_MEMORY_LEVEL_3    = 2
FFLI_RESOURCE_MEMORY_LEVEL_4    = 3
FFLI_RESOURCE_MEMORY_LEVEL_5    = 4
FFLI_RESOURCE_MEMORY_LEVEL_6    = 5
FFLI_RESOURCE_MEMORY_LEVEL_7    = 6
FFLI_RESOURCE_MEMORY_LEVEL_8    = 7
FFLI_RESOURCE_MEMORY_LEVEL_9    = 8
FFLI_RESOURCE_MEMORY_LEVEL_MAX  = 9

# FFLiResourceStrategy
FFLI_RESOURCE_STRATEGY_DEFAULT_STRATEGY = 0
FFLI_RESOURCE_STRATEGY_FILTERED         = 1
FFLI_RESOURCE_STRATEGY_HUFFMAN_ONLY     = 2
FFLI_RESOURCE_STRATEGY_RLE              = 3
FFLI_RESOURCE_STRATEGY_FIXED            = 4
FFLI_RESOURCE_STRATEGY_UNCOMPRESSED     = 5
FFLI_RESOURCE_STRATEGY_MAX              = 6


class FFLiResourcePartsInfo:
    _format = '>3I4B'
    size = struct.calcsize(_format)
    assert size == 0x10

    def __init__(self):
        self.compressLevel = FFLI_RESOURCE_COMPRESS_LEVEL_MAX
        self.windowBits = FFLI_RESOURCE_WINDOW_BITS_MAX
        self.memoryLevel = FFLI_RESOURCE_MEMORY_LEVEL_MAX
        self.strategy = FFLI_RESOURCE_STRATEGY_MAX

    def load(self, partsInfoData, data, pos=0, isExpand=False, expandAlignment=0):
        (dataPos,
         dataSize,
         compressedSize,
         self.compressLevel,
         self.windowBits,
         self.memoryLevel,
         self.strategy) = struct.unpack(self._format, partsInfoData)

        alignment = FS_IO_BUFFER_ALIGN
        if isExpand:
            alignment = max(alignment, expandAlignment)
            assert self.strategy == FFLI_RESOURCE_STRATEGY_UNCOMPRESSED

        assert dataPos % alignment == 0

        if dataSize == 0:
            partsData = b''

        elif self.strategy == FFLI_RESOURCE_STRATEGY_UNCOMPRESSED:
            partsData = data[pos + dataPos:pos + dataPos + dataSize]

        else:
            assert self.compressLevel < FFLI_RESOURCE_COMPRESS_LEVEL_MAX
            assert self.windowBits < FFLI_RESOURCE_WINDOW_BITS_MAX
            assert self.memoryLevel < FFLI_RESOURCE_MEMORY_LEVEL_MAX
            assert self.strategy < FFLI_RESOURCE_STRATEGY_UNCOMPRESSED

            # compressLevel = -1 if self.compressLevel == FFLI_RESOURCE_COMPRESS_LEVEL_DEFAULT_COMPRESSION else self.compressLevel
            windowBits = [
                     8,      9,      10,      11,      12,      13,      14,      15,   # zlib
                16 + 8, 16 + 9, 16 + 10, 16 + 11, 16 + 12, 16 + 13, 16 + 14, 16 + 15,   # gzip
                                                                             32 + 15,   # zlib or gzip
                                                                                  15    # default
            ][self.windowBits]
            # memoryLevel = self.memoryLevel + 1
            # strategy = self.strategy

            partsData = zlib.decompress(data[pos + dataPos:pos + dataPos + compressedSize], windowBits, dataSize)

        assert len(partsData) == dataSize
        return partsData

    def save(self, partsData, currentFileSize, isExpand=False, expandAlignment=0):
        data = bytearray()

        if partsData:
            alignment = FS_IO_BUFFER_ALIGN
            if isExpand:
                alignment = max(alignment, expandAlignment)
                assert self.strategy == FFLI_RESOURCE_STRATEGY_UNCOMPRESSED

            align = lambda x, y: x + (y - 1) & ~(y - 1)
            prevFileSize = currentFileSize
            currentFileSize = align(currentFileSize, alignment)
            data += b'\0' * (currentFileSize - prevFileSize)

            dataPos = currentFileSize
            dataSize = len(partsData)

            if self.strategy == FFLI_RESOURCE_STRATEGY_UNCOMPRESSED:
                compressedSize = 0

            else:
                assert self.compressLevel < FFLI_RESOURCE_COMPRESS_LEVEL_MAX
                assert self.windowBits < FFLI_RESOURCE_WINDOW_BITS_MAX
                assert self.memoryLevel < FFLI_RESOURCE_MEMORY_LEVEL_MAX
                assert self.strategy < FFLI_RESOURCE_STRATEGY_UNCOMPRESSED

                compressLevel = -1 if self.compressLevel == FFLI_RESOURCE_COMPRESS_LEVEL_DEFAULT_COMPRESSION else self.compressLevel
                windowBits = [
                         8,      9,      10,      11,      12,      13,      14,      15,   # zlib
                    16 + 8, 16 + 9, 16 + 10, 16 + 11, 16 + 12, 16 + 13, 16 + 14, 16 + 15,   # gzip
                                                                                 32 + 15,   # zlib or gzip
                                                                                      15    # default
                ][self.windowBits]
                memoryLevel = self.memoryLevel + 1
                strategy = self.strategy

                compressobj = zlib.compressobj(compressLevel, zlib.DEFLATED, windowBits, memoryLevel, strategy)
                partsData = b''.join([
                    compressobj.compress(partsData),
                    compressobj.flush()
                ])
                compressedSize = len(partsData)

            data += partsData

        else:
            dataPos = 0
            dataSize = 0
            compressedSize = 0

        partsInfoData = struct.pack(
            self._format,
            dataPos,
            dataSize,
            compressedSize,
            self.compressLevel,
            self.windowBits,
            self.memoryLevel,
            self.strategy
        )

        return partsInfoData, data

    def compare(self, other, label):
        if label:
            label += ".FFLiResourcePartsInfo"
        else:
            label = "FFLiResourcePartsInfo"

        if self.compressLevel != other.compressLevel:
            print(label + ".CompressLevel:", self.compressLevel, other.compressLevel)

        if self.windowBits != other.windowBits:
            print(label + ".WindowBits:", self.windowBits, other.windowBits)

        if self.memoryLevel != other.memoryLevel:
            print(label + ".MemoryLevel:", self.memoryLevel, other.memoryLevel)

        if self.strategy != other.strategy:
            print(label + ".Strategy:", self.strategy, other.strategy)


class FFLiResourceTextureFooter:
    _format = '>I2H2B2x'
    size = struct.calcsize(_format)
    assert size == 0xC

    @staticmethod
    def load(data):
        if not data:
            return None

        footerOffset = len(data) - FFLiResourceTextureFooter.size

        (mipOffset,
         width,
         height,
         numMips,
         textureFormat) = struct.unpack_from(FFLiResourceTextureFooter._format, data, footerOffset)

        gx2Texture = GX2Texture.initTexture(
            GX2SurfaceDim.Dim2D,
            width, height, 1, numMips,
            *([
                (GX2SurfaceFormat.Unorm_R8,    GX2CompSel.RZZO),
                (GX2SurfaceFormat.Unorm_RG8,   GX2CompSel.RGZO),
                (GX2SurfaceFormat.Unorm_RGBA8, GX2CompSel.RGBA)
            ][textureFormat])
        )

        if mipOffset:
            assert gx2Texture.surface.imageSize <= mipOffset
            assert mipOffset + gx2Texture.surface.mipSize <= footerOffset

        else:
            assert gx2Texture.surface.imageSize <= footerOffset

        gx2Texture.surface.imageData = data[:gx2Texture.surface.imageSize]

        if mipOffset:
            gx2Texture.surface.mipData = data[mipOffset:mipOffset + gx2Texture.surface.mipSize]

        else:
            gx2Texture.surface.mipData = b''

        return gx2Texture

    @staticmethod
    def save(gx2Texture):
        if gx2Texture is None:
            return b''

        data = bytearray(gx2Texture.surface.imageData)
        if gx2Texture.surface.mipSize == 0:
            mipOffset = 0
        else:
            prevMipOffset = len(data)
            mipOffset = gx2Texture.surface.mipOffset[0]
            data += b'\0' * (mipOffset - prevMipOffset)
            data += gx2Texture.surface.mipData

        width = gx2Texture.surface.width
        height = gx2Texture.surface.height
        numMips = gx2Texture.surface.numMips
        textureFormat = [
            GX2SurfaceFormat.Unorm_R8,
            GX2SurfaceFormat.Unorm_RG8,
            GX2SurfaceFormat.Unorm_RGBA8
        ].index(gx2Texture.surface.format)

        data += struct.pack(
            FFLiResourceTextureFooter._format,
            mipOffset,
            width,
            height,
            numMips,
            textureFormat
        )

        return data

    @staticmethod
    def compare(gx2TextureA, gx2TextureB, label):
        if label:
            label += ".FFLiResourceTexture"
        else:
            label = "FFLiResourcePartsInfo"

        widthA = gx2TextureA.surface.width
        heightA = gx2TextureA.surface.height
        numMipsA = gx2TextureA.surface.numMips
        formatA = gx2TextureA.surface.format

        widthB = gx2TextureB.surface.width
        heightB = gx2TextureB.surface.height
        numMipsB = gx2TextureB.surface.numMips
        formatB = gx2TextureB.surface.format

        if widthA != widthB:
            print(label + ".Width:", widthA, widthB)

        if heightA != heightB:
            print(label + ".Height:", heightA, heightB)

        if numMipsA != numMipsB:
            print(label + ".NumMips:", numMipsA, numMipsB)

        if formatA != formatB:
            print(label + ".Format:", formatA, formatB)

        if widthA == widthB and heightA == heightB and formatA == formatB:
            assert gx2TextureA.surface.imageSize == gx2TextureB.surface.imageSize
            if gx2TextureA.surface.imageData != gx2TextureB.surface.imageData:
                print(label + ".ImageData")
            else:
                if numMipsA == numMipsB and numMipsA > 1:
                    assert gx2TextureA.surface.mipSize == gx2TextureB.surface.mipSize
                    if gx2TextureA.surface.mipData != gx2TextureB.surface.mipData:
                        print(label + ".MipData")


# FFLiTexturePartsType
FFLI_TEXTURE_PARTS_TYPE_BEARD       =  0
FFLI_TEXTURE_PARTS_TYPE_CAP         =  1
FFLI_TEXTURE_PARTS_TYPE_EYE         =  2
FFLI_TEXTURE_PARTS_TYPE_EYEBROW     =  3
FFLI_TEXTURE_PARTS_TYPE_FACELINE    =  4
FFLI_TEXTURE_PARTS_TYPE_FACE_MAKEUP =  5
FFLI_TEXTURE_PARTS_TYPE_GLASS       =  6
FFLI_TEXTURE_PARTS_TYPE_MOLE        =  7
FFLI_TEXTURE_PARTS_TYPE_MOUTH       =  8
FFLI_TEXTURE_PARTS_TYPE_MUSTACHE    =  9
FFLI_TEXTURE_PARTS_TYPE_NOSELINE    = 10
FFLI_TEXTURE_PARTS_TYPE_MAX         = 11


class FFLiResourceTextureHeader:
    _format = '>%dI%ds%ds%ds%ds%ds%ds%ds%ds%ds%ds%ds' % (
        FFLI_TEXTURE_PARTS_TYPE_MAX,
        FFLiResourcePartsInfo.size * 3,
        FFLiResourcePartsInfo.size * 132,
        FFLiResourcePartsInfo.size * 62,
        FFLiResourcePartsInfo.size * 24,
        FFLiResourcePartsInfo.size * 12,
        FFLiResourcePartsInfo.size * 12,
        FFLiResourcePartsInfo.size * 9,
        FFLiResourcePartsInfo.size * 2,
        FFLiResourcePartsInfo.size * 37,
        FFLiResourcePartsInfo.size * 6,
        FFLiResourcePartsInfo.size * 18
    )
    size = struct.calcsize(_format)
    assert size == 0x13FC

    def load(self, headerData, data, pos=0, isExpand=False):
        (partsMaxSizeBeard,
         partsMaxSizeCap,
         partsMaxSizeEye,
         partsMaxSizeEyebrow,
         partsMaxSizeFaceline,
         partsMaxSizeFaceMake,
         partsMaxSizeGlass,
         partsMaxSizeMole,
         partsMaxSizeMouth,
         partsMaxSizeMustache,
         partsMaxSizeNoseline,
         partsInfoBeardData,
         partsInfoCapData,
         partsInfoEyeData,
         partsInfoEyebrowData,
         partsInfoFacelineData,
         partsInfoFaceMakeData,
         partsInfoGlassData,
         partsInfoMoleData,
         partsInfoMouthData,
         partsInfoMustacheData,
         partsInfoNoselineData) = struct.unpack(self._format, headerData)

        self.partsInfoBeard = []
        for i in range(3):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoBeardData[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, TEXTURE_DATA_MAX_ALIGNMENT)
            gx2Texture = FFLiResourceTextureFooter.load(partsData)
            if gx2Texture:
                gx2Texture.compSel = 0x00000000
                gx2Texture.initTextureRegs()
            self.partsInfoBeard.append([partsInfo, gx2Texture])

        self.partsInfoCap = []
        for i in range(132):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoCapData[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, TEXTURE_DATA_MAX_ALIGNMENT)
            gx2Texture = FFLiResourceTextureFooter.load(partsData)
            if gx2Texture:
                gx2Texture.compSel = 0x00000005
                gx2Texture.initTextureRegs()
            self.partsInfoCap.append([partsInfo, gx2Texture])

        self.partsInfoEye = []
        for i in range(62):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoEyeData[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, TEXTURE_DATA_MAX_ALIGNMENT)
            gx2Texture = FFLiResourceTextureFooter.load(partsData)
            self.partsInfoEye.append([partsInfo, gx2Texture])

        self.partsInfoEyebrow = []
        for i in range(24):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoEyebrowData[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, TEXTURE_DATA_MAX_ALIGNMENT)
            gx2Texture = FFLiResourceTextureFooter.load(partsData)
            if gx2Texture:
                gx2Texture.compSel = 0x00000000
                gx2Texture.initTextureRegs()
            self.partsInfoEyebrow.append([partsInfo, gx2Texture])

        self.partsInfoFaceline = []
        for i in range(12):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoFacelineData[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, TEXTURE_DATA_MAX_ALIGNMENT)
            gx2Texture = FFLiResourceTextureFooter.load(partsData)
            if gx2Texture:
                gx2Texture.compSel = 0x00000000
                gx2Texture.initTextureRegs()
            self.partsInfoFaceline.append([partsInfo, gx2Texture])

        self.partsInfoFaceMake = []
        for i in range(12):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoFaceMakeData[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, TEXTURE_DATA_MAX_ALIGNMENT)
            gx2Texture = FFLiResourceTextureFooter.load(partsData)
            self.partsInfoFaceMake.append([partsInfo, gx2Texture])

        self.partsInfoGlass = []
        for i in range(9):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoGlassData[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, TEXTURE_DATA_MAX_ALIGNMENT)
            gx2Texture = FFLiResourceTextureFooter.load(partsData)
            if gx2Texture:
                gx2Texture.compSel = 0x01010100
                gx2Texture.initTextureRegs()
            self.partsInfoGlass.append([partsInfo, gx2Texture])

        self.partsInfoMole = []
        for i in range(2):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoMoleData[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, TEXTURE_DATA_MAX_ALIGNMENT)
            gx2Texture = FFLiResourceTextureFooter.load(partsData)
            if gx2Texture:
                gx2Texture.compSel = 0x00000000
                gx2Texture.initTextureRegs()
            self.partsInfoMole.append([partsInfo, gx2Texture])

        self.partsInfoMouth = []
        for i in range(37):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoMouthData[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, TEXTURE_DATA_MAX_ALIGNMENT)
            gx2Texture = FFLiResourceTextureFooter.load(partsData)
            self.partsInfoMouth.append([partsInfo, gx2Texture])

        self.partsInfoMustache = []
        for i in range(6):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoMustacheData[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, TEXTURE_DATA_MAX_ALIGNMENT)
            gx2Texture = FFLiResourceTextureFooter.load(partsData)
            if gx2Texture:
                gx2Texture.compSel = 0x00000000
                gx2Texture.initTextureRegs()
            self.partsInfoMustache.append([partsInfo, gx2Texture])

        self.partsInfoNoseline = []
        for i in range(18):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoNoselineData[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, TEXTURE_DATA_MAX_ALIGNMENT)
            gx2Texture = FFLiResourceTextureFooter.load(partsData)
            if gx2Texture:
                gx2Texture.compSel = 0x00000000
                gx2Texture.initTextureRegs()
            self.partsInfoNoseline.append([partsInfo, gx2Texture])

    def save(self, currentFileSize, isExpand=False):
        data = bytearray()

        partsMaxSizeBeard = 0
        partsInfoBeardData = bytearray()
        for partsInfo, gx2Texture in self.partsInfoBeard:
            partsData = FFLiResourceTextureFooter.save(gx2Texture)
            partsMaxSizeBeard = max(len(partsData), partsMaxSizeBeard)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, TEXTURE_DATA_MAX_ALIGNMENT); currentFileSize += len(partsData)
            partsInfoBeardData += partsInfoData
            data += partsData

        partsMaxSizeCap = 0
        partsInfoCapData = bytearray()
        for partsInfo, gx2Texture in self.partsInfoCap:
            partsData = FFLiResourceTextureFooter.save(gx2Texture)
            partsMaxSizeCap = max(len(partsData), partsMaxSizeCap)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, TEXTURE_DATA_MAX_ALIGNMENT); currentFileSize += len(partsData)
            partsInfoCapData += partsInfoData
            data += partsData

        partsMaxSizeEye = 0
        partsInfoEyeData = bytearray()
        for partsInfo, gx2Texture in self.partsInfoEye:
            partsData = FFLiResourceTextureFooter.save(gx2Texture)
            partsMaxSizeEye = max(len(partsData), partsMaxSizeEye)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, TEXTURE_DATA_MAX_ALIGNMENT); currentFileSize += len(partsData)
            partsInfoEyeData += partsInfoData
            data += partsData

        partsMaxSizeEyebrow = 0
        partsInfoEyebrowData = bytearray()
        for partsInfo, gx2Texture in self.partsInfoEyebrow:
            partsData = FFLiResourceTextureFooter.save(gx2Texture)
            partsMaxSizeEyebrow = max(len(partsData), partsMaxSizeEyebrow)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, TEXTURE_DATA_MAX_ALIGNMENT); currentFileSize += len(partsData)
            partsInfoEyebrowData += partsInfoData
            data += partsData

        partsMaxSizeFaceline = 0
        partsInfoFacelineData = bytearray()
        for partsInfo, gx2Texture in self.partsInfoFaceline:
            partsData = FFLiResourceTextureFooter.save(gx2Texture)
            partsMaxSizeFaceline = max(len(partsData), partsMaxSizeFaceline)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, TEXTURE_DATA_MAX_ALIGNMENT); currentFileSize += len(partsData)
            partsInfoFacelineData += partsInfoData
            data += partsData

        partsMaxSizeFaceMake = 0
        partsInfoFaceMakeData = bytearray()
        for partsInfo, gx2Texture in self.partsInfoFaceMake:
            partsData = FFLiResourceTextureFooter.save(gx2Texture)
            partsMaxSizeFaceMake = max(len(partsData), partsMaxSizeFaceMake)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, TEXTURE_DATA_MAX_ALIGNMENT); currentFileSize += len(partsData)
            partsInfoFaceMakeData += partsInfoData
            data += partsData

        partsMaxSizeGlass = 0
        partsInfoGlassData = bytearray()
        for partsInfo, gx2Texture in self.partsInfoGlass:
            partsData = FFLiResourceTextureFooter.save(gx2Texture)
            partsMaxSizeGlass = max(len(partsData), partsMaxSizeGlass)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, TEXTURE_DATA_MAX_ALIGNMENT); currentFileSize += len(partsData)
            partsInfoGlassData += partsInfoData
            data += partsData

        partsMaxSizeMole = 0
        partsInfoMoleData = bytearray()
        for partsInfo, gx2Texture in self.partsInfoMole:
            partsData = FFLiResourceTextureFooter.save(gx2Texture)
            partsMaxSizeMole = max(len(partsData), partsMaxSizeMole)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, TEXTURE_DATA_MAX_ALIGNMENT); currentFileSize += len(partsData)
            partsInfoMoleData += partsInfoData
            data += partsData

        partsMaxSizeMouth = 0
        partsInfoMouthData = bytearray()
        for partsInfo, gx2Texture in self.partsInfoMouth:
            partsData = FFLiResourceTextureFooter.save(gx2Texture)
            partsMaxSizeMouth = max(len(partsData), partsMaxSizeMouth)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, TEXTURE_DATA_MAX_ALIGNMENT); currentFileSize += len(partsData)
            partsInfoMouthData += partsInfoData
            data += partsData

        partsMaxSizeMustache = 0
        partsInfoMustacheData = bytearray()
        for partsInfo, gx2Texture in self.partsInfoMustache:
            partsData = FFLiResourceTextureFooter.save(gx2Texture)
            partsMaxSizeMustache = max(len(partsData), partsMaxSizeMustache)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, TEXTURE_DATA_MAX_ALIGNMENT); currentFileSize += len(partsData)
            partsInfoMustacheData += partsInfoData
            data += partsData

        partsMaxSizeNoseline = 0
        partsInfoNoselineData = bytearray()
        for partsInfo, gx2Texture in self.partsInfoNoseline:
            partsData = FFLiResourceTextureFooter.save(gx2Texture)
            partsMaxSizeNoseline = max(len(partsData), partsMaxSizeNoseline)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, TEXTURE_DATA_MAX_ALIGNMENT); currentFileSize += len(partsData)
            partsInfoNoselineData += partsInfoData
            data += partsData

        headerData = struct.pack(
            self._format,
            partsMaxSizeBeard,
            partsMaxSizeCap,
            partsMaxSizeEye,
            partsMaxSizeEyebrow,
            partsMaxSizeFaceline,
            partsMaxSizeFaceMake,
            partsMaxSizeGlass,
            partsMaxSizeMole,
            partsMaxSizeMouth,
            partsMaxSizeMustache,
            partsMaxSizeNoseline,
            partsInfoBeardData,
            partsInfoCapData,
            partsInfoEyeData,
            partsInfoEyebrowData,
            partsInfoFacelineData,
            partsInfoFaceMakeData,
            partsInfoGlassData,
            partsInfoMoleData,
            partsInfoMouthData,
            partsInfoMustacheData,
            partsInfoNoselineData
        )

        return headerData, data

    def export(self, path='', asPng=True):
        pathIsCwd = not path or path == '.'

        if not pathIsCwd:
            cwd = os.getcwd()
            os.chdir(path)

        for partsInfoArray, name in (
            (self.partsInfoBeard, "Beard"),
            (self.partsInfoCap, "Cap"),
            (self.partsInfoEye, "Eye"),
            (self.partsInfoEyebrow, "Eyebrow"),
            (self.partsInfoFaceline, "Faceline"),
            (self.partsInfoFaceMake, "FaceMake"),
            (self.partsInfoGlass, "Glass"),
            (self.partsInfoMole, "Mole"),
            (self.partsInfoMouth, "Mouth"),
            (self.partsInfoMustache, "Mustache"),
            (self.partsInfoNoseline, "Noseline")
        ):
            for i, (partsInfo, gx2Texture) in enumerate(partsInfoArray):
                if gx2Texture is None:
                    continue

                textureFormat = [
                    GX2SurfaceFormat.Unorm_R8,
                    GX2SurfaceFormat.Unorm_RG8,
                    GX2SurfaceFormat.Unorm_RGBA8
                ].index(gx2Texture.surface.format)
                s = [
                    "R",
                    "RG",
                    "RGBA"
                ][textureFormat]

                filename = '%s_%d' % (name, i)
                texture_filename = filename + (".png" if asPng else ".dds")

                json_dict = {
                    "partsInfo": {
                        "compressLevel": partsInfo.compressLevel,
                        "windowBits": partsInfo.windowBits,
                        "memoryLevel": partsInfo.memoryLevel,
                        "strategy": partsInfo.strategy
                    },
                    "texture": {
                        "filename": texture_filename,
                        "numMips": gx2Texture.surface.numMips,
                        "format": s
                    }
                }

                json_filename = filename + '.json'
                with open(json_filename, 'w') as outf:
                    json.dump(json_dict, outf, indent=2, sort_keys=True)

                if asPng:
                    png = next(GX2TextureToPNG(gx2Texture, False))
                    png.save(texture_filename)

                else:
                    dds = GX2TextureToDDS(gx2Texture, False)
                    with open(texture_filename, "wb+") as outf:
                        outf.write(dds)

        if not pathIsCwd:
            os.chdir(cwd)

    def importFromPath(self, path=''):
        pathIsCwd = not path or path == '.'

        if not pathIsCwd:
            cwd = os.getcwd()
            os.chdir(path)

        for count, compSel, name in (
            (  3, 0x00000000, "Beard"),
            (132, 0x00000005, "Cap"),
            ( 62, 0x00010203, "Eye"),
            ( 24, 0x00000000, "Eyebrow"),
            ( 12, 0x00000000, "Faceline"),
            ( 12, 0x00010203, "FaceMake"),
            (  9, 0x01010100, "Glass"),
            (  2, 0x00000000, "Mole"),
            ( 37, 0x00010203, "Mouth"),
            (  6, 0x00000000, "Mustache"),
            ( 18, 0x00000000, "Noseline")
        ):
            partsInfoArray = []

            for i in range(count):
                partsInfo = FFLiResourcePartsInfo()

                filename = '%s_%d' % (name, i)
                json_filename = filename + '.json'
                if not os.path.isfile(json_filename):
                    partsInfoArray.append([partsInfo, None])
                    continue

                with open(json_filename, 'r') as inf:
                    json_dict = json.load(inf)

                partsInfo.compressLevel = json_dict["partsInfo"]["compressLevel"]
                partsInfo.windowBits = json_dict["partsInfo"]["windowBits"]
                partsInfo.memoryLevel = json_dict["partsInfo"]["memoryLevel"]
                partsInfo.strategy = json_dict["partsInfo"]["strategy"]

                texture_filename = json_dict["texture"]["filename"]
                numMips = json_dict["texture"]["numMips"]
                s = json_dict["texture"]["format"]

                textureFormat = [
                    "R",
                    "RG",
                    "RGBA"
                ].index(s)
                format = [
                    GX2SurfaceFormat.Unorm_R8,
                    GX2SurfaceFormat.Unorm_RG8,
                    GX2SurfaceFormat.Unorm_RGBA8
                ][textureFormat]

                if texture_filename.endswith(".png"):
                    # # TODO: numMips, format, compSel
                    # gx2Texture = PNGToGX2Texture([texture_filename], 0, 7, GX2TileMode.Default, 0, False, (0, 1, 2, 3), False)
                    raise NotImplementedError("Importing PNG is not implemented yet.")

                else:
                    assert texture_filename.endswith(".dds")
                    gx2Texture = DDSToGX2Texture(texture_filename, 0, 7, GX2TileMode.Default, 0, False, (0, 1, 2, 3), False)
                    assert gx2Texture.surface.numMips == numMips
                    assert gx2Texture.surface.format == format
                    assert gx2Texture.compSel == compSel

                partsInfoArray.append([partsInfo, gx2Texture])

            exec("self.partsInfo%s = partsInfoArray" % name)

        if not pathIsCwd:
            os.chdir(cwd)

    def compare(self, other):
        for partsInfoArrayA, partsInfoArrayB, name in (
            (self.partsInfoBeard, other.partsInfoBeard, "Beard"),
            (self.partsInfoCap, other.partsInfoCap, "Cap"),
            (self.partsInfoEye, other.partsInfoEye, "Eye"),
            (self.partsInfoEyebrow, other.partsInfoEyebrow, "Eyebrow"),
            (self.partsInfoFaceline, other.partsInfoFaceline, "Faceline"),
            (self.partsInfoFaceMake, other.partsInfoFaceMake, "FaceMake"),
            (self.partsInfoGlass, other.partsInfoGlass, "Glass"),
            (self.partsInfoMole, other.partsInfoMole, "Mole"),
            (self.partsInfoMouth, other.partsInfoMouth, "Mouth"),
            (self.partsInfoMustache, other.partsInfoMustache, "Mustache"),
            (self.partsInfoNoseline, other.partsInfoNoseline, "Noseline")
        ):
            for i, ((partsInfoA, gx2TextureA), (partsInfoB, gx2TextureB)) in enumerate(zip(partsInfoArrayA, partsInfoArrayB)):
                label = "FFLiResourceTextureHeader.%s.%d" % (name, i)
                if gx2TextureA is None:
                    if gx2TextureB is not None:
                        print(label + ": A is None")
                    continue
                elif gx2TextureB is None:
                    print(label + ": B is None")
                    continue

                partsInfoA.compare(partsInfoB, label)
                FFLiResourceTextureFooter.compare(gx2TextureA, gx2TextureB, label)


def snormN(v, n):
    v0 = 1 << (n - 1)
    v1 = 1 << n

    # Unsigned to Signed
    v &= v1 - 1
    if v & v0:  # Check sign bit
        v -= v1

    v += v0  # Shift range from [-v0, v0 - 1] to [0, v1 - 1]
    v = 2 * v / (v1 - 1)
    v -= 1

    assert -1 <= v <= 1

    return v


def snormNFromFloat(v, n, convertToUnsigned):
    v0 = 1 << (n - 1)
    v1 = 1 << n

    assert -1 <= v <= 1

    v += 1
    v = int(v * (v1 - 1) / 2); assert 0 <= v <= v1 - 1
    v -= v0

    if convertToUnsigned:
        # Signed to unsigned
        if v < 0:
            v += v1
        v &= v1 - 1

    return v


def unormN(v, n):
    v1 = 1 << n

    v = v / (v1 - 1)

    assert 0 <= v <= 1

    return v


def unormNFromFloat(v, n):
    v1 = 1 << n

    assert 0 <= v <= 1

    v = int(v * (v1 - 1)); assert 0 <= v <= v1 - 1

    return v


class FFLiSnorm10_10_10_2:
    _format = ">I"
    size = struct.calcsize(_format)
    assert size == 4

    @staticmethod
    def load(data, pos):
        v, = struct.unpack_from(FFLiSnorm10_10_10_2._format, data, pos)

        w_snorm = (v >> 30) & 0b11
        z_snorm = (v >> 20) & 0b1111111111
        y_snorm = (v >> 10) & 0b1111111111
        x_snorm = (v >>  0) & 0b1111111111

        w = snormN(w_snorm,  2)
        z = snormN(z_snorm, 10)
        y = snormN(y_snorm, 10)
        x = snormN(x_snorm, 10)

        assert w == 1
        return (x, y, z)

    @staticmethod
    def save(x, y, z):
        w_snorm = snormNFromFloat(1,  2, True)
        z_snorm = snormNFromFloat(z, 10, True)
        y_snorm = snormNFromFloat(y, 10, True)
        x_snorm = snormNFromFloat(x, 10, True)

        v = 0
        v |= w_snorm << 30
        v |= z_snorm << 20
        v |= y_snorm << 10
        v |= x_snorm <<  0

        return struct.pack(FFLiSnorm10_10_10_2._format, v)


class FFLiSnorm8_8_8_8:
    _format = ">4b"
    size = struct.calcsize(_format)
    assert size == 4

    @staticmethod
    def load(data, pos):
        (x_snorm,
         y_snorm,
         z_snorm,
         w_snorm) = struct.unpack_from(FFLiSnorm8_8_8_8._format, data, pos)

        x = snormN(x_snorm, 8)
        y = snormN(y_snorm, 8)
        z = snormN(z_snorm, 8)
        w = snormN(w_snorm, 8)

        if w == -0.9921568627450981:
            x /= -0.9921568627450981
            y /= -0.9921568627450981
            z /= -0.9921568627450981
            w /= -0.9921568627450981

        assert w == 1

        mag = math.sqrt(x*x + y*y + z*z)
        assert mag > 0
        x /= mag
        y /= mag
        z /= mag

        return (x, y, z)

    @staticmethod
    def save(x, y, z):
        x_snorm = snormNFromFloat(x, 8, False)
        y_snorm = snormNFromFloat(y, 8, False)
        z_snorm = snormNFromFloat(z, 8, False)
        w_snorm = snormNFromFloat(1, 8, False)

        return struct.pack(
            FFLiSnorm8_8_8_8._format,
            x_snorm,
            y_snorm,
            z_snorm,
            w_snorm
        )


class FFLiUnorm8_8_8_8:
    _format = ">4B"
    size = struct.calcsize(_format)
    assert size == 4

    @staticmethod
    def load(data, pos):
        (x_unorm,
         y_unorm,
         z_unorm,
         w_unorm) = struct.unpack_from(FFLiUnorm8_8_8_8._format, data, pos)

        x = unormN(x_unorm, 8)
        y = unormN(y_unorm, 8)
        z = unormN(z_unorm, 8)
        w = unormN(w_unorm, 8)

        return (x, y, z, w)

    @staticmethod
    def save(x, y, z, w):
        x_unorm = unormNFromFloat(x, 8)
        y_unorm = unormNFromFloat(y, 8)
        z_unorm = unormNFromFloat(z, 8)
        w_unorm = unormNFromFloat(w, 8)

        return struct.pack(
            FFLiUnorm8_8_8_8._format,
            x_unorm,
            y_unorm,
            z_unorm,
            w_unorm
        )


# FFLiResourceShapeElementType
FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_POSITION   = 0
FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_NORMAL     = 1
FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TEXCOORD   = 2
FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TANGENT    = 3
FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_COLOR      = 4
FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_INDEX      = 5
FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_BUFFER_MAX = 6


class FFLiResourceShapeDataHeader:
    _dataPosSizeFormat = '>%dI' % FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_BUFFER_MAX
    _dataPosSizeFormatSize = struct.calcsize(_dataPosSizeFormat)
    _vec3Format = '>3f'
    _vec3FormatSize = struct.calcsize(_vec3Format)
    size = (_dataPosSizeFormatSize +
            _dataPosSizeFormatSize +
            _vec3FormatSize + _vec3FormatSize +
            _vec3FormatSize + _vec3FormatSize + _vec3FormatSize +_vec3FormatSize + _vec3FormatSize + _vec3FormatSize)
    assert size == 0x90

    _positionFormat = '>3f4x'
    _positionFormatSize = struct.calcsize(_positionFormat)
    assert _positionFormatSize == 0x10

    _texCoordFormat = '>2f'
    _texCoordFormatSize = struct.calcsize(_texCoordFormat)
    assert _texCoordFormatSize == 8

    _tangetFormat = '>4b'
    _tangetFormatSize = struct.calcsize(_tangetFormat)
    assert _tangetFormatSize == 4

    _colorFormat = '>4B'
    _colorFormatSize = struct.calcsize(_colorFormat)
    assert _colorFormatSize == 4

    @staticmethod
    def load(partsData, isExpand=False):
        if not partsData:
            return None

        shape = FFLiResourceShapeDataHeader()

        localPos = 0

        dataPos = struct.unpack_from(shape._dataPosSizeFormat, partsData, localPos); localPos += shape._dataPosSizeFormatSize
        dataSize = struct.unpack_from(shape._dataPosSizeFormat, partsData, localPos); localPos += shape._dataPosSizeFormatSize
        shape.boundingBox = [struct.unpack_from(shape._vec3Format, partsData, localPos + shape._vec3FormatSize * i) for i in range(2)]; localPos += shape._vec3FormatSize * 2
        shape.transform = [struct.unpack_from(shape._vec3Format, partsData, localPos + shape._vec3FormatSize * i) for i in range(6)]; localPos += shape._vec3FormatSize * 6

        assert localPos == shape.size

        if isExpand:
            assert dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_POSITION] % ATTRIBUTE_DATA_ALIGNMENT == 0
            assert dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_NORMAL] % ATTRIBUTE_DATA_ALIGNMENT == 0
            assert dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TEXCOORD] % ATTRIBUTE_DATA_ALIGNMENT == 0
            assert dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TANGENT] % ATTRIBUTE_DATA_ALIGNMENT == 0
            assert dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_COLOR] % ATTRIBUTE_DATA_ALIGNMENT == 0
            assert dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_INDEX] % INDEX_DATA_ALIGNMENT == 0

        totalVertexNum = 0

        if dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_POSITION] == 0:
            shape.position = None

        else:
            assert dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_POSITION] % shape._positionFormatSize == 0
            vertexNum = dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_POSITION] // shape._positionFormatSize
            localPos = dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_POSITION]
            shape.position = [struct.unpack_from(shape._positionFormat, partsData, localPos + shape._positionFormatSize * i) for i in range(vertexNum)]

            totalVertexNum = vertexNum

        if dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_NORMAL] == 0:
            assert totalVertexNum == 0
            shape.normal = None

        else:
            assert dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_NORMAL] % FFLiSnorm10_10_10_2.size == 0
            vertexNum = dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_NORMAL] // FFLiSnorm10_10_10_2.size
            localPos = dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_NORMAL]
            shape.normal = [FFLiSnorm10_10_10_2.load(partsData, localPos + FFLiSnorm10_10_10_2.size * i) for i in range(vertexNum)]

            assert totalVertexNum == vertexNum

        if dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TEXCOORD] == 0:
            shape.texCoord = None

        else:
            assert dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TEXCOORD] % shape._texCoordFormatSize == 0
            vertexNum = dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TEXCOORD] // shape._texCoordFormatSize
            localPos = dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TEXCOORD]
            shape.texCoord = [struct.unpack_from(shape._texCoordFormat, partsData, localPos + shape._texCoordFormatSize * i) for i in range(vertexNum)]

            assert totalVertexNum == vertexNum

        if dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TANGENT] == 0:
            shape.tangent = None

        else:
            assert dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TANGENT] % FFLiSnorm8_8_8_8.size == 0
            vertexNum = dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TANGENT] // FFLiSnorm8_8_8_8.size
            localPos = dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TANGENT]
            shape.tangent = [FFLiSnorm8_8_8_8.load(partsData, localPos + FFLiSnorm8_8_8_8.size * i) for i in range(vertexNum)]

            assert totalVertexNum == vertexNum

        if dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_COLOR] == 0:
            assert totalVertexNum == 0
            shape.color = None

        else:
            assert dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_COLOR] % FFLiUnorm8_8_8_8.size == 0
            vertexNum = dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_COLOR] // FFLiUnorm8_8_8_8.size
            localPos = dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_COLOR]
            shape.color = [FFLiUnorm8_8_8_8.load(partsData, localPos + FFLiUnorm8_8_8_8.size * i) for i in range(vertexNum)]
            shape.uniformColor = vertexNum == 1

            if shape.uniformColor:
                shape.color *= totalVertexNum
            else:
                assert totalVertexNum == vertexNum

        indexNum = dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_INDEX]
        assert (indexNum == 0 and totalVertexNum == 0) or \
               (indexNum != 0 and totalVertexNum != 0)

        if indexNum == 0:
            shape.index = None

        else:
            localPos = dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_INDEX]
            shape.index = struct.unpack_from(">%dH" % indexNum, partsData, localPos)
            assert max(shape.index) < totalVertexNum

        if shape.tangent:
            assert shape.normal

        return shape

    @staticmethod
    def save(shape, isExpand=False):
        if shape is None:
            return b''

        dataPos = [0] * FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_BUFFER_MAX
        dataSize = [0] * FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_BUFFER_MAX

        pos = shape.size
        partsData = bytearray()

        align = lambda x, y: x + (y - 1) & ~(y - 1)

        if shape.position is not None:
            if isExpand:
                prevPos = pos
                pos = align(pos, ATTRIBUTE_DATA_ALIGNMENT)
                partsData += b'\0' * (pos - prevPos)

            data = b''.join([struct.pack(shape._positionFormat, *vec) for vec in shape.position])
            dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_POSITION] = pos
            dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_POSITION] = len(data)
            partsData += data
            pos += len(data)

        if shape.normal is not None:
            if isExpand:
                prevPos = pos
                pos = align(pos, ATTRIBUTE_DATA_ALIGNMENT)
                partsData += b'\0' * (pos - prevPos)

            data = b''.join([FFLiSnorm10_10_10_2.save(*vec) for vec in shape.normal])
            dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_NORMAL] = pos
            dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_NORMAL] = len(data)
            partsData += data
            pos += len(data)

        if shape.texCoord is not None:
            if isExpand:
                prevPos = pos
                pos = align(pos, ATTRIBUTE_DATA_ALIGNMENT)
                partsData += b'\0' * (pos - prevPos)

            data = b''.join([struct.pack(shape._texCoordFormat, *vec) for vec in shape.texCoord])
            dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TEXCOORD] = pos
            dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TEXCOORD] = len(data)
            partsData += data
            pos += len(data)

        if shape.tangent is not None:
            if isExpand:
                prevPos = pos
                pos = align(pos, ATTRIBUTE_DATA_ALIGNMENT)
                partsData += b'\0' * (pos - prevPos)

            data = b''.join([FFLiSnorm8_8_8_8.save(*vec) for vec in shape.tangent])
            dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TANGENT] = pos
            dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_TANGENT] = len(data)
            partsData += data
            pos += len(data)

        if shape.color is not None:
            if isExpand:
                prevPos = pos
                pos = align(pos, ATTRIBUTE_DATA_ALIGNMENT)
                partsData += b'\0' * (pos - prevPos)

            if shape.uniformColor:
                data = FFLiUnorm8_8_8_8.save(*(shape.color[0]))
            else:
                data = b''.join([FFLiUnorm8_8_8_8.save(*vec) for vec in shape.color])
            dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_COLOR] = pos
            dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_COLOR] = len(data)
            partsData += data
            pos += len(data)

        if shape.index is not None:
            if isExpand:
                prevPos = pos
                pos = align(pos, INDEX_DATA_ALIGNMENT)
                partsData += b'\0' * (pos - prevPos)

            indexNum = len(shape.index)
            data = struct.pack(">%dH" % indexNum, *(shape.index))
            dataPos[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_INDEX] = pos
            dataSize[FFLI_RESOURCE_SHAPE_ELEMENT_TYPE_INDEX] = indexNum
            partsData += data
            pos += len(data)

        return b''.join([
            struct.pack(shape._dataPosSizeFormat, *dataPos),
            struct.pack(shape._dataPosSizeFormat, *dataSize),
            b''.join([struct.pack(shape._vec3Format, *vec) for vec in shape.boundingBox]),
            b''.join([struct.pack(shape._vec3Format, *vec) for vec in shape.transform]),
            partsData
        ])

    @staticmethod
    def export(shape):
        if shape.index is None:
            return None

        indexNum = len(shape.index)
        vertexNum = len(shape.position)

        bufferIndex = 0
        accessors = []
        bufferViews = []
        buffer = bytearray()

        indexBufferIndex = bufferIndex; bufferIndex += 1
        accessors.append(
            pygltflib.Accessor(
                bufferView=indexBufferIndex,
                componentType=pygltflib.UNSIGNED_SHORT,
                count=indexNum,
                type=pygltflib.SCALAR
            )
        )
        bufferViews.append(
            pygltflib.BufferView(
                buffer=0,
                byteOffset=len(buffer),
                byteLength=2 * indexNum,
                target=pygltflib.ELEMENT_ARRAY_BUFFER
            )
        )
        buffer += struct.pack("<%dH" % indexNum, *shape.index)

        positionBufferIndex = bufferIndex; bufferIndex += 1
        accessors.append(
            pygltflib.Accessor(
                bufferView=positionBufferIndex,
                componentType=pygltflib.FLOAT,
                count=vertexNum,
                type=pygltflib.VEC3,
                max=shape.boundingBox[1],
                min=shape.boundingBox[0]
            )
        )
        bufferViews.append(
            pygltflib.BufferView(
                buffer=0,
                byteOffset=len(buffer),
                byteLength=4 * 3 * vertexNum,
                target=pygltflib.ARRAY_BUFFER
            )
        )
        for position in shape.position:
            buffer += struct.pack("<3f", *position)

        if shape.texCoord is None:
            texCoordBufferIndex = None
        else:
            texCoordBufferIndex = bufferIndex; bufferIndex += 1
            accessors.append(
                pygltflib.Accessor(
                    bufferView=texCoordBufferIndex,
                    componentType=pygltflib.FLOAT,
                    count=vertexNum,
                    type=pygltflib.VEC2
                )
            )
            bufferViews.append(
                pygltflib.BufferView(
                    buffer=0,
                    byteOffset=len(buffer),
                    byteLength=4 * 2 * vertexNum,
                    target=pygltflib.ARRAY_BUFFER
                )
            )
            for texCoord in shape.texCoord:
                buffer += struct.pack("<2f", *texCoord)

        normalBufferIndex = bufferIndex; bufferIndex += 1
        accessors.append(
            pygltflib.Accessor(
                bufferView=normalBufferIndex,
                componentType=pygltflib.FLOAT,
                count=vertexNum,
                type=pygltflib.VEC3
            )
        )
        bufferViews.append(
            pygltflib.BufferView(
                buffer=0,
                byteOffset=len(buffer),
                byteLength=4 * 3 * vertexNum,
                target=pygltflib.ARRAY_BUFFER
            )
        )
        for normal in shape.normal:
            buffer += struct.pack("<3f", *normal)

        if shape.tangent is None:
            tangentBufferIndex = None
        else:
            tangentBufferIndex = bufferIndex; bufferIndex += 1
            accessors.append(
                pygltflib.Accessor(
                    bufferView=tangentBufferIndex,
                    componentType=pygltflib.FLOAT,
                    count=vertexNum,
                    type=pygltflib.VEC4
                )
            )
            bufferViews.append(
                pygltflib.BufferView(
                    buffer=0,
                    byteOffset=len(buffer),
                    byteLength=4 * 4 * vertexNum,
                    target=pygltflib.ARRAY_BUFFER
                )
            )
            for tangent in shape.tangent:
                buffer += struct.pack("<4f", tangent[0], tangent[1], tangent[2], 1.0)

        colorBufferIndex = bufferIndex; bufferIndex += 1
        accessors.append(
            pygltflib.Accessor(
                bufferView=colorBufferIndex,
                componentType=pygltflib.FLOAT,
                count=vertexNum,
                type=pygltflib.VEC4
            )
        )
        bufferViews.append(
            pygltflib.BufferView(
                buffer=0,
                byteOffset=len(buffer),
                byteLength=4 * 4 * vertexNum,
                target=pygltflib.ARRAY_BUFFER
            )
        )
        for color in shape.color:
            buffer += struct.pack("<4f", *color)

        glb = pygltflib.GLTF2(
            scene=0,
            scenes=[pygltflib.Scene(nodes=[0])],
            nodes=[pygltflib.Node(mesh=0)],
            meshes=[
                pygltflib.Mesh(
                    primitives=[
                        pygltflib.Primitive(
                            attributes=pygltflib.Attributes(
                                POSITION=positionBufferIndex,
                                TEXCOORD_0=texCoordBufferIndex,
                                NORMAL=normalBufferIndex,
                                TANGENT=tangentBufferIndex,
                                COLOR_0=colorBufferIndex
                            ),
                            indices=indexBufferIndex
                        )
                    ]
                )
            ],
            accessors=accessors,
            bufferViews=bufferViews,
            buffers=[
                pygltflib.Buffer(
                    byteLength=len(buffer)
                )
            ]
        )
        glb.set_binary_blob(buffer)
        return glb

    @staticmethod
    def importFromGLB(shape, glb: pygltflib.GLTF2):
        shape.position = None
        shape.normal = None
        shape.texCoord = None
        shape.tangent = None
        shape.color = None
        shape.index = None

        shape.transform = None
        shape.boundingBox = None

        buffer = glb.binary_blob()
        assert glb.buffers[0].byteLength == len(buffer)

        primitive = glb.meshes[0].primitives[0]
        accessors = glb.accessors
        bufferViews=  glb.bufferViews

        indexBufferIndex = primitive.indices
        indexBufferAccessor = accessors[indexBufferIndex]
        assert indexBufferAccessor.byteOffset == 0
        assert indexBufferAccessor.componentType == pygltflib.UNSIGNED_SHORT
        assert indexBufferAccessor.type == pygltflib.SCALAR
        indexNum = indexBufferAccessor.count
        indexBufferView = bufferViews[indexBufferAccessor.bufferView]
        assert indexBufferView.buffer == 0
        assert indexBufferView.byteLength == 2 * indexNum
        assert indexBufferView.target == pygltflib.ELEMENT_ARRAY_BUFFER
        shape.index = struct.unpack_from("<%dH" % indexNum, buffer, indexBufferView.byteOffset)

        positionBufferIndex = primitive.attributes.POSITION
        positionBufferAccessor = accessors[positionBufferIndex]
        assert positionBufferAccessor.byteOffset == 0
        assert positionBufferAccessor.componentType == pygltflib.FLOAT
        assert positionBufferAccessor.type == pygltflib.VEC3
        shape.boundingBox = [positionBufferAccessor.min, positionBufferAccessor.max]
        vertexNum = positionBufferAccessor.count
        positionBufferView = bufferViews[positionBufferAccessor.bufferView]
        assert positionBufferView.buffer == 0
        assert positionBufferView.byteLength == 4 * 3 * vertexNum
        assert positionBufferView.target == pygltflib.ARRAY_BUFFER
        shape.position = [struct.unpack_from("<3f", buffer, positionBufferView.byteOffset + i * 4 * 3) for i in range(vertexNum)]

        texCoordBufferIndex = primitive.attributes.TEXCOORD_0
        if not (texCoordBufferIndex is not None and texCoordBufferIndex >= 0):
            shape.texCoord = None

        else:
            texCoordBufferAccessor = accessors[texCoordBufferIndex]
            assert texCoordBufferAccessor.byteOffset == 0
            assert texCoordBufferAccessor.componentType == pygltflib.FLOAT
            assert texCoordBufferAccessor.type == pygltflib.VEC2
            assert texCoordBufferAccessor.count == vertexNum
            texCoordBufferView = bufferViews[texCoordBufferAccessor.bufferView]
            assert texCoordBufferView.buffer == 0
            assert texCoordBufferView.byteLength == 4 * 2 * vertexNum
            assert texCoordBufferView.target == pygltflib.ARRAY_BUFFER
            shape.texCoord = [struct.unpack_from("<2f", buffer, texCoordBufferView.byteOffset + i * 4 * 2) for i in range(vertexNum)]

        normalBufferIndex = primitive.attributes.NORMAL
        normalBufferAccessor = accessors[normalBufferIndex]
        assert normalBufferAccessor.byteOffset == 0
        assert normalBufferAccessor.componentType == pygltflib.FLOAT
        assert normalBufferAccessor.type == pygltflib.VEC3
        assert normalBufferAccessor.count == vertexNum
        normalBufferView = bufferViews[normalBufferAccessor.bufferView]
        assert normalBufferView.buffer == 0
        assert normalBufferView.byteLength == 4 * 3 * vertexNum
        assert normalBufferView.target == pygltflib.ARRAY_BUFFER
        shape.normal = [struct.unpack_from("<3f", buffer, normalBufferView.byteOffset + i * 4 * 3) for i in range(vertexNum)]

        tangentBufferIndex = primitive.attributes.TANGENT
        if not (tangentBufferIndex is not None and tangentBufferIndex >= 0):
            shape.tangent = None

        else:
            tangentBufferAccessor = accessors[tangentBufferIndex]
            assert tangentBufferAccessor.byteOffset == 0
            assert tangentBufferAccessor.componentType == pygltflib.FLOAT
            assert tangentBufferAccessor.type == pygltflib.VEC4
            assert tangentBufferAccessor.count == vertexNum
            tangentBufferView = bufferViews[tangentBufferAccessor.bufferView]
            assert tangentBufferView.buffer == 0
            assert tangentBufferView.byteLength == 4 * 4 * vertexNum
            assert tangentBufferView.target == pygltflib.ARRAY_BUFFER
            shape.tangent = []
            for i in range(vertexNum):
                x, y, z, w = struct.unpack_from("<4f", buffer, tangentBufferView.byteOffset + i * 4 * 4)
                shape.tangent.append((x / w, y / w, z / w))

        colorBufferIndex = primitive.attributes.COLOR_0
        colorBufferAccessor = accessors[colorBufferIndex]
        assert colorBufferAccessor.byteOffset == 0
        assert colorBufferAccessor.componentType == pygltflib.FLOAT
        assert colorBufferAccessor.type == pygltflib.VEC4
        assert colorBufferAccessor.count == vertexNum
        colorBufferView = bufferViews[colorBufferAccessor.bufferView]
        assert colorBufferView.buffer == 0
        assert colorBufferView.byteLength == 4 * 4 * vertexNum
        assert colorBufferView.target == pygltflib.ARRAY_BUFFER
        shape.color = [struct.unpack_from("<4f", buffer, colorBufferView.byteOffset + i * 4 * 4) for i in range(vertexNum)]

        g = itertools.groupby(shape.color)
        shape.uniformColor = next(g, True) and not next(g, False)
        if shape.uniformColor:
            shape.color = shape.color[:1]

    @staticmethod
    def compare(shapeA, shapeB, label):
        if label:
            label += ".FFLiResourceShapeDataHeader"
        else:
            label = "FFLiResourceShapeDataHeader"

        if shapeA.boundingBox != shapeB.boundingBox:
            print(label + ".BoundingBox")

        if shapeA.transform != shapeB.transform:
            print(label + ".Transform")

        if shapeA.index is None:
            if shapeB.index is not None:
                print(label + ": A not renderable")
            return
        elif shapeB.index is None:
            print(label + ": B not renderable")
            return

        if shapeA.position != shapeB.position:
            print(label + ".Position")

        if shapeA.normal != shapeB.normal:
            print(label + ".Normal")

        if shapeA.texCoord is None:
            if shapeB.texCoord is not None:
                print(label + ": A no TexCoord")
        elif shapeB.texCoord is None:
            print(label + ": B no TexCoord")
        else:
            if shapeA.texCoord != shapeB.texCoord:
                print(label + ".TexCoord")

        if shapeA.tangent is None:
            if shapeB.tangent is not None:
                print(label + ": A no Tangent")
        elif shapeB.tangent is None:
            print(label + ": B no Tangent")
        else:
            if shapeA.tangent != shapeB.tangent:
                print(label + ".Tangent")

        if shapeA.color != shapeB.color:
            print(label + ".Color")

        if shapeA.index != shapeB.index:
            print(label + ".Index")


# FFLAttributeBufferType
FFL_ATTRIBUTE_BUFFER_TYPE_POSITION  = 0
FFL_ATTRIBUTE_BUFFER_TYPE_TEXCOORD  = 1
FFL_ATTRIBUTE_BUFFER_TYPE_NORMAL    = 2
FFL_ATTRIBUTE_BUFFER_TYPE_TANGENT   = 3
FFL_ATTRIBUTE_BUFFER_TYPE_COLOR     = 4


# FFLiShapePartsType
FFLI_SHAPE_PARTS_TYPE_BEARD         =  0
FFLI_SHAPE_PARTS_TYPE_CAP_1         =  1
FFLI_SHAPE_PARTS_TYPE_CAP_2         =  2
FFLI_SHAPE_PARTS_TYPE_FACELINE      =  3
FFLI_SHAPE_PARTS_TYPE_GLASS         =  4
FFLI_SHAPE_PARTS_TYPE_MASK          =  5
FFLI_SHAPE_PARTS_TYPE_NOSELINE      =  6
FFLI_SHAPE_PARTS_TYPE_NOSE          =  7
FFLI_SHAPE_PARTS_TYPE_HAIR_1        =  8
FFLI_SHAPE_PARTS_TYPE_HAIR_2        =  9
FFLI_SHAPE_PARTS_TYPE_FOREHEAD_1    = 10
FFLI_SHAPE_PARTS_TYPE_FOREHEAD_2    = 11
FFLI_SHAPE_PARTS_TYPE_MAX           = 12


class FFLiResourceShapeHeader:
    _format = '>%dI%ds%ds%ds%ds%ds%ds%ds%ds%ds%ds%ds%ds' % (
        FFLI_SHAPE_PARTS_TYPE_MAX,
        FFLiResourcePartsInfo.size * 4,
        FFLiResourcePartsInfo.size * 132,
        FFLiResourcePartsInfo.size * 132,
        FFLiResourcePartsInfo.size * 12,
        FFLiResourcePartsInfo.size * 1,
        FFLiResourcePartsInfo.size * 12,
        FFLiResourcePartsInfo.size * 18,
        FFLiResourcePartsInfo.size * 18,
        FFLiResourcePartsInfo.size * 132,
        FFLiResourcePartsInfo.size * 132,
        FFLiResourcePartsInfo.size * 132,
        FFLiResourcePartsInfo.size * 132
    )
    size = struct.calcsize(_format)
    assert size == 0x35C0

    def load(self, headerData, data, pos=0, isExpand=False):
        (partsMaxSizeBeard,
         partsMaxSizeCap1,
         partsMaxSizeCap2,
         partsMaxSizeFaceline,
         partsMaxSizeGlass,
         partsMaxSizeMask,
         partsMaxSizeNoseline,
         partsMaxSizeNose,
         partsMaxSizeHair1,
         partsMaxSizeHair2,
         partsMaxSizeForehead1,
         partsMaxSizeForehead2,
         partsInfoBeardData,
         partsInfoCap1Data,
         partsInfoCap2Data,
         partsInfoFacelineData,
         partsInfoGlassData,
         partsInfoMaskData,
         partsInfoNoselineData,
         partsInfoNoseData,
         partsInfoHair1Data,
         partsInfoHair2Data,
         partsInfoForehead1Data,
         partsInfoForehead2Data) = struct.unpack(self._format, headerData)

        expandAlignment = max(ATTRIBUTE_DATA_ALIGNMENT, INDEX_DATA_ALIGNMENT)

        self.partsInfoBeard = []
        for i in range(4):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoBeardData[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, expandAlignment)
            shape = FFLiResourceShapeDataHeader.load(partsData, isExpand)
            self.partsInfoBeard.append([partsInfo, shape])

        self.partsInfoCap1 = []
        for i in range(132):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoCap1Data[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, expandAlignment)
            shape = FFLiResourceShapeDataHeader.load(partsData, isExpand)
            self.partsInfoCap1.append([partsInfo, shape])

        self.partsInfoCap2 = []
        for i in range(132):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoCap2Data[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, expandAlignment)
            shape = FFLiResourceShapeDataHeader.load(partsData, isExpand)
            self.partsInfoCap2.append([partsInfo, shape])

        self.partsInfoFaceline = []
        for i in range(12):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoFacelineData[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, expandAlignment)
            shape = FFLiResourceShapeDataHeader.load(partsData, isExpand)
            self.partsInfoFaceline.append([partsInfo, shape])

        self.partsInfoGlass = []
        for i in range(1):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoGlassData[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, expandAlignment)
            shape = FFLiResourceShapeDataHeader.load(partsData, isExpand)
            self.partsInfoGlass.append([partsInfo, shape])

        self.partsInfoMask = []
        for i in range(12):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoMaskData[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, expandAlignment)
            shape = FFLiResourceShapeDataHeader.load(partsData, isExpand)
            self.partsInfoMask.append([partsInfo, shape])

        self.partsInfoNoseline = []
        for i in range(18):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoNoselineData[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, expandAlignment)
            shape = FFLiResourceShapeDataHeader.load(partsData, isExpand)
            self.partsInfoNoseline.append([partsInfo, shape])

        self.partsInfoNose = []
        for i in range(18):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoNoseData[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, expandAlignment)
            shape = FFLiResourceShapeDataHeader.load(partsData, isExpand)
            self.partsInfoNose.append([partsInfo, shape])

        self.partsInfoHair1 = []
        for i in range(132):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoHair1Data[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, expandAlignment)
            shape = FFLiResourceShapeDataHeader.load(partsData, isExpand)
            self.partsInfoHair1.append([partsInfo, shape])

        self.partsInfoHair2 = []
        for i in range(132):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoHair2Data[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, expandAlignment)
            shape = FFLiResourceShapeDataHeader.load(partsData, isExpand)
            self.partsInfoHair2.append([partsInfo, shape])

        self.partsInfoForehead1 = []
        for i in range(132):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoForehead1Data[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, expandAlignment)
            shape = FFLiResourceShapeDataHeader.load(partsData, isExpand)
            self.partsInfoForehead1.append([partsInfo, shape])

        self.partsInfoForehead2 = []
        for i in range(132):
            partsInfo = FFLiResourcePartsInfo()
            partsData = partsInfo.load(partsInfoForehead2Data[i * FFLiResourcePartsInfo.size:(i + 1) * FFLiResourcePartsInfo.size], data, pos, isExpand, expandAlignment)
            shape = FFLiResourceShapeDataHeader.load(partsData, isExpand)
            self.partsInfoForehead2.append([partsInfo, shape])

    def save(self, currentFileSize, isExpand):
        data = bytearray()

        expandAlignment = max(ATTRIBUTE_DATA_ALIGNMENT, INDEX_DATA_ALIGNMENT)

        partsMaxSizeBeard = 0
        partsInfoBeardData = bytearray()
        for partsInfo, shape in self.partsInfoBeard:
            partsData = FFLiResourceShapeDataHeader.save(shape, isExpand)
            partsMaxSizeBeard = max(len(partsData), partsMaxSizeBeard)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, expandAlignment); currentFileSize += len(partsData)
            partsInfoBeardData += partsInfoData
            data += partsData

        partsMaxSizeCap1 = 0
        partsInfoCap1Data = bytearray()
        for partsInfo, shape in self.partsInfoCap1:
            partsData = FFLiResourceShapeDataHeader.save(shape, isExpand)
            partsMaxSizeCap1 = max(len(partsData), partsMaxSizeCap1)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, expandAlignment); currentFileSize += len(partsData)
            partsInfoCap1Data += partsInfoData
            data += partsData

        partsMaxSizeCap2 = 0
        partsInfoCap2Data = bytearray()
        for partsInfo, shape in self.partsInfoCap2:
            partsData = FFLiResourceShapeDataHeader.save(shape, isExpand)
            partsMaxSizeCap2 = max(len(partsData), partsMaxSizeCap2)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, expandAlignment); currentFileSize += len(partsData)
            partsInfoCap2Data += partsInfoData
            data += partsData

        partsMaxSizeFaceline = 0
        partsInfoFacelineData = bytearray()
        for partsInfo, shape in self.partsInfoFaceline:
            partsData = FFLiResourceShapeDataHeader.save(shape, isExpand)
            partsMaxSizeFaceline = max(len(partsData), partsMaxSizeFaceline)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, expandAlignment); currentFileSize += len(partsData)
            partsInfoFacelineData += partsInfoData
            data += partsData

        partsMaxSizeGlass = 0
        partsInfoGlassData = bytearray()
        for partsInfo, shape in self.partsInfoGlass:
            partsData = FFLiResourceShapeDataHeader.save(shape, isExpand)
            partsMaxSizeGlass = max(len(partsData), partsMaxSizeGlass)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, expandAlignment); currentFileSize += len(partsData)
            partsInfoGlassData += partsInfoData
            data += partsData

        partsMaxSizeMask = 0
        partsInfoMaskData = bytearray()
        for partsInfo, shape in self.partsInfoMask:
            partsData = FFLiResourceShapeDataHeader.save(shape, isExpand)
            partsMaxSizeMask = max(len(partsData), partsMaxSizeMask)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, expandAlignment); currentFileSize += len(partsData)
            partsInfoMaskData += partsInfoData
            data += partsData

        partsMaxSizeNoseline = 0
        partsInfoNoselineData = bytearray()
        for partsInfo, shape in self.partsInfoNoseline:
            partsData = FFLiResourceShapeDataHeader.save(shape, isExpand)
            partsMaxSizeNoseline = max(len(partsData), partsMaxSizeNoseline)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, expandAlignment); currentFileSize += len(partsData)
            partsInfoNoselineData += partsInfoData
            data += partsData

        partsMaxSizeNose = 0
        partsInfoNoseData = bytearray()
        for partsInfo, shape in self.partsInfoNose:
            partsData = FFLiResourceShapeDataHeader.save(shape, isExpand)
            partsMaxSizeNose = max(len(partsData), partsMaxSizeNose)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, expandAlignment); currentFileSize += len(partsData)
            partsInfoNoseData += partsInfoData
            data += partsData

        partsMaxSizeHair1 = 0
        partsInfoHair1Data = bytearray()
        for partsInfo, shape in self.partsInfoHair1:
            partsData = FFLiResourceShapeDataHeader.save(shape, isExpand)
            partsMaxSizeHair1 = max(len(partsData), partsMaxSizeHair1)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, expandAlignment); currentFileSize += len(partsData)
            partsInfoHair1Data += partsInfoData
            data += partsData

        partsMaxSizeHair2 = 0
        partsInfoHair2Data = bytearray()
        for partsInfo, shape in self.partsInfoHair2:
            partsData = FFLiResourceShapeDataHeader.save(shape, isExpand)
            partsMaxSizeHair2 = max(len(partsData), partsMaxSizeHair2)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, expandAlignment); currentFileSize += len(partsData)
            partsInfoHair2Data += partsInfoData
            data += partsData

        partsMaxSizeForehead1 = 0
        partsInfoForehead1Data = bytearray()
        for partsInfo, shape in self.partsInfoForehead1:
            partsData = FFLiResourceShapeDataHeader.save(shape, isExpand)
            partsMaxSizeForehead1 = max(len(partsData), partsMaxSizeForehead1)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, expandAlignment); currentFileSize += len(partsData)
            partsInfoForehead1Data += partsInfoData
            data += partsData

        partsMaxSizeForehead2 = 0
        partsInfoForehead2Data = bytearray()
        for partsInfo, shape in self.partsInfoForehead2:
            partsData = FFLiResourceShapeDataHeader.save(shape, isExpand)
            partsMaxSizeForehead2 = max(len(partsData), partsMaxSizeForehead2)
            partsInfoData, partsData = partsInfo.save(partsData, currentFileSize, isExpand, expandAlignment); currentFileSize += len(partsData)
            partsInfoForehead2Data += partsInfoData
            data += partsData

        headerData = struct.pack(
            self._format,
            partsMaxSizeBeard,
            partsMaxSizeCap1,
            partsMaxSizeCap2,
            partsMaxSizeFaceline,
            partsMaxSizeGlass,
            partsMaxSizeMask,
            partsMaxSizeNoseline,
            partsMaxSizeNose,
            partsMaxSizeHair1,
            partsMaxSizeHair2,
            partsMaxSizeForehead1,
            partsMaxSizeForehead2,
            partsInfoBeardData,
            partsInfoCap1Data,
            partsInfoCap2Data,
            partsInfoFacelineData,
            partsInfoGlassData,
            partsInfoMaskData,
            partsInfoNoselineData,
            partsInfoNoseData,
            partsInfoHair1Data,
            partsInfoHair2Data,
            partsInfoForehead1Data,
            partsInfoForehead2Data
        )

        return headerData, data

    def export(self, path=''):
        pathIsCwd = not path or path == '.'

        if not pathIsCwd:
            cwd = os.getcwd()
            os.chdir(path)

        for partsInfoArray, name in (
            (self.partsInfoBeard, "Beard"),
            (self.partsInfoCap1, "Cap1"),
            (self.partsInfoCap2, "Cap2"),
            (self.partsInfoFaceline, "Faceline"),
            (self.partsInfoGlass, "Glass"),
            (self.partsInfoMask, "Mask"),
            (self.partsInfoNoseline, "Noseline"),
            (self.partsInfoNose, "Nose"),
            (self.partsInfoHair1, "Hair1"),
            (self.partsInfoHair2, "Hair2"),
            (self.partsInfoForehead1, "Forehead1"),
            (self.partsInfoForehead2, "Forehead2")
        ):
            for i, (partsInfo, shape) in enumerate(partsInfoArray):
                if shape is None:
                    continue

                filename = '%s_%d' % (name, i)

                json_dict = {
                    "partsInfo": {
                        "compressLevel": partsInfo.compressLevel,
                        "windowBits": partsInfo.windowBits,
                        "memoryLevel": partsInfo.memoryLevel,
                        "strategy": partsInfo.strategy
                    },
                    "shape": {
                        "transform": shape.transform
                    }
                }

                glb = FFLiResourceShapeDataHeader.export(shape)
                if glb is not None:
                    glb_filename = filename + '.glb'
                    json_dict["shape"]["filename"] = glb_filename
                    glb.save(glb_filename)

                json_filename = filename + '.json'
                with open(json_filename, 'w') as outf:
                    json.dump(json_dict, outf, indent=2, sort_keys=True)

        if not pathIsCwd:
            os.chdir(cwd)

    def importFromPath(self, path=''):
        pathIsCwd = not path or path == '.'

        if not pathIsCwd:
            cwd = os.getcwd()
            os.chdir(path)

        for count, name in (
            (  4, "Beard"),
            (132, "Cap1"),
            (132, "Cap2"),
            ( 12, "Faceline"),
            (  1, "Glass"),
            ( 12, "Mask"),
            ( 18, "Noseline"),
            ( 18, "Nose"),
            (132, "Hair1"),
            (132, "Hair2"),
            (132, "Forehead1"),
            (132, "Forehead2")
        ):
            partsInfoArray = []

            for i in range(count):
                partsInfo = FFLiResourcePartsInfo()

                filename = '%s_%d' % (name, i)
                json_filename = filename + '.json'
                if not os.path.isfile(json_filename):
                    partsInfoArray.append([partsInfo, None])
                    continue

                with open(json_filename, 'r') as inf:
                    json_dict = json.load(inf)

                partsInfo.compressLevel = json_dict["partsInfo"]["compressLevel"]
                partsInfo.windowBits = json_dict["partsInfo"]["windowBits"]
                partsInfo.memoryLevel = json_dict["partsInfo"]["memoryLevel"]
                partsInfo.strategy = json_dict["partsInfo"]["strategy"]

                shape = FFLiResourceShapeDataHeader()

                if "filename" in json_dict["shape"]:
                    glb_filename = json_dict["shape"]["filename"]
                    glb = pygltflib.GLTF2().load(glb_filename)
                    FFLiResourceShapeDataHeader.importFromGLB(shape, glb)
                else:
                    shape.boundingBox = [[0.0, 0.0, 0.0], [0.0, 0.0, 0.0]]
                    shape.transform = None

                    shape.position = None
                    shape.normal = None
                    shape.texCoord = None
                    shape.tangent = None
                    shape.color = None
                    shape.index = None

                shape.transform = json_dict["shape"]["transform"]

                partsInfoArray.append([partsInfo, shape])

            exec("self.partsInfo%s = partsInfoArray" % name)

        if not pathIsCwd:
            os.chdir(cwd)

    def compare(self, other):
        for partsInfoArrayA, partsInfoArrayB, name in (
            (self.partsInfoBeard, other.partsInfoBeard, "Beard"),
            (self.partsInfoCap1, other.partsInfoCap1, "Cap1"),
            (self.partsInfoCap2, other.partsInfoCap2, "Cap2"),
            (self.partsInfoFaceline, other.partsInfoFaceline, "Faceline"),
            (self.partsInfoGlass, other.partsInfoGlass, "Glass"),
            (self.partsInfoMask, other.partsInfoMask, "Mask"),
            (self.partsInfoNoseline, other.partsInfoNoseline, "Noseline"),
            (self.partsInfoNose, other.partsInfoNose, "Nose"),
            (self.partsInfoHair1, other.partsInfoHair1, "Hair1"),
            (self.partsInfoHair2, other.partsInfoHair2, "Hair2"),
            (self.partsInfoForehead1, other.partsInfoForehead1, "Forehead1"),
            (self.partsInfoForehead2, other.partsInfoForehead2, "Forehead2")
        ):
            for i, ((partsInfoA, shapeA), (partsInfoB, shapeB)) in enumerate(zip(partsInfoArrayA, partsInfoArrayB)):
                label = "FFLiResourceShapeHeader.%s.%d" % (name, i)
                if shapeA is None:
                    if shapeB is not None:
                        print(label + ": A is None")
                    continue
                elif shapeB is None:
                    print(label + ": B is None")
                    continue

                partsInfoA.compare(partsInfoB, label)
                FFLiResourceShapeDataHeader.compare(shapeA, shapeB, label)


class FFLiResourceHeader:
    _format = '>4s2I4xI%ds%ds48x' % (FFLiResourceTextureHeader.size, FFLiResourceShapeHeader.size)
    size = struct.calcsize(_format)
    assert size == 0x4A00

    def load(self, data, pos=0):
        (magic,
         version,
         self.uncompressBufferSize,
         isExpand,
         textureHeaderData,
         shapeHeaderData) = struct.unpack_from(self._format, data, pos)

        assert magic == b'FFRA'
        assert version == 0x00070000

        self.isExpand = isExpand == 1

        self.textureHeader = FFLiResourceTextureHeader()
        self.textureHeader.load(textureHeaderData, data, pos, self.isExpand)

        self.shapeHeader = FFLiResourceShapeHeader()
        self.shapeHeader.load(shapeHeaderData, data, pos, self.isExpand)

    def save(self):
        fileSize = FFLiResourceHeader.size

        textureHeaderData, textureData = self.textureHeader.save(fileSize, self.isExpand); fileSize += len(textureData)
        shapeHeaderData, shapeData = self.shapeHeader.save(fileSize, self.isExpand); fileSize += len(shapeData)

        headerData = struct.pack(
            self._format,
            b'FFRA',
            0x00070000,
            self.uncompressBufferSize,
            int(self.isExpand),
            textureHeaderData,
            shapeHeaderData
        )

        data = b''.join([headerData, textureData, shapeData])
        assert len(data) == fileSize

        return data

    def export(self, name, path='', asPng=True):
        # Make sure no funny business is happening
        path = os.path.join(path, os.path.dirname(name))
        name = os.path.basename(name)
        if name.endswith(".dat"):
            name = name[:-4]  # 4 == len(".dat")
        elif name.endswith(".json"):
            name = name[:-5]  # 5 == len(".json")

        pathIsCwd = not path or path == '.'

        if not pathIsCwd:
            cwd = os.getcwd()
            os.chdir(path)

        texturePath = name + "_texture"
        shapePath = name + "_shape"

        if not os.path.isdir(texturePath):
            os.mkdir(texturePath)

        if not os.path.isdir(shapePath):
            os.mkdir(shapePath)

        json_dict = {
            "uncompressBufferSize": self.uncompressBufferSize,
            "isExpand": int(self.isExpand),
            "texturePath": texturePath,
            "shapePath": shapePath
        }

        json_filename = name + '.json'
        with open(json_filename, 'w') as outf:
            json.dump(json_dict, outf, indent=2, sort_keys=True)

        self.textureHeader.export(texturePath, asPng)
        self.shapeHeader.export(shapePath)

        if not pathIsCwd:
            os.chdir(cwd)

    def importFromPath(self, name, path=''):
        # Make sure no funny business is happening
        path = os.path.join(path, os.path.dirname(name))
        name = os.path.basename(name)
        if name.endswith(".json"):
            json_filename = name
        elif name.endswith(".dat"):
            json_filename = name[:-4] + ".json"  # 4 == len(".dat")
        else:
            json_filename = name + ".json"

        pathIsCwd = not path or path == '.'

        if not pathIsCwd:
            cwd = os.getcwd()
            os.chdir(path)

        with open(json_filename, 'r') as inf:
            json_dict = json.load(inf)

        self.uncompressBufferSize = json_dict["uncompressBufferSize"]
        self.isExpand = json_dict["isExpand"] == 1
        texturePath = json_dict["texturePath"]
        shapePath = json_dict["shapePath"]

        self.textureHeader = FFLiResourceTextureHeader()
        self.textureHeader.importFromPath(texturePath)

        self.shapeHeader = FFLiResourceShapeHeader()
        self.shapeHeader.importFromPath(shapePath)

        if not pathIsCwd:
            os.chdir(cwd)

    def compare(self, other):
        if self.uncompressBufferSize != other.uncompressBufferSize:
            print("FFLiResourceHeader.UncompressBufferSize:", self.uncompressBufferSize, other.uncompressBufferSize)

        if self.isExpand != other.isExpand:
            print("FFLiResourceHeader.IsExpand:", self.isExpand, other.isExpand)

        self.textureHeader.compare(other.textureHeader)
        self.shapeHeader.compare(other.shapeHeader)


def help():
    appname = sys.argv[0]
    if appname.endswith(".py"):
        appname = "python " + appname

    print("Usage:")
    print(appname + " -fromJSON <json filename> <dat filename>")
    print(appname + " -toJSON [-PNG] <dat filename> <json filename>")


def main():
    if len(sys.argv) < 4:
        help()
        sys.exit(1)

    if sys.argv[1].lower() == "-fromjson":
        filename = sys.argv[2]
        out_filename = sys.argv[3]

        if not os.path.isfile(filename):
            print("Input file not found:", filename)
            sys.exit(1)

        header = FFLiResourceHeader()
        header.importFromPath(filename)
        outb = header.save()

        with open(out_filename, "wb") as outf:
            outf.write(outb)

    elif sys.argv[1].lower() == "-tojson":
        asPng = False
        if len(sys.argv) > 5:
            help()
            sys.exit(1)

        if len(sys.argv) == 5:
            if sys.argv[2].lower() != "-png":
                help()
                sys.exit(1)

            asPng = True
            filename = sys.argv[3]
            out_filename = sys.argv[4]

        else:
            filename = sys.argv[2]
            out_filename = sys.argv[3]

        if not os.path.isfile(filename):
            print("Input file not found:", filename)
            sys.exit(1)

        with open(filename, "rb") as inf:
            inb = inf.read()

        header = FFLiResourceHeader()
        header.load(inb)
        header.export(out_filename, asPng=asPng)

    else:
        help()
        sys.exit(1)



if __name__ == "__main__":
    main()
