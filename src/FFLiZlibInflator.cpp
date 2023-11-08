#include <nn/ffl/FFLiZlibInflator.h>

#include <misc/rio_MemUtil.h>

FFLiZlibInflator::FFLiZlibInflator(s32 windowBits)
    : m_IsStreamEnd(false)
{
    rio::MemUtil::set(&m_Stream, 0, sizeof(z_stream));
    inflateInit2(&m_Stream, windowBits);
}

FFLiZlibInflator::~FFLiZlibInflator()
{
    inflateEnd(&m_Stream);
}

s32 FFLiZlibInflator::Process(void** ppDst, u32* pDstSize, const void** ppSrc, u32* pSrcSize, s32 flush)
{
    m_Stream.next_out = (Bytef*)*ppDst;
    m_Stream.avail_out = *pDstSize;
    m_Stream.next_in = (Bytef*)*ppSrc;
    m_Stream.avail_in = *pSrcSize;

    s32 ret = inflate(&m_Stream, flush);
    if (ret == Z_STREAM_END)
        m_IsStreamEnd = true;

    *ppDst = m_Stream.next_out;
    *pDstSize = m_Stream.avail_out;
    *ppSrc = m_Stream.next_in;
    *pSrcSize = m_Stream.avail_in;

    return ret;
}
