// filename : TGAImage.h
// author   : Jaejun Jang
// email    : jjj404001@gmail.com
// brief    : Image file structure for TGA image format.

// TGA format details:
// http://tfc.duke.free.fr/coding/tga_specs.pdf

class TGAImage
{
    // header : 
    // ID Length 1 byte
    // Color Map Type 1 byte
    // Image Type 1 byte
    // Color Map Specification 5 bytes
    // - color map origin 2 bytes
    // - color map length 2 bytes
    // Color map deph 1 byte
    // Image Specification 10 bytes
    // - x origin 2 bytes
    // - y origin 2 bytes
    // - width 2 bytes
    // - height 2 bytes
    // - bits per pixel 1 byte
    // - image descriptor 1 byte
};