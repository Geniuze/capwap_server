#ifndef _BUFFER_H_
#define _BUFFER_H_

#define SAFE_FREE(buf) \
    if(buf) {             \
        free(buf);        \
        buf = NULL;       \
    }

#define SAFE_DELETE(p) \
    if (p) { \
        delete p;         \
        p = NULL;        \
    }

#define BUFFER_DEFAULT_SIZE 1024

class CBuffer
{
public:
    CBuffer(bool owner)
    {
        _owner = owner;
        _buffer = NULL;
        _offset = 0;
        _size = 0;
        _length = 0;
    }
    CBuffer()
    {
        _owner = true;
        _buffer = NULL;
        _offset = 0;
        _size = 0;
        _length = 0;
    }
    ~CBuffer(){
        if (_owner) SAFE_FREE(_buffer);
    }

    uint8_t *GetPtr()
    {
        return _buffer + _offset;
    }

    void retrive8(uint8_t &value)
    {
        value = *GetPtr();
        _offset += 1;
    }
    void retrive16(uint16_t &value)
    {
        value = ntohs(*((uint16_t *)GetPtr()));
        _offset += 2;
    }
    void retrive32(uint32_t &value)
    {
        value = ntohl(*((uint32_t*)GetPtr()));
        _offset += 4;
    }
    void retrive64(uint64_t &value)
    {
        //value = ntohll(((uint64_t*)GetPtr()));
        _offset += 8;
    }
    void retriverawbytes(uint8_t *buf, size_t len)
    {
        if (buf) memcpy(buf, GetPtr(), len);
        _offset += len;
    }

    void store8(uint8_t value)
    {
        *GetPtr() = value;
        _offset += 1;
        _length += 1;
    }
    void store16(uint16_t value)
    {
        *((uint16_t *)GetPtr()) = htons(value);
        _offset += 2;
        _length += 2;
    }
    void store32(uint32_t value)
    {
        *((uint32_t *)GetPtr()) = htonl(value);
        _offset += 4;
        _length += 4;
    }
    void store64(uint64_t value)
    {
        //*((uint64_t *)GetPtr()) = htonll(value);
        _offset += 8;
        _length += 8;
    }
    void storerawbytes(uint8_t *buf, size_t len)
    {
        if(buf) memcpy(GetPtr(), buf, len);
        _offset += len;
        _length += len;
    }

    int toBuffer(char *buf, size_t len)
    {
        if (_owner)
        {
            if (_buffer) SAFE_FREE(_buffer);
            _buffer = (uint8_t *)malloc(len);
            if (NULL == _buffer)
            {
                dlog(LOG_ERR, "%s.%d malloc error %s", __func__, __LINE__, strerror(errno));
                return 1;
            }
            memcpy(_buffer, buf, len);

            _length = len;
            _size = len;
            //_offset = 0;

            dlog(LOG_DEBUG, "%s.%d bufferptr %#x len %d offset %d ", __func__, __LINE__, (int)_buffer, _length, _offset);
        }
        else
        {
            _buffer = (uint8_t *)buf;
            _length = len;
        }
        return 0;
    }
    int revert(size_t len)
    {
        if (_offset >= len)
        {
            _offset -= len;
            return len;
        }
        else
        {
            return 0;
        }
    }
    int forward(size_t len)
    {
        _offset += len;
        return len;
    }
    uint32_t GetOffset()
    {
        return _offset;
    }
    uint8_t * GetBuffer()
    {
        return _buffer;
    }
    uint32_t GetLength()
    {
        return _length;
    }
    int extend(size_t size = BUFFER_DEFAULT_SIZE)
    {
        uint8_t *buffer = (uint8_t*)malloc(size);
        if (NULL == buffer)
        {
            dlog(LOG_ERR, "%s.%d malloc error %s", __func__, __LINE__, strerror(errno));
            return -1;
        }
        if (_buffer) memcpy(buffer, _buffer, (size>_offset)?_offset:size);

        SAFE_FREE(_buffer);
        _buffer = buffer;
        return 0;
    }
private:
    uint8_t *_buffer;
    uint32_t _offset;
    uint32_t _size;
    uint32_t _length;
    bool     _owner;
};

#endif
