#ifndef IMAGE_PROCESSOR_IMPROC_HPP
#define IMAGE_PROCESSOR_IMPROC_HPP

#include "bitmap.h"

#include <vector>
#include <cstring>
#include <memory>
#include <string>
#include <functional>


#define ARRAY_2D 1
#define VECTOR_OF_VECTORS 2
#define MATRIX_DATA_TYPE ARRAY_2D


template<typename T>
#if MATRIX_DATA_TYPE == ARRAY_2D

using MatrixData = T**;

#else

using MatrixData = std::vector<std::vector<T>>;

#endif


template<typename  T>
class Matrix{
public:
#if MATRIX_DATA_TYPE == VECTOR_OF_VECTORS
    Matrix( std::size_t r, std::size_t c, T v) : _nrows(r), _ncols(c), _matrix(std::vector(r, std::vector(c, v))) {};
    Matrix( const Matrix& ref) : _nrows(ref.get_nrows()), _ncols(ref.get_ncols()),  _matrix(MatrixData<T>()) {
        for(std::size_t i = 0; i < _nrows; i++) {
            _matrix.push_back(std::vector<T>());
            _matrix[i] = ref[i]; //FIXME preferably std::copy
        }
    };
    std::size_t get_nrows() const {return _nrows; }
    std::size_t get_ncols() const {return _ncols;}
    typename MatrixData<T>::reference operator[](std::size_t pos) { return _matrix[pos]; }
    typename MatrixData<T>::const_reference operator[](std::size_t pos) const { return _matrix[pos]; }
    ~Matrix() = default;
    Matrix(std::size_t r, std::size_t c, MatrixData<T> ins) : _nrows(r), _ncols(c), _matrix(ins) {}

#else

    Matrix(std::size_t r, std::size_t c, T v) : _nrows(r), _ncols(c) {
        _matrix = new T*[_nrows];
        for(std::size_t i = 0; i < _nrows; i++){
            _matrix[i] = new T[_ncols];
            std::fill_n(_matrix[i], _ncols, v);
        }

    };
    T* operator[](std::size_t pos) { return _matrix[pos]; } //FIXME apparently have to use typenames and shit
    const T* operator[](std::size_t pos) const { return _matrix[pos]; }
    Matrix(const Matrix& ref) : _nrows(ref.get_nrows()), _ncols(ref.get_ncols()) {
        _matrix = new T*[_nrows];
        for(std::size_t i = 0; i < _nrows; i++){
            _matrix[i] = new T[_ncols];
            std::copy(ref[i], ref[i] + _ncols, _matrix[i]);
        }
    }//TODO copy constructor FIXME fix matrix data declaration




    std::size_t get_nrows() const {return _nrows; }
    std::size_t get_ncols() const {return _ncols; }
    ~Matrix() {
        if(_matrix != nullptr){
            for(std::size_t i = 0; i < _nrows; i++){
                delete _matrix[i];
            }
            delete _matrix;
        }
    }

    Matrix(std::size_t r, std::size_t c, MatrixData<T> ins) : Matrix(r, c, (T) 0) {
        for(std::size_t r_ = 0; r_ < _nrows; r_++){
            for(std::size_t c_ = 0; c_ < _ncols; c_++){
                _matrix[r_][c_] = ins[r_][c_];
            }
        }
    }

#endif

    MatrixData<T> get_matrix() const {return _matrix; }



private:
    std::size_t _nrows;
    std::size_t _ncols;
    #if MATRIX_DATA_TYPE == VECTOR_OF_VECTORS
    // different data declaration for vector of vectors
    std::vector<std::vector<T>> _matrix;
    #else
    T** _matrix;
    #endif
};




//FIXME not sure if works here or in cpp implement
struct BmpInfoDeleter {
    void operator()(BITMAPINFO* ptr) { delete[] ptr; }
};

using BmpInfoUniquePtr = std::unique_ptr<BITMAPINFO, BmpInfoDeleter>;


class Image : public Matrix<byte>{
private:
    BmpInfoUniquePtr _bmp_inf;
    BITMAPFILEHEADER _bmp_hdr;
public:

    BmpInfoUniquePtr copy_bitmapinfo(BITMAPINFO* bmi, BITMAPFILEHEADER hdr) {
    if(bmi == nullptr){
        return nullptr;
    }
    #if (WIN32)
            std::size_t size_b = hdr.bfOffBits - sizeof(BITMAPFILEHEADER);
    #else
            std::size_t size_b = hdr.bfOffBits - 18;
    #endif
        BITMAPINFO* ptr = reinterpret_cast<BITMAPINFO*>(new char[size_b]);
        std::memcpy(ptr, bmi, size_b);
        BmpInfoUniquePtr u_ptr(ptr);
        return u_ptr;
    };

    Image(std::size_t h, std::size_t w, BITMAPINFO* bmi, BITMAPFILEHEADER hdr) : Matrix(h, w, (byte) 0), _bmp_inf(copy_bitmapinfo(bmi, hdr)), _bmp_hdr(hdr) {};
    Image(std::size_t h, std::size_t w, BITMAPINFO* bmi, BITMAPFILEHEADER hdr, MatrixData<byte> data) : Matrix(h, w, data), _bmp_inf(bmi), _bmp_hdr(hdr){};

    Image(const Image& ref) : Matrix<byte>(ref.get_nrows(), ref.get_ncols(), ref.get_matrix()), _bmp_hdr(ref.get_bitmapheader()){
        _bmp_inf = copy_bitmapinfo(ref.get_bitmapinfo(), _bmp_hdr);
    };


    const BITMAPFILEHEADER& get_bitmapheader() const { return _bmp_hdr; }
    BITMAPINFO* get_bitmapinfo() const { return _bmp_inf.get(); }
    BITMAPINFO* get_bitmapinfo() { return _bmp_inf.get(); }
    ~Image() = default;
};


enum ImagePrintMode { CHARS, NUMS};

std::string to_string(const Image& im, ImagePrintMode mode);


using Mask = Matrix<double>;

Image transform(const Image& im_in, std::function<byte(byte)> func);

Mask get_averaging_mask(std::size_t n);

Image filter(const Image& im_in, const Mask& mask);


/**
 * Load a BMP file from the disk.
 *
 * @param filepath File to load.
 * @return The loaded image.
 */
extern byte** load_bitmap_r(const char* filepath, BITMAPINFO **BitmapInfo);

extern Image load_bitmap(const std::string& filepath);

/**
 * Save a BMP file to disk.
 *
 * @param filepath File to save.
 * @param image Image to save.
 * @return EXIT_SUCCESS on success or EXIT_FAILURE on failure.
 */
extern int save_bitmap_r(const char* filepath, byte** image, BITMAPINFO* BitmapInfo);

extern int save_bitmap(const std::string& filename, const Image& image);

#endif //IMAGE_PROCESSOR_IMPROC_HPP
