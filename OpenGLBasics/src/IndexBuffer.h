#pragma once

//NOTE: size => in bytes
//      count => in number of elements

class IndexBuffer {
    private:
    unsigned int rendererId;
    unsigned int count;

    public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    inline unsigned int getCount() const { return count; }

    void bind() const;
    void unbind() const;
};