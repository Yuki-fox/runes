// RegionCreator.cpp: implementation of the CRegionCreator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RegionCreator.h"
#include "sq_stdio.h"
bool CRegionCreator::g_bLoad=true;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegionCreator::CRegionCreator(LPCTSTR name)
{
	m_h =0;
	lstrcpy(m_name,name);

}

CRegionCreator::~CRegionCreator()
{
}

HRGN CRegionCreator::CreateRegionFromBitmap(HBITMAP hBitmap, COLORREF transparentColor,int &w,int &h)
{
	

	// Check for valid bitmap handle
	if ( hBitmap != NULL )
	{
		// Get bitmap object information
		BITMAP bitmap;
		GetObject(hBitmap, sizeof(BITMAP), &bitmap);
		DWORD dwSize = bitmap.bmHeight * bitmap.bmWidthBytes;
		int bitsPixel = bitmap.bmBitsPixel / 8;
		w=bitmap.bmWidth;
		h=bitmap.bmHeight;



		if (LoadRegion())
			return m_h;

		const DWORD RDHDR = sizeof(RGNDATAHEADER);
		const DWORD MAXBUF = 40;		// size of one block in RECTs
		LPRECT	pRects;								
		DWORD	cBlocks = 0;			// number of allocated blocks
		bool	wasfirst = false;		// set when if mask was found in current scan line
		bool	ismask=false;					// set when current color is mask color

			// allocate memory for region data
		RGNDATAHEADER* pRgnData = (RGNDATAHEADER*)new BYTE[ RDHDR + ++cBlocks * MAXBUF * sizeof(RECT) ];
		memset( pRgnData, 0, RDHDR + cBlocks * MAXBUF * sizeof(RECT) );


			// fill it by default
		pRgnData->dwSize	= RDHDR;
		pRgnData->iType		= RDH_RECTANGLES;
		pRgnData->nCount	= 0;

		INT		first = 0;		

		// Check bitmap color depth (only 24 or 32 bits per pixel allowed)
		if ( ( bitsPixel == 3 ) || ( bitsPixel == 4 ) )
		{
			// Get bitmap bits
			unsigned char* pBits = new unsigned char[dwSize];
			GetBitmapBits(hBitmap, dwSize, pBits);

			

			// Create region from bitmap
			unsigned char red=0, green=0, blue=0,alpha=0;
			for ( int y=0; y<bitmap.bmHeight; y++ )
			{
				bool ismask=false;
				int Y_Offset=y*bitmap.bmWidthBytes;
				for ( int x=0; x<bitmap.bmWidth; x++ )
				{
					int XYOffset=Y_Offset+bitsPixel*x;
					if (bitsPixel == 4)
					{
							alpha=pBits[XYOffset+3];

							ismask=(alpha>127);
					}
					else
					{
						// Get pixel color
						blue = pBits[XYOffset];
						green = pBits[XYOffset+1];
						red = pBits[XYOffset+2];
						int transparentColorXX=RGB(red,green,blue);
						ismask=  (transparentColorXX == transparentColor);
					}
				
					
				
					// Check transparent color
					if (wasfirst && ((ismask && (x==(bitmap.bmWidth-1)))||(ismask ^ (x<bitmap.bmWidth))))
					{

						pRects = (LPRECT)((LPBYTE)pRgnData + RDHDR);
						pRects+=pRgnData->nCount;
						
						pRects->left=first;
						pRects->top= y ;
						pRects->right=x+(x==(bitmap.bmWidth-1));
						pRects->bottom=y+1 ;

						// save current RECT
						pRgnData->nCount++;

						if ( pRgnData->nCount >= cBlocks * MAXBUF )
						{
							LPBYTE pRgnDataNew = new BYTE[ RDHDR + ++cBlocks * MAXBUF * sizeof(RECT) ];
							memcpy( pRgnDataNew, pRgnData, RDHDR + (cBlocks - 1) * MAXBUF * sizeof(RECT) );
							delete pRgnData;
							pRgnData = (RGNDATAHEADER*)pRgnDataNew;
						}
						wasfirst = false;

						
					}
					else if ( !wasfirst && ismask )
					{
						first = x;
						wasfirst = true;
					}
						
				}
				
			}

			// Free bitmap bits
			delete pBits;
		}

		HRGN hRgn=CreateRectRgn(0, 0, 0, 0);
		ASSERT( hRgn!=NULL );
		pRects = (LPRECT)((LPBYTE)pRgnData + RDHDR);
		for(int i=0;i<(int)pRgnData->nCount;i++)
		{
			HRGN hr=CreateRectRgn(pRects[i].left, pRects[i].top, pRects[i].right, pRects[i].bottom);
			CombineRgn(hRgn, hRgn, hr, RGN_OR);
			if (hr)
				DeleteObject(hr);
		}
		ASSERT( hRgn!=NULL );

	
		m_h=hRgn;
		SaveRegion();
	}	
		
	


	return m_h;
}
void CRegionCreator::SaveRegion(void)
{
	
	BYTE *pdata;

	int count=GetRegionData(m_h,NULL,0);
	pdata=new BYTE[count]; 
	GetRegionData(m_h,count,(LPRGNDATA)pdata);
	SQFILE file=sqstd_fopen(m_name,_T("wb"));
	if (file)
	{
		sqstd_fwrite(pdata,1,count,file);
		sqstd_fclose(file);
	}
	delete []pdata;

}
bool CRegionCreator::LoadRegion(void)
{
	int size;
	bool ret=false;
	
	BYTE *pdata;

	SQFILE file=sqstd_fopen(m_name,_T("rb"));
	if (file&&g_bLoad)
	{
		ret=true;
		sqstd_fseek( file , 0, SQ_SEEK_END );
		size=sqstd_ftell(file);
		sqstd_fseek( file , 0, SQ_SEEK_SET);
		pdata=new BYTE[size];

		sqstd_fread(pdata,1,size,file);

		m_h = ExtCreateRegion(NULL, size,(LPRGNDATA) pdata); 
		sqstd_fclose(file);
		delete []pdata;
	}
	return ret;
}