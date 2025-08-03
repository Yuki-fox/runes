#include <squirrel\squirrel.h>
#include <sq_stdio.h>
#include "cImageBMP.h"

#if IMAGELIB_SUPPORT_BMP

//#define SQ_SEEK_CUR 0
//#define SQ_SEEK_END 1
//#define SQ_SEEK_SET 2

bool cImageBMP::Decode( SQStream* file )
{


	if ( file == NULL ) return false;

	BITMAPFILEHEADER bf;

	file->Seek(0,SQ_SEEK_SET);
	file->Read( &bf, min(14, sizeof(bf)));

	if ( bf.bfType != BFT_BITMAP ) 
		return false;			// 檢查是否為BMP格式


	// 圖形資料
	BITMAPINFOHEADER info;
	file->Read( &info, sizeof(info) );
	

	if ( info.biCompression != BI_RGB ) 
		return false;		// 暫時不直援RLE壓縮

	if ( !Create( info.biWidth, info.biHeight, info.biBitCount, IMAGELIB_FORMAT_BMP ) ) 
	{
		return false;
	}

	switch ( info.biBitCount )
	{		
		// 24Bits Color 像素排列為BGR,BGR,BGR...
	case 24:
		{
			int lineBytes = ALIGN_LONG(info.biWidth * 3);
			BYTE *pTmp = new BYTE[lineBytes * m_height];

			if ( bf.bfOffBits != 0 ) 
				file->Seek(  bf.bfOffBits, SQ_SEEK_SET );

			file->Read( pTmp, lineBytes * m_height );
			
			m_pixelFormat = PF_R8G8B8;
			for ( int yy = 0; yy < m_height; yy++ ) 
			{
				memcpy( m_pPixels + (yy * m_width * 3), pTmp + (m_height - yy - 1) * lineBytes, m_width * 3 );
			}
			delete [] pTmp;
			break;
		}
	case 32:
		{
			int lineBytesD = ALIGN_LONG(info.biWidth * 4);
			//int lineBytesS = ALIGN_LONG(info.biWidth * 4);
			BYTE *pTmp = new BYTE[lineBytesD * m_height];

			if ( bf.bfOffBits != 0 ) 
				file->Seek(  bf.bfOffBits, SQ_SEEK_SET );

			file->Read( pTmp, lineBytesD * m_height );

			m_pixelFormat = PF_A8R8G8B8;

			for ( int yy = 0; yy < m_height; yy++ ) 
			{
				memcpy( m_pPixels + (yy * lineBytesD), pTmp + (m_height - yy - 1) * lineBytesD,lineBytesD );
				
			}
			delete [] pTmp;
			break;
		}
	case 8:
	case 4:
	case 1:
		{
			int lineBytes = ALIGN_LONG( info.biWidth / (8 / info.biBitCount) );
			BYTE *pTmp = new BYTE[lineBytes * m_height];
			m_paletteCount = info.biClrUsed;

			file->Read( m_pPalette,  info.biClrUsed* sizeof(RGBQUAD));
		

			if ( bf.bfOffBits != 0 ) 
					file->	Seek(  bf.bfOffBits, SQ_SEEK_SET );

			file->Read( pTmp, lineBytes * m_height );
		
			m_pixelFormat = PF_I8;
			switch ( info.biBitCount )
			{
			case 8:
				{				
					for ( int yy = 0; yy < m_height; yy++ ) 
					{
						memcpy( m_pPixels + (yy * m_width), pTmp + (m_height - yy -1) * lineBytes, m_width );
					}
					break;
				}
			case 4:
				{
					BYTE *pDest = m_pPixels;
					BYTE *pSrc = pTmp + (m_height - 1) * lineBytes;
					for ( int yy = 0; yy < m_height; yy++ ) {
						for ( int xx = 0; xx < m_width; xx += 2 ) {
							pDest[xx]		= pSrc[xx >> 1] & 0x0F;
							pDest[xx + 1]	= pSrc[xx >> 1] >> 4;
						}
						pDest += m_width;
						pSrc -= lineBytes;
					}
					break;
				}
			case 1:
				{
					BYTE *pDest = m_pPixels;
					BYTE *pSrc = pTmp + (m_height - 1) * lineBytes;
					for ( int yy = 0; yy < m_height; yy++ ) {
						for ( int xx = 0; xx < m_width; xx += 8 ) {
							pDest[xx]		= pSrc[xx >> 3] & 0x01;
							pDest[xx + 1]	= (pSrc[xx >> 3] >> 1) & 0x01;
							pDest[xx + 2]	= (pSrc[xx >> 3] >> 2) & 0x01;
							pDest[xx + 3]	= (pSrc[xx >> 3] >> 3) & 0x01;
							pDest[xx + 4]	= (pSrc[xx >> 3] >> 4) & 0x01;
							pDest[xx + 5]	= (pSrc[xx >> 3] >> 5) & 0x01;
							pDest[xx + 6]	= (pSrc[xx >> 3] >> 6) & 0x01;
							pDest[xx + 7]	= (pSrc[xx >> 3] >> 7) & 0x01;
						}
						pDest += m_width;
						pSrc -= lineBytes;
					}
					break;
				}
			}

			delete [] pTmp;
			break;
		}
	default:
		{		
			Destroy();
			return false;
		}
	}

	return true;

}
#endif //IMAGELIB_SUPPORT_BMP

#include "cImagePNG.h"

#if IMAGELIB_SUPPORT_PNG

void png_SQStream_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
SQStream* file=(SQStream*)png_ptr->io_ptr;

file->Read(data,length);
}

bool cImagePNG::Decode( SQStream* file )
{ 	
	png_structp png_ptr;
	png_infop info_ptr;
	png_uint_32 width, height;
	png_colorp png_palette;
	int color_type, palette_entries;
	int bit_depth, pixel_depth;		// pixel_depth = bit_depth * channels

	if ( file == NULL ) 
		return false;

	// verify PNG signature
	file->Seek(  0, SQ_SEEK_SET );
	png_byte sig[8];
	file->Read( sig,  8 );
	if ( png_sig_cmp( sig, 0, 8 ) )
	{
		return false;
	}

	/* open the file */

	/* Create and initialize the png_struct with the desired error handler
	* functions.  If you want to use the default stderr and longjump method,
	* you can supply NULL for the last three parameters.  We also supply the
	* the compiler header file version, so that we know if the application
	* was compiled with a compatible version of the library.  REQUIRED
	*/
	// read struct
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,(void *)NULL,NULL,NULL);
	if (png_ptr == NULL) {
		return false ;
	}

	/* Allocate/initialize the memory for image information.  REQUIRED. */
	// info struct
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) 
	{
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		return false;
	}

	/* Set error handling if you are using the setjmp/longjmp method (this is
	* the normal method of doing things with libpng).  REQUIRED unless you
	* set up your own error handlers in the png_create_read_struct() earlier.
	*/
	if (setjmp(png_ptr->jmpbuf)) 
	{
		/* Free all of the memory associated with the png_ptr and info_ptr */
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		return false ;
	}
	/* set up the input control */
	//png_init_io(png_ptr, file);
	png_set_read_fn(png_ptr, file,png_SQStream_read_data);


	png_set_sig_bytes(png_ptr, 8);  // we already read 8 bytes for the sig

	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
	pixel_depth = info_ptr->pixel_depth;

	if ( bit_depth == 16 ) {
		if ( color_type == PNG_COLOR_TYPE_GRAY ) {
			// turn on 16 bit byte swapping
			png_set_swap(png_ptr);
		} else {
			// tell libpng to strip 16 bit/color files down to 8 bits/color
			png_set_strip_16(png_ptr);
			bit_depth = 8;
		}
	}

	// set some additional flags
	switch(color_type)
	{
	case PNG_COLOR_TYPE_RGB:
	case PNG_COLOR_TYPE_RGB_ALPHA:
		// flip the RGB pixels to BGR (or RGBA to BGRA)

		png_set_bgr(png_ptr);
		break;

	case PNG_COLOR_TYPE_PALETTE:
		// expand palette images to the full 8 bits from 2 or 4 bits/pixel

		if ((pixel_depth == 2) || (pixel_depth == 4)) {
			png_set_packing(png_ptr);
			pixel_depth = 8;
		}
		break;

	case PNG_COLOR_TYPE_GRAY:
		// expand grayscale images to the full 8 bits from 2 or 4 bits/pixel

		if ((pixel_depth == 2) || (pixel_depth == 4)) {
			png_set_expand(png_ptr);
			pixel_depth = 8;
		}
		break;

	case PNG_COLOR_TYPE_GRAY_ALPHA:
		// expand 8-bit greyscale + 8-bit alpha to 32-bit

		png_set_gray_to_rgb(png_ptr);

		// flip the RGBA pixels to BGRA

		png_set_bgr(png_ptr);
		pixel_depth = 32;
		break;
	}

	// Get the background color to draw transparent and alpha images over.
	// Note that even if the PNG file supplies a background, you are not required to
	// use it - you should use the (solid) application background if it has one.

	png_color_16p image_background = NULL;
	RGBQUAD rgbBkColor;

	if (png_get_bKGD(png_ptr, info_ptr, &image_background)) {
		rgbBkColor.rgbRed      = (BYTE)image_background->red;
		rgbBkColor.rgbGreen    = (BYTE)image_background->green;
		rgbBkColor.rgbBlue     = (BYTE)image_background->blue;
		rgbBkColor.rgbReserved = 0;
	}

	// if this image has transparency, store the trns values

	png_bytep trans               = NULL;
	int num_trans                 = 0;
	png_color_16p trans_values    = NULL;
	png_uint_32 transparent_value = png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans, &trans_values);

	// unlike the example in the libpng documentation, we have *no* idea where
	// this file may have come from--so if it doesn't have a file gamma, don't
	// do any correction ("do no harm")	


	// all transformations have been registered; now update info_ptr data

	png_read_update_info(png_ptr, info_ptr);

	// color type may have changed, due to our transformations

	color_type = png_get_color_type(png_ptr,info_ptr);

	// create a DIB and write the bitmap header
	// set up the DIB palette, if needed

	switch (color_type)
	{
	case PNG_COLOR_TYPE_RGB:
		png_set_invert_alpha(png_ptr);

		Create( width, height, 24, IMAGELIB_FORMAT_PNG );			
		break;

	case PNG_COLOR_TYPE_RGB_ALPHA :
		Create( width, height, 32, IMAGELIB_FORMAT_PNG );
		break;

	case PNG_COLOR_TYPE_PALETTE :
		Create( width, height, pixel_depth, IMAGELIB_FORMAT_PNG );

		png_get_PLTE( png_ptr, info_ptr, &png_palette, &palette_entries);
		SetPalette( (BGRCOLOR*)png_palette, palette_entries );			

		// store the transparency table
		if ( png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS) ) {
			SetPaletteAlpha( trans, num_trans );				
		}
		break;

	case PNG_COLOR_TYPE_GRAY:
		Create( width, height, pixel_depth, IMAGELIB_FORMAT_PNG );
		SetGrayPalette();

		// store the transparency table
		if ( png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS) ) {
			SetPaletteAlpha( trans, num_trans );
		}
		break;

	default:
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return false;
	}	

	// set the individual row_pointers to point at the correct offsets

	png_bytepp  row_pointers = (png_bytepp)malloc(height * sizeof(png_bytep));
	if (!row_pointers) {		

		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

		free(row_pointers);
		return false;
	}

	// read in the bitmap bits via the pointer table
	for (png_uint_32 k = 0; k < height; k++)
		row_pointers[k] = m_pPixels + k * (width * pixel_depth / 8);

	png_read_image(png_ptr, row_pointers);

	// cleanup
	if (row_pointers)
		free(row_pointers);

	// read the rest of the file, getting any additional chunks in info_ptr
	png_read_end(png_ptr, info_ptr);

	if (png_ptr) {
		// clean up after the read, and free any memory allocated - REQUIRED
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
	}

	return true;
}
#endif//IMAGELIB_SUPPORT_PNG



