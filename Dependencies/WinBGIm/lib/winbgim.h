// The winbgim library, Version 6.0, August 9, 2004
// Written by:
//      Grant Macklem (Grant.Macklem@colorado.edu)
//      Gregory Schmelter (Gregory.Schmelter@colorado.edu)
//      Alan Schmidt (Alan.Schmidt@colorado.edu)
//      Ivan Stashak (Ivan.Stashak@colorado.edu)
//      Michael Main (Michael.Main@colorado.edu)
// CSCI 4830/7818: API Programming
// University of Colorado at Boulder, Spring 2003


// ---------------------------------------------------------------------------
//                          Notes
// ---------------------------------------------------------------------------
// * This library is still under development.
// * Please see http://www.cs.colorado.edu/~main/bgi for information on
// * using this library with the mingw32 g++ compiler.
// * This library only works with Windows API level 4.0 and higher (Windows 95, NT 4.0 and newer)
// * This library may not be compatible with 64-bit versions of Windows
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
//                          Macro Guard and Include Directives
// ---------------------------------------------------------------------------
#ifndef WINBGI_H
#define WINBGI_H
#include <windows.h>        // Provides the mouse message types
#include <limits.h>         // Provides INT_MAX
#include <sstream>          // Provides std::ostringstream
#include "keys_n_colors.h"

// Line thickness settings
enum line_thickness {NORM_WIDTH=1, THICK_WIDTH=3};

// Character Size and Direction
#define USER_CHAR_SIZE  0
enum text_direction {HORIZ_DIR, VERT_DIR};

// Constants for closegraph
#define CURRENT_WINDOW -1
#define ALL_WINDOWS -2
#define NO_CURRENT_WINDOW -3

// The standard Borland 16 colors
#define MAXCOLORS       15
// The standard line styles
enum line_styles { SOLID_LINE, DOTTED_LINE, CENTER_LINE, DASHED_LINE, USERBIT_LINE };

// The standard fill styles
enum fill_styles { EMPTY_FILL, SOLID_FILL, LINE_FILL, LTSLASH_FILL, SLASH_FILL,
                   BKSLASH_FILL, LTBKSLASH_FILL, HATCH_FILL, XHATCH_FILL, INTERLEAVE_FILL,
                   WIDE_DOT_FILL, CLOSE_DOT_FILL, USER_FILL };

// The various graphics drivers
enum graphics_drivers { DETECT, CGA, MCGA, EGA, EGA64, EGAMONO, IBM8514, HERCMONO,
                        ATT400, VGA, PC3270 };

// Various modes for each graphics driver
enum graphics_modes { CGAC0, CGAC1, CGAC2, CGAC3, CGAHI, 
                      MCGAC0 = 0, MCGAC1, MCGAC2, MCGAC3, MCGAMED, MCGAHI,
                      EGALO = 0, EGAHI,
                      EGA64LO = 0, EGA64HI,
                      EGAMONOHI = 3,
                      HERCMONOHI = 0,
                      ATT400C0 = 0, ATT400C1, ATT400C2, ATT400C3, ATT400MED, ATT400HI,
                      VGALO = 0, VGAMED, VGAHI,
                      PC3270HI = 0,
                      IBM8514LO = 0, IBM8514HI };

// Borland error messages for the graphics window.
#define NO_CLICK        -1      // No mouse event of the current type in getmouseclick
enum graph_errors { grInvalidVersion = -18, grInvalidDeviceNum = -15, grInvalidFontNum,
                    grInvalidFont, grIOerror, grError, grInvalidMode, grNoFontMem,
                    grFontNotFound, grNoFloodMem, grNoScanMem, grNoLoadMem,
                    grInvalidDriver, grFileNotFound, grNotDetected, grNoInitGraph,
                    grOk };

// Write modes
enum putimage_ops{ COPY_PUT, XOR_PUT, OR_PUT, AND_PUT, NOT_PUT, TRANSPARENT_PUT, TRANSPARENTBK_PUT, ALPHABLEND_PUT };
enum resize_ops { COLORONCOLOR_RESIZE, HALFTONE_RESIZE,ALPHABLEND_RESIZE };
// Text Modes
enum horiz_align { LEFT_TEXT, CENTER_TEXT, RIGHT_TEXT };
enum vertical_align { BOTTOM_TEXT, VCENTER_TEXT, TOP_TEXT }; // middle not needed other than as seperator
enum font_names { DEFAULT_FONT, TRIPLEX_FONT, SMALL_FONT, SANS_SERIF_FONT,
             GOTHIC_FONT, SCRIPT_FONT, SIMPLEX_FONT, TRIPLEX_SCR_FONT,
			 COMPLEX_FONT, EUROPEAN_FONT, BOLD_FONT };
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
//                              Structures
// ---------------------------------------------------------------------------
// This structure records information about the last call to arc.  It is used
// by getarccoords to get the location of the endpoints of the arc.
struct arccoordstype
{
    int x, y;                   // Center point of the arc
    int xstart, ystart;         // The starting position of the arc
    int xend, yend;             // The ending position of the arc.
};


// This structure defines the fill style for the current window.  Pattern is
// one of the system patterns such as SOLID_FILL.  Color is the color to
// fill with
struct fillsettingstype
{
    int pattern;                // Current fill pattern
    int color;                  // Current fill color
};


// This structure records information about the current line style.
// linestyle is one of the line styles such as SOLID_LINE, upattern is a
// 16-bit pattern for user defined lines, and thickness is the width of the
// line in pixels.
struct linesettingstype
{
    int linestyle;              // Current line style
    unsigned upattern;          // 16-bit user line pattern
    int thickness;              // Width of the line in pixels
};


// This structure records information about the text settings.
struct textsettingstype
{
    int font;                   // The font in use
    int direction;              // Text direction
    int charsize;               // Character size
    int horiz;                  // Horizontal text justification
    int vert;                   // Vertical text justification
};


// This structure records information about the viewport
struct viewporttype
{
    int left, top,              // Viewport bounding box
        right, bottom;
    int clip;                   // Whether to clip image to viewport
};


// This structure records information about the palette.
struct palettetype
{
    unsigned char size;
    signed char colors[MAXCOLORS + 1];
};
// ---------------------------------------------------------------------------



// ---------------------------------------------------------------------------
//                          API Entries
// ---------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

// Drawing Functions
void arc( int x, int y, int stangle, int endangle, int radius );
void bar( int left, int top, int right, int bottom );
void bar3d( int left, int top, int right, int bottom, int depth, int topflag );
void circle( int x, int y, int radius );
void cleardevice( );
void clearviewport( );
void drawpoly(int n_points, int* points);
void ellipse( int x, int y, int stangle, int endangle, int xradius, int yradius );
void fillellipse( int x, int y, int xradius, int yradius );
void fillpoly(int n_points, int* points);
void floodfill( int x, int y, int border );
void line( int x1, int y1, int x2, int y2 );
void linerel( int dx, int dy );
void lineto( int x, int y );
void pieslice( int x, int y, int stangle, int endangle, int radius );
void putpixel( int x, int y, int color );
void rectangle( int left, int top, int right, int bottom );
void sector( int x, int y, int stangle, int endangle, int xradius, int yradius );

// Miscellaneous Functions
int getdisplaycolor( int color );
int converttorgb( int color );
int converttobgi( int color );
void _bgi_delay( int msec );
#define delay _bgi_delay
void getarccoords( arccoordstype *arccoords );
int getbkcolor( );
int getcolor( );
void getfillpattern( char *pattern );
void getfillsettings( fillsettingstype *fillinfo );
void getlinesettings( linesettingstype *lineinfo );
int getmaxcolor( );
int getmaxheight( );
int getmaxwidth( );
int getmaxx( );
int getmaxy( );
bool getrefreshingbgi( );
int getwindowheight( );
int getwindowwidth( );
int getpixel( int x, int y );
void getviewsettings( viewporttype *viewport );
int getx( );
int gety( );
void moverel( int dx, int dy );
void moveto( int x, int y );
void refreshbgi(int left, int top, int right, int bottom);
void refreshallbgi( );    
void setbkcolor( int color );
[[deprecated("Use setbkcolor(NO_COLOR)")]] 	
inline void setbkmode(int mode)
{ setbkcolor(mode==TRANSPARENT?NO_COLOR:BLACK);
}
void setcolor( int color );
void setfillpattern( const char *upattern, int color );
void setfillstyle( int pattern, int color , int bkcolor=-1);
void setlinestyle( int linestyle, unsigned upattern, int thickness );
void setrefreshingbgi(bool value);
void setviewport( int left, int top, int right, int bottom, int clip );
void setwritemode( int mode );
//void settransform( double m[2][3]);

// Window Creation / Graphics Manipulation
void closegraph( int wid=ALL_WINDOWS );
void detectgraph( int *graphdriver, int *graphmode );
void getaspectratio( int *xasp, int *yasp );
const char *getdrivername( );
int getgraphmode( );
int getmaxmode( );
const char *getmodename( int mode_number );
void getmoderange( int graphdriver, int *lomode, int *himode );
void graphdefaults( );
const char *grapherrormsg( int errorcode );
int graphresult( );
void initgraph( int *graphdriver, int *graphmode, const char *pathtodriver );
int initwindow
    ( int width, int height, const char* title="Windows BGI", int left=0, int top=0, bool dbuf=false, bool closeall=true );
[[deprecated("Not available in WinBGI")]] int installuserdriver( const char *name, int *fp );
[[deprecated("Not available in WinBGI")]] int installuserfont( const char *name );  
[[deprecated("Not available in WinBGI")]] int registerbgidriver( void *driver ); 
[[deprecated("Not available in WinBGI")]] int registerbgifont( void *font );              
void restorecrtmode( );
void setaspectratio( int xasp, int yasp );
[[deprecated("Not available in WinBGI")]] unsigned setgraphbufsize( unsigned bufsize );  
void setgraphmode( int mode );
void showerrorbox( const char *msg = NULL );

// User Interaction
int _bgi_getch(int special=0);
#define getch _bgi_getch
int _bgi_kbhit(int mousehit=0);
#define kbhit _bgi_kbhit
int _bgi_controlkeystate( );
#define controlkeystate _bgi_controlkeystate
int keystate(int key);

// User-Controlled Window Functions (winbgi.cpp)
int getcurrentwindow( );
void setcurrentwindow( int window );
    
// Double buffering support (winbgi.cpp)
int getactivepage( );
int getvisualpage( );
void setactivepage( int page );
void setvisualpage( int page );
void swapbuffers( );

// Image Functions (drawing.cpp)
typedef void IMAGE;
unsigned imagesize( int left, int top, int right, int bottom );
void getimage( int left, int top, int right, int bottom, IMAGE *bitmap );
void putimage( int left, int top, IMAGE *bitmap, int op=COPY_PUT, int width=0, int height=0);
void printimage(
    const char* title=NULL,	
    double width_inches=7, double border_left_inches=0.75, double border_top_inches=0.75,
    int left=0, int top=0, int right=INT_MAX, int bottom=INT_MAX,
    bool active=true, HWND hwnd=NULL
    );
void readimagefile(
    const char* filename=NULL,
    int left=0, int top=0, int right=INT_MAX, int bottom=INT_MAX
    );
void writeimagefile(
    const char* filename=NULL,
    int left=0, int top=0, int right=INT_MAX, int bottom=INT_MAX,
    bool active=true, HWND hwnd=NULL
    );

// Text Functions (text.cpp)
void gettextsettings(struct textsettingstype *texttypeinfo);
void outtext(const char *textstring);
void outtextxy(int x, int y, const char *textstring);
void settextjustify(int horiz, int vert);
void settextstyle(int font, int direction, int charsize);
void setusercharsize(int multx, int divx, int multy, int divy);
int textheight(const char *textstring);
int textwidth(const char *textstring);
extern std::ostringstream bgiout;    
void outstream(std::ostringstream& out=bgiout);
void outstreamxy(int x, int y, std::ostringstream& out=bgiout);    
    
// Mouse Functions (mouse.cpp)
void clearmouseclick( int kind );
//void clearresizeevent( );
void getmouseclick( int kind, int& x, int& y );
bool ismouseclick( int kind );
// bool isresizeevent( );
int mousex( );
int mousey( );
int mousebuttons( );
void registermousehandler( int kind, void h( int, int ) );
void setmousequeuestatus( int kind, bool status=true );

#define registerkeyhandler(h) registermousehandler(WM_MOUSELAST+1, h)

// Palette Functions
palettetype *getdefaultpalette( );
void getpalette( palettetype *palette );
int getpalettesize( );
void setallpalette( palettetype *palette );
void setpalette( int colornum, int color );
void setrgbpalette( int colornum, int red, int green, int blue );

int imageheight(IMAGE *image);
int imagewidth(IMAGE *image);
IMAGE *createimage(int w, int h, int color=NO_COLOR);
void freeimage(IMAGE *image);
void imageputpixel(IMAGE *image,int x, int y, int color, int alpha=0xff);
int imagegetpixel(IMAGE *image,int x,int y);

int imagegetalpha(IMAGE *image,int x,int y);
void imagesetalpha(IMAGE *image,int x,int y, int alpha);
void imagealphatransparent(IMAGE *image, int color=-1, int alpha1=0xFF, int alpha2=0);

IMAGE *imagemirror(IMAGE *image, int dir=HORIZ_DIR);
IMAGE *imageturn(IMAGE *image, double a=90.0, int color=NO_COLOR);
IMAGE *imageresize(IMAGE *image, int dwidth=0, int dheight=0, int op=COLORONCOLOR_RESIZE);
IMAGE *imagecopy(IMAGE *, int x=0, int y=0, int width=0, int height=0);

IMAGE *loadBMP(const char *filename);
void saveBMP(const char *filename, IMAGE *image);

// Color Macros
#define IS_BGI_COLOR(v)     ( ((v) >= 0) && ((v) < 16) )
#define RGB_COLOR_BIT 0x01000000
#define IS_RGB_COLOR(v)     ( (v) & RGB_COLOR_BIT )
#define RED_VALUE(v)        int(GetRValue( converttorgb(v) ))
#define GREEN_VALUE(v)      int(GetGValue( converttorgb(v) ))
#define BLUE_VALUE(v)       int(GetBValue( converttorgb(v) ))
#undef COLOR
int COLOR(int r, int g, int b);

#ifdef __cplusplus
}
#endif
// ---------------------------------------------------------------------------

#endif // WINBGI_H

