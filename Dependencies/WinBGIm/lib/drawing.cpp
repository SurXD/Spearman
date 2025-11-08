// $Id: drawing.cpp,v 1.7 2003/04/23 06:09:50 bush Exp $
// Written by:
//      Grant Macklem (Grant.Macklem@colorado.edu)
//      Gregory Schmelter (Gregory.Schmelter@colorado.edu)
//      Alan Schmidt (Alan.Schmidt@colorado.edu)
//      Ivan Stashak (Ivan.Stashak@colorado.edu)
// CSCI 4830/7818: API Programming
// University of Colorado at Boulder, Spring 2003
// http://www.cs.colorado.edu/~main/bgi
//


/*****************************************************************************
*
*   Includes and conditional defines (needed for g++)
*
*****************************************************************************/
#define _USE_MATH_DEFINES   // Actually use the definitions in math.h
#include <windows.h>        // Provides the Win32 API
#include <windowsx.h>       // Provides GDI helper macros
#include <math.h>           // For mathematical functions
#include <string.h>         // Provides strlen
#include "winbgim.h"         // API routines
#include "winbgitypes.h"    // Internal structure data
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif


/*****************************************************************************
*
*   Helper functions
*
*****************************************************************************/
void minmaxswap(int &x, int &y)
{
  if(x<=y) return;
  int t=x;
  x=y;
  y=t;
}

// This function returns a pointer to the internal data structure holding all
// necessary data for the window specified by hWnd.
//
WindowData* BGI__GetWindowDataPtr( HWND hWnd )
{
    // Get the handle to the current window from the table if none is
    // specified.  Otherwise, use the specified value
    if ( hWnd == NULL && BGI__CurrentWindow >= 0 && BGI__CurrentWindow < BGI__WindowCount)
        hWnd = BGI__WindowTable[BGI__CurrentWindow];
    if (hWnd == NULL)
    {
	showerrorbox("Drawing operation was attempted when there was no current window.");
	exit(0);
    }
    // This gets the address of the WindowData structure associated with the window
    return (WindowData*)GetWindowLongPtr(hWnd, GWLP_USERDATA );
}


// This function returns the device context of the active page for the window
// given by hWnd.  This device context can be used for GDI drawing commands.
//
HDC BGI__GetWinbgiDC( HWND hWnd )
{
    // Get the handle to the current window from the table if none is
    // specified.  Otherwise, use the specified value
    if ( hWnd == NULL )
        hWnd = BGI__WindowTable[BGI__CurrentWindow];
    // This gets the address of the WindowData structure associated with the window
    WindowData *pWndData = BGI__GetWindowDataPtr( hWnd );

    // MGM: Added mutex to prevent conflict with OnPaint thread.
    // Anyone who calls BGI_GetWinbgiDC must later call
    // BGI_ReleaseWinbgiDC.
    WaitForSingleObject(pWndData->hDCMutex, 5000);
    // This is the device context we want to draw to
    return pWndData->hDC[pWndData->ActivePage];
}


void BGI__ReleaseWinbgiDC( HWND hWnd )
{
    // Get the handle to the current window from the table if none is
    // specified.  Otherwise, use the specified value
    if ( hWnd == NULL )
        hWnd = BGI__WindowTable[BGI__CurrentWindow];
    // This gets the address of the WindowData structure associated with the window
    WindowData *pWndData = BGI__GetWindowDataPtr( hWnd );

    // MGM: Added mutex to prevent conflict with OnPaint thread.
    // Anyone who calls BGI_GetWinbgiDC must later call
    // BGI_ReleaseWinbgiDC.
    ReleaseMutex(pWndData->hDCMutex);
}


// This function converts the coordinates the user gives the system (relative
// to the center of the desired object) to coordinates specifying the corners
// of the box surrounding the object.
//
void CenterToBox( int x, int y, int xradius, int yradius,
                 int* left, int* top, int* right, int* bottom )
{
    *left   = x - xradius;
    *top    = y - yradius;
    *right  = x + xradius;
    *bottom = y + yradius;
}


// This function converts coordinates of an arc, specified by a center, radii,
// and start and end angle to actual coordinates of the window of the start
// and end of the arc.
//
void ArcEndPoints( int x, int y, int xradius, int yradius, int stangle,
                  int endangle, int* xstart, int* ystart, int* xend, int* yend )
{
    *xstart = int( xradius * cos( stangle  * M_PI / 180 ) );
    *ystart = int( yradius * sin( stangle  * M_PI / 180 ) );
    *xend   = int( xradius * cos( endangle * M_PI / 180 ) );
    *yend   = int( yradius * sin( endangle * M_PI / 180 ) );

    // These values must be in logical coordinates of the window.
    // Thus, we must translate from coordinates respective to the
    // center of the bounding box to the window.  Also, the direction
    // of positive y changes from up to down.
    *xstart += x;
    *ystart  = -*ystart + y;
    *xend   += x;
    *yend    = -*yend + y;
}

// This function will refresh the area of the window specified by rect.  If
// want to update the entire screen, pass in NULL for rect.
// POSTCONDITION: The parameter rect has been updated to now refer to
//                device coordinates instead of logical coordinates.  Also,
//                if we are refreshing, then the region specified by rect
//                (in device coordinates) has been marked to repaint
void RefreshWindow( RECT* rect )
{
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    POINT p[2];

    if ( rect != NULL )
    {
        p[0].x = rect->left;
        p[0].y = rect->top;
        p[1].x = rect->right;
        p[1].y = rect->bottom;

        // Convert from the device points to logical points (viewport relative)
	hDC = BGI__GetWinbgiDC( );
        LPtoDP( hDC, p, 2 );
        BGI__ReleaseWinbgiDC( );
	
        // Copy back into the rectangle
        rect->left = p[0].x;
        rect->top = p[0].y;
        rect->right = p[1].x;
        rect->bottom = p[1].y;
    }

    if (pWndData->refreshing || rect == NULL)
    {    
	// Only invalidate the window if we are viewing what we are drawing.
	// The call to InvalidateRect can fail, but I don't know what to do if it does.
	if ( pWndData->VisualPage == pWndData->ActivePage )
	    InvalidateRect( pWndData->hWnd, rect, FALSE );
    }
}

bool getrefreshingbgi( )
{
    return BGI__GetWindowDataPtr( )->refreshing;
}


void setrefreshingbgi(bool value)
{
    BGI__GetWindowDataPtr( )->refreshing = value;
}

void refreshallbgi( )
{
    RefreshWindow(NULL);
}

void refreshbgi(int left, int top, int right, int bottom)
{
    // The update rectangle does not contain the right or bottom edge.  Thus
    // add 1 so the entire region is included.
    RECT rect;
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    POINT p[2];

    p[0].x = min(left, right);
    p[0].y = min(top, bottom);
    p[1].x = max(left, right);
    p[1].y = max(top, bottom);

    // Convert from the device points to logical points (viewport relative)
    hDC = BGI__GetWinbgiDC( );
    LPtoDP( hDC, p, 2 );
    BGI__ReleaseWinbgiDC( );

    // Copy into the rectangle
    rect.left = p[0].x;
    rect.top = p[0].y;
    rect.right = p[1].x;
    rect.bottom = p[1].y;
    
    // Only invalidate the window if we are viewing what we are drawing.
    if ( pWndData->VisualPage == pWndData->ActivePage )
        InvalidateRect( pWndData->hWnd, &rect, FALSE );
}

/*****************************************************************************
*
*   The actual API calls are implemented below
*
*****************************************************************************/

// This function draws a circular arc, centered at (x,y) with the given radius.
// The arc travels from angle stangle to angle endangle.  The angles are given
// in degrees in standard mathematical notation, with 0 degrees along the
// vector (1,0) and travelling counterclockwise.
// POSTCONDITION: The arccoords variable (arcinfo) for the current window
//                is set with data resulting from this call.
//                The current position is not modified.
//
void arc( int x, int y, int stangle, int endangle, int radius )
{
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    // Convert coordinates to those expected by GDI Arc
    int left, top, right, bottom;
    int xstart, ystart, xend, yend;

    // Convert center coordinates to box coordinates
    CenterToBox( x, y, radius, radius, &left, &top, &right, &bottom );
    // Convert given arc specifications to pixel start and end points.
    ArcEndPoints( x, y, radius, radius, stangle, endangle, &xstart, &ystart, &xend, &yend );

    // Draw to the current active page
    hDC = BGI__GetWinbgiDC( );
    Arc( hDC, left, top, right, bottom, xstart, ystart, xend, yend );
    BGI__ReleaseWinbgiDC( );
    
    // The update rectangle does not contain the right or bottom edge.  Thus
    // add 1 so the entire region is included.
    int w=pWndData->lineInfo.thickness;
    RECT rect = { left-w, top-w, right+w, bottom+w };
    RefreshWindow( &rect );

    // Set the arccoords structure to relevant data.
    pWndData->arcInfo.x = x;
    pWndData->arcInfo.y = y;
    pWndData->arcInfo.xstart = xstart;
    pWndData->arcInfo.ystart = ystart;
    pWndData->arcInfo.xend = xend;
    pWndData->arcInfo.yend = yend;
}

// This function draws a 2D bar.
//
void bar( int left, int top, int right, int bottom )
{
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    HBRUSH hBrush;
    int color;
    int w=pWndData->lineInfo.thickness;

    hDC = BGI__GetWinbgiDC( );
    // Is it okay to use the currently selected brush to paint with?
    hBrush = (HBRUSH)GetCurrentObject( hDC, OBJ_BRUSH );
    // Set the text color for the fill pattern
    // Convert from BGI color to RGB color
    color = converttorgb( pWndData->fillInfo.color );
    SetTextColor( hDC, color );
    minmaxswap(left,right);
    minmaxswap(top,bottom);
    RECT r = {left, top, right+1, bottom+1};
    FillRect( hDC, &r, hBrush );
    // Reset the text color to the drawing color
    color = converttorgb( pWndData->drawColor );
    SetTextColor( hDC, color );
    BGI__ReleaseWinbgiDC( );
    
    // The update rectangle does not contain the right or bottom edge.  Thus
    // add 1 so the entire region is included.
    RECT rect = { left-w, top-w, right+w, bottom+w };
    RefreshWindow( &rect );
}


// This function draws a bar with a 3D outline.  The angle of the bar background is
// 30 degrees.
void bar3d( int left, int top, int right, int bottom, int depth, int topflag )
{
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    int color;
    int dy;     // Distance to draw 3D bar up to
    POINT p[4]; // An array to hold vertices for the outline

    minmaxswap(left,right);
    minmaxswap(top,bottom);

    hDC = BGI__GetWinbgiDC( );
    // Set the text color for the fill pattern
    // Convert from BGI color to RGB color
    color = converttorgb( pWndData->fillInfo.color );
    SetTextColor( hDC, color );
    Rectangle( hDC, left, top, right, bottom );
    // Reset the text color to the drawing color
    color = converttorgb( pWndData->drawColor );
    SetTextColor( hDC, color );

    // Draw the surrounding part.
    // The depth is specified to be the x-distance from the front line to the
    // back line, not the actual diagonal line length
    dy = (int)(depth * tan( 30.0 * M_PI / 180.0 ));
    
    p[0].x = right;
    p[0].y = bottom;            // Bottom right of box
    p[1].x = right + depth;
    p[1].y = bottom - dy;       // Bottom right of outline
    p[2].x = right + depth;
    p[2].y = top - dy;          // Upper right of outline
    p[3].x = right;
    p[3].y = top;               // Upper right of box

    // A depth of zero is a way to draw a 2D bar with an outline.  No need to
    // draw the 3D outline in this case.
    if ( depth != 0 )
        Polyline( hDC, p, 4 );

    // If the user specifies the top to be drawn
    if ( topflag != 0 )
    {
        p[0].x = right + depth;
        p[0].y = top - dy;          // Upper right of outline
        p[1].x = left + depth;
        p[1].y = top - dy;          // Upper left of outline
        p[2].x = left;
        p[2].y = top;               // Upper left of box
        Polyline( hDC, p, 3 );
    }
    BGI__ReleaseWinbgiDC( );
    
    // The update rectangle does not contain the right or bottom edge.  Thus
    // add 1 so the entire region is included.
    int w=pWndData->lineInfo.thickness;
    RECT rect = { left-w, top-dy-w, right+depth+w, bottom+w };
    RefreshWindow( &rect );
}


// Thus function draws a circle centered at (x,y) of given radius.
//
void circle( int x, int y, int radius )
{
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    int left, top, right, bottom;
    int w=pWndData->lineInfo.thickness;

    // Convert center coordinates to box coordinates
    CenterToBox( x, y, radius, radius, &left, &top, &right, &bottom );

    // When the start and end points are the same, Arc draws a complete ellipse
    hDC = BGI__GetWinbgiDC( );
    Arc( hDC, left, top, right, bottom, x+radius, y, x+radius, y );
    BGI__ReleaseWinbgiDC( );
    
    // The update rectangle does not contain the right or bottom edge.  Thus
    // add 1 so the entire region is included.
    RECT rect = { left-w, top-w, right+w, bottom+w };
    RefreshWindow( &rect );
}


// This function clears the graphics screen (with the background color) and
// moves the current point to (0,0)
//
void cleardevice( )
{
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    int color;          // Background color to fill with
    RECT rect;          // The rectangle to fill
    HRGN hRGN;          // The clipping region (if any)
    int is_rgn;         // Whether or not a clipping region is present
    POINT p;            // Upper left point of window (convert from device to logical points)
    HBRUSH hBrush;      // Brush in the background color

    // Convert from BGI color to RGB color
    color = converttorgb( pWndData->bgColor );

    hDC = BGI__GetWinbgiDC( );
    // Even though a viewport may be set, this function clears the entire screen.
    // Compute the origin in logical coordinates.
    p.x = 0;
    p.y = 0;
    DPtoLP( hDC, &p, 1 );
    rect.left = p.x;
    rect.top = p.y;
    rect.right = pWndData->width;
    rect.bottom = pWndData->height;

    // Get the current clipping region, if any.  The region object must first
    // be created with some valid region.  If the GetClipRgn function
    // succeeds, the region info will be updated to reflect the new region.
    // However, this does not create a new region object.  That is, this
    // simply overwrites the memory of the old region object and we don't have
    // to worry about deleting the old region.
    hRGN = CreateRectRgn( 0, 0, 5, 5 );
    is_rgn = GetClipRgn( hDC, hRGN );
    // If there is a clipping region, select none
    if ( is_rgn != 0 )
        SelectClipRgn( hDC, NULL );
    
    // Fill hDC with background color
    hBrush = CreateSolidBrush( color );
    FillRect( hDC, &rect, hBrush );
    DeleteObject( hBrush );
    // Move the CP back to (0,0) (NOT viewport relative)
    moveto( p.x, p.y );

    // Select the old clipping region back into the device context
    if ( is_rgn != 0 )
        SelectClipRgn( hDC, hRGN );
    // Delete the region
    DeleteRgn( hRGN );
    BGI__ReleaseWinbgiDC( );
    
    RefreshWindow( NULL );
}


// This function clears the current viewport (with the background color) and
// moves the current point to (0,0 (relative to the viewport)
//
void clearviewport( )
{
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    int color;
    RECT rect;
    HBRUSH hBrush;
    
    // Convert from BGI color to RGB color
    color = converttorgb( pWndData->bgColor );

    rect.left = 0;
    rect.top = 0;
    rect.right = pWndData->viewportInfo.right - pWndData->viewportInfo.left;
    rect.bottom = pWndData->viewportInfo.bottom - pWndData->viewportInfo.top;

    // Fill hDC with background color
    hDC = BGI__GetWinbgiDC( );
    hBrush = CreateSolidBrush( color );
    FillRect( hDC, &rect, hBrush );
    DeleteObject( hBrush );
    BGI__ReleaseWinbgiDC( );
    moveto( 0, 0 );

    RefreshWindow( NULL );
}


void drawpoly(int n_points, int* points) 
{ 
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    
    hDC = BGI__GetWinbgiDC();
    Polyline(hDC, (POINT*)points, n_points);
    BGI__ReleaseWinbgiDC( );
    
    // One could compute the convex hull of these points and create the
    // associated region to update...
    RefreshWindow( NULL );
}


// This function draws an elliptical arc with the current drawing color,
// centered at (x,y) with major and minor axes given by xradius and yradius.
// The arc travels from angle stangle to angle endangle.  The angles are given
// in degrees in standard mathematical notation, with 0 degrees along the
// vector (1,0) and travelling counterclockwise.
// 
void ellipse( int x, int y, int stangle, int endangle, int xradius, int yradius )
{
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    // Convert coordinates to those expected by GDI Arc
    int left, top, right, bottom;
    int xstart, ystart, xend, yend;


    // Convert center coordinates to box coordinates
    CenterToBox( x, y, xradius, yradius, &left, &top, &right, &bottom );
    // Convert given arc specifications to pixel start and end points.
    ArcEndPoints( x, y, xradius, yradius, stangle, endangle, &xstart, &ystart, &xend, &yend );

    // Draw to the current active page
    hDC = BGI__GetWinbgiDC( );
    Arc( hDC, left, top, right, bottom, xstart, ystart, xend, yend );
    BGI__ReleaseWinbgiDC( );

    // The update rectangle does not contain the right or bottom edge.  Thus
    // add 1 so the entire region is included.
    RECT rect = { left, top, right+1, bottom+1 };
    RefreshWindow( &rect );
}


// This function draws and ellipse centered at (x,y) with major and minor axes
// xradius and yradius.  It fills the ellipse with the current fill color and
// fill pattern.
//
void fillellipse( int x, int y, int xradius, int yradius )
{
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    // Convert coordinates to those expected by GDI Ellipse
    int left, top, right, bottom;
    int color;
    int w=pWndData->lineInfo.thickness;

    // Convert center coordinates to box coordinates
    CenterToBox( x, y, xradius, yradius, &left, &top, &right, &bottom );

    // Set the text color for the fill pattern
    // Convert from BGI color to RGB color
    hDC = BGI__GetWinbgiDC( );
    color = converttorgb( pWndData->fillInfo.color );
    SetTextColor( hDC, color );
    Ellipse( hDC, left, top, right, bottom );
    // Reset the text color to the drawing color
    color = converttorgb( pWndData->drawColor );
    SetTextColor( hDC, color );
    BGI__ReleaseWinbgiDC( );

    // The update rectangle does not contain the right or bottom edge.  Thus
    // add 1 so the entire region is included.
    RECT rect = { left-w, top-w, right+w, bottom+w };
    RefreshWindow( &rect );
}


void fillpoly(int n_points, int* points)
{
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    int color;

    // Set the text color for the fill pattern
    // Convert from BGI color to RGB color
    hDC = BGI__GetWinbgiDC();
    color = converttorgb( pWndData->fillInfo.color );
    SetTextColor( hDC, color );

    Polygon(hDC, (POINT*)points, n_points);

    // Reset the text color to the drawing color
    color = converttorgb( pWndData->drawColor );
    SetTextColor( hDC, color );
    BGI__ReleaseWinbgiDC( );

    RefreshWindow( NULL );
}


// This function fills an enclosed area bordered by a given color.  If the
// reference poitn (x,y) is within the closed area, the area is filled.  If
// it is outside the closed area, the outside area will be filled.  The
// current fill pattern and style is used.
//
void floodfill( int x, int y, int border )
{
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    int color;

    // Set the text color for the fill pattern
    // Convert from BGI color to RGB color
    color = converttorgb( pWndData->fillInfo.color );
    border = converttorgb( border );
    hDC = BGI__GetWinbgiDC( );
    SetTextColor( hDC, color );
    FloodFill( hDC, x, y, border );
    // Reset the text color to the drawing color
    color = converttorgb( pWndData->drawColor );
    SetTextColor( hDC, color );
    BGI__ReleaseWinbgiDC( );

    RefreshWindow( NULL );
}


// This function draws a line from (x1,y1) to (x2,y2) using the current line
// style and thickness.  It does not update the current point.
//
void line( int x1, int y1, int x2, int y2 )
{
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    int w=pWndData->lineInfo.thickness;

    // The current position
    POINT cp;

    // Move to first point, save old point
    hDC = BGI__GetWinbgiDC( );
    MoveToEx( hDC, x1, y1, &cp );
    // Draw the line
    LineTo( hDC, x2, y2 );
    // Move the current point back to its original position
    MoveToEx( hDC, cp.x, cp.y, NULL );
    BGI__ReleaseWinbgiDC( );

    // The update rectangle does not contain the right or bottom edge.  Thus
    // add 1 so the entire region is included.
    RECT rect = { min(x1,x2)-w, min(y1,y2)-w, max(x1,x2)+w, max(y1,y2)+w };
    RefreshWindow( &rect );
}


// This function draws a line from the current point to a point that is a
// relative distance (dx,dy) away.  The current point is updated to the final
// point.
//
void linerel( int dx, int dy )
{
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    int w=pWndData->lineInfo.thickness;

    // The current position
    POINT cp;

    hDC = BGI__GetWinbgiDC( );
    GetCurrentPositionEx( hDC, &cp );
    LineTo( hDC, cp.x + dx, cp.y + dy );
    BGI__ReleaseWinbgiDC( );

    // The update rectangle does not contain the right or bottom edge.  Thus
    // add 1 so the entire region is included.
    RECT rect = { min(cp.x,cp.x+dx)-w, min(cp.y,cp.y+dy)-w, max(cp.x,cp.x+dx)+w, max(cp.y,cp.y+dy)+w};
    RefreshWindow( &rect );
}


// This function draws a line from the current point to (x,y).  The current
// point is updated to (x,y)
//
void lineto( int x, int y )
{
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    int w=pWndData->lineInfo.thickness;

    // The current position
    POINT cp;

    hDC = BGI__GetWinbgiDC( );
    GetCurrentPositionEx( hDC, &cp );
    LineTo( hDC, x, y );
    BGI__ReleaseWinbgiDC( );

    // The update rectangle does not contain the right or bottom edge.  Thus
    // add 1 so the entire region is included.
    RECT rect = { min(cp.x,x)-w, min(cp.y,y)-w, max(cp.x,x)+w, max(cp.y,y)+w};
    RefreshWindow( &rect );
}


// This function draws a pie slice centered at (x,y) with a given radius.  It
// is filled with the current fill pattern and color and outlined with the
// current line color.  The pie slice travels from angle stangle to angle
// endangle.  The angles are given in degrees in standard mathematical
// notation, with 0 degrees along the vector (1,0) and travelling
//counterclockwise.
// 
void pieslice( int x, int y, int stangle, int endangle, int radius )
{
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    // Convert coordinates to those expected by GDI Pie
    int left, top, right, bottom;
    int xstart, ystart, xend, yend;
    int color;
    int w=pWndData->lineInfo.thickness;


    // Convert center coordinates to box coordinates
    CenterToBox( x, y, radius, radius, &left, &top, &right, &bottom );
    // Convert given arc specifications to pixel start and end points.
    ArcEndPoints( x, y, radius, radius, stangle, endangle, &xstart, &ystart, &xend, &yend );

    // Set the text color for the fill pattern
    // Convert from BGI color to RGB color
    color = converttorgb( pWndData->fillInfo.color );
    hDC = BGI__GetWinbgiDC( );
    SetTextColor( hDC, color );
    Pie( hDC, left, top, right, bottom, xstart, ystart, xend, yend );
    // Reset the text color to the drawing color
    color = converttorgb( pWndData->drawColor );
    SetTextColor( hDC, color );
    BGI__ReleaseWinbgiDC( );

    // The update rectangle does not contain the right or bottom edge.  Thus
    // add 1 so the entire region is included.
    RECT rect = { left-w, top-w, right+w, bottom+w };
    RefreshWindow( &rect );
}

// This function plots a pixel in the specified color at point (x,y)
//
void putpixel( int x, int y, int color )
{
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );

    color = converttorgb( color );
    // The call to SetPixelV might fail, but I don't know what to do if it does.
    hDC = BGI__GetWinbgiDC( );
    SetPixelV( hDC, x, y, color );
    BGI__ReleaseWinbgiDC( );

    // The update rectangle does not contain the right or bottom edge.  Thus
    // add 1 so the entire region is included.
    RECT rect = { x, y, x+1, y+1 };
    RefreshWindow( &rect );
}


// This function draws a rectangle border in the current line style, thickness, and color
//
void rectangle( int left, int top, int right, int bottom )
{
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    int w=pWndData->lineInfo.thickness;
    minmaxswap(left,right);
    minmaxswap(top,bottom);

    POINT endpoints[5];         // Endpoints of the line
    endpoints[0].x = left;      // Upper left
    endpoints[0].y = top;
    endpoints[1].x = right;     // Upper right
    endpoints[1].y = top;
    endpoints[2].x = right;     // Lower right
    endpoints[2].y = bottom;
    endpoints[3].x = left;      // Lower left
    endpoints[3].y = bottom;
    endpoints[4].x = left;      // Upper left to complete rectangle
    endpoints[4].y = top;

    hDC = BGI__GetWinbgiDC( );
    Polyline( hDC, endpoints, 5 );
    BGI__ReleaseWinbgiDC( );

    // The update rectangle does not contain the right or bottom edge.  Thus
    // add 1 so the entire region is included.
    RECT rect = { left-w, top-w, right+w, bottom+w };
    RefreshWindow( &rect );
}


// This function draws an elliptical pie slice centered at (x,y) with major
// and minor radii given by xradius and yradius.  It is filled with the
// current fill pattern and color and outlined with the current line color.
// The pie slice travels from angle stangle to angle endangle.  The angles are
// given in degrees in standard mathematical notation, with 0 degrees along
// the vector (1,0) and travelling counterclockwise.
// 
void sector( int x, int y, int stangle, int endangle, int xradius, int yradius )
{
    HDC hDC;
    WindowData* pWndData = BGI__GetWindowDataPtr( );
    // Convert coordinates to those expected by GDI Pie
    int left, top, right, bottom;
    int xstart, ystart, xend, yend;
    int color;
    int w=pWndData->lineInfo.thickness;


    // Convert center coordinates to box coordinates
    CenterToBox( x, y, xradius, yradius, &left, &top, &right, &bottom );
    // Convert given arc specifications to pixel start and end points.
    ArcEndPoints( x, y, xradius, yradius, stangle, endangle, &xstart, &ystart, &xend, &yend );

    // Set the text color for the fill pattern
    // Convert from BGI color to RGB color
    color = converttorgb( pWndData->fillInfo.color );
    hDC = BGI__GetWinbgiDC( );
    SetTextColor( hDC, color );
    Pie( hDC, left, top, right, bottom, xstart, ystart, xend, yend );
    // Reset the text color to the drawing color
    color = converttorgb( pWndData->drawColor );
    SetTextColor( hDC, color );
    BGI__ReleaseWinbgiDC( );

    // The update rectangle does not contain the right or bottom edge.  Thus
    // add 1 so the entire region is included.
    RECT rect = { left-w, top-w, right+w, bottom+w };
    RefreshWindow( &rect );
}


