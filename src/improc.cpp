#include "../include/improc.hpp"
#include "../include/bitmap.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <exception>

//TODO error class that inherits from runtime_error and calls its constructor;
class FileIOError : public std::runtime_error{
public:
    FileIOError() : std::runtime_error("error when loading file!") {
        //FIXME add the required function override
    }
};

//this shit actually writes to the passed pointers FIXME the wrapper function won't work on the vector implementation so TODO refactor
byte** load_bitmap_r(const char* filepath, BITMAPINFO **BitmapInfo) {
    BITMAPFILEHEADER header;
    byte* bitmapBytes = LoadDIBitmap(filepath, BitmapInfo, &header);

    if (!bitmapBytes) {
        // Error when reading the input file.
        exit(-1);
    }

    const size_t h = (size_t) (*BitmapInfo)->bmiHeader.biHeight;
    const size_t w = (size_t) (*BitmapInfo)->bmiHeader.biWidth;
    const size_t bits_per_pixel = (*BitmapInfo)->bmiHeader.biBitCount;

    /* see: https://en.wikipedia.org/wiki/BMP_file_format#Pixel_storage */
    size_t row_size = (bits_per_pixel * w + 31) / 32 * 4;
#if (WIN32)
    printf("Successfully loaded a %llux%llu image - %s.\n\n", h, w, filepath);
#else
    printf("Successfully loaded a %lux%lu image - %s.\n\n", h, w, filepath);
#endif

    byte** image_array = (byte**) malloc(sizeof(byte*) * h);
    for (size_t i = 0; i < h; i++) {
        image_array[i] = (byte*) malloc(sizeof(byte) * w);
        memset(image_array[i], 0, w);
    }

    byte* reader = bitmapBytes;

    // The order of the pixels in BMP file is as follows: from left to right, from bottom to top (first pixel is from
    // lower left corner of the picture).
    for (size_t i = 0; i < h; ++i) {
        /* Copy values of pixels in an image row. */
        for (size_t j = 0; j < w; ++j) {
            image_array[h - i - 1][j] = *reader;
            ++reader;
        }

        /* Skip padding bytes. */
        reader += row_size - w;
    }

    free(bitmapBytes);

    return image_array;
}

Image load_bitmap(const std::string& filepath){
    BITMAPINFO* inf = new BITMAPINFO();
    BITMAPFILEHEADER header;

    byte* bitmapBytes = LoadDIBitmap(filepath.c_str(), &inf, &header);

    if (!bitmapBytes) {
        // Error when reading the input file.
        throw FileIOError();
    }

    const size_t h = (size_t) (inf)->bmiHeader.biHeight;
    const size_t w = (size_t) (inf)->bmiHeader.biWidth;
    const size_t bits_per_pixel = (inf)->bmiHeader.biBitCount;

    Image return_image(h, w, inf, header);

    /* see: https://en.wikipedia.org/wiki/BMP_file_format#Pixel_storage */
    size_t row_size = (bits_per_pixel * w + 31) / 32 * 4;
#if (WIN32)
    printf("Successfully loaded a %llux%llu image - %s.\n\n", h, w, filepath.c_str());
#else
    printf("Successfully loaded a %lux%lu image - %s.\n\n", h, w, filepath);
#endif

//    byte** image_array = (byte**) malloc(sizeof(byte*) * h);
//    for (size_t i = 0; i < h; i++) {
//        image_array[i] = (byte*) malloc(sizeof(byte) * w);
//        memset(image_array[i], 0, w);
//    }         //dont need that - will allocate straight to matrix

    byte* reader = bitmapBytes;

    // The order of the pixels in BMP file is as follows: from left to right, from bottom to top (first pixel is from
    // lower left corner of the picture).
    for (size_t i = 0; i < h; ++i) {
        /* Copy values of pixels in an image row. */
        for (size_t j = 0; j < w; ++j) {
            return_image[h - i - 1][j] = (byte) *reader; //FIXME values are right but it doesnt copy???
            ++reader;
        }

        /* Skip padding bytes. */
        reader += row_size - w;
    }

    free(bitmapBytes);

    return return_image;
}

int save_bitmap_r(const char* filepath, byte** image, BITMAPINFO* BitmapInfo) {

    const size_t h = (size_t) BitmapInfo->bmiHeader.biWidth;
    const size_t w = (size_t) BitmapInfo->bmiHeader.biHeight;
    const size_t bits_per_pixel = BitmapInfo->bmiHeader.biBitCount;
    const size_t row_size = (bits_per_pixel * w + 31) / 32 * 4;

    byte* bitmapBytes = (byte*) malloc(sizeof(byte) * BitmapInfo->bmiHeader.biSizeImage);
    byte* writer = bitmapBytes;

    const size_t padding = row_size - w;

    for (size_t i = 0; i < h; ++i) {

        /* Przepisz wartosci pikseli wiersza obrazu. */
        for (size_t j = 0; j < w; j++) {
            *writer++ = image[h - i - 1][j];
        }

        /* Ustaw bajty wyrownania. */
        for (size_t j = 0; j < padding; j++) {
            *writer++ = 0;
        }
    }

    int status = SaveDIBitmap(filepath, BitmapInfo, bitmapBytes);
    free(bitmapBytes);

    return status;
}

extern int save_bitmap(const std::string& filename, const Image& image){
    const size_t h = (size_t) (image.get_bitmapinfo())->bmiHeader.biWidth;
    const size_t w = (size_t) (image.get_bitmapinfo())->bmiHeader.biHeight;
    const size_t bits_per_pixel = (image.get_bitmapinfo())->bmiHeader.biBitCount;
    const size_t row_size = (bits_per_pixel * w + 31) / 32 * 4;

    byte* bitmapBytes = (byte*) malloc(sizeof(byte) * (image.get_bitmapinfo())->bmiHeader.biSizeImage);
    byte* writer = bitmapBytes;

    const size_t padding = row_size - w;

    for (size_t i = 0; i < h; ++i) {

        /* Przepisz wartosci pikseli wiersza obrazu. */
        for (size_t j = 0; j < w; j++) {
            *writer++ = image[h - i - 1][j];
        }

        /* Ustaw bajty wyrownania. */
        for (size_t j = 0; j < padding; j++) {
            *writer++ = 0;
        }
    }

    int status = SaveDIBitmap(filename.c_str(), image.get_bitmapinfo(), bitmapBytes);
    free(bitmapBytes);

    return status;
}

std::string to_string(const Image& im, ImagePrintMode mode){
    std::ostringstream out;
    for(std::size_t r = 0; r < im.get_nrows(); r++){
        for(std::size_t c = 0; c < im.get_ncols(); c++){
            if(mode == CHARS){
                out << (char) im[r][c];
            }else{
                out << std::setw(4) << (int) im[r][c];
            }
        }
        if(r < im.get_nrows() -1){
            out << std::endl;
        }
    }
    return out.str();
}

Image transform(const Image& im_in, std::function<byte(byte)> func){
    Image processed(im_in);
    for(std::size_t r = 0; r < processed.get_nrows(); r++){
        for(std::size_t c = 0; c < processed.get_ncols(); c++) {
            processed[r][c] = func(processed[r][c]);
        }
    }
    return processed;
}

Mask get_averaging_mask(std::size_t n){
    return Mask(n, n, (int) (1 / (n * n)));
}

Image filter(const Image& im_in, const Mask& mask){
    Image processed(im_in);
    std::size_t mask_range = (mask.get_ncols() - 1) / 2;
    for(std::size_t r = 0; r < processed.get_nrows(); r++){
        for(std::size_t c = 0; c < processed.get_ncols(); c++){
                byte sum = 0;//FIXME
                std::size_t mr = 0;
                std::size_t mc = 0;
                for(std::size_t r_m = r; r_m <= r + mask.get_nrows(); r_m++){
                    for(std::size_t c_m = c; c_m <= c + mask.get_ncols(); c_m++){
                        std::size_t pixel_value;
                        if(r_m < mask_range || r_m - mask_range <= im_in.get_nrows() || c_m < mask_range || c_m - mask_range <= im_in.get_ncols()) {
                            pixel_value = 0;
                        }else {
                            pixel_value = im_in[r + r_m - mask_range][c + c_m - mask_range];
                        }

                        sum += (byte) ((double) pixel_value * mask[mr++][mc++]);
                    }
                }
                processed[r][c] = sum;
            }
        }
    return processed;
}
