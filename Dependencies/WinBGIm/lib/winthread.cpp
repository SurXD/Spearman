// File: WindowThread.cpp
// Written by:
//      Grant Macklem (Grant.Macklem@colorado.edu)
//      Gregory Schmelter (Gregory.Schmelter@colorado.edu)
//      Alan Schmidt (Alan.Schmidt@colorado.edu)
//      Ivan Stashak (Ivan.Stashak@colorado.edu)
// CSCI 4830/7818: API Programming
// University of Colorado at Boulder, Spring 2003
// http://www.cs.colorado.edu/~main/bgi
//

#include <windows.h>            // Provides the Win32 API
#include <windowsx.h>           // Provides message cracker macros (p. 96)
#include "winbgim.h"             // External API routines
#include "winbgitypes.h"        // Internal structures and routines
#include <vector>               // Used in BGI__WindowTable
#include <queue>                // Provides queue<POINTS>

// This structure is how the user interacts with the window.  Upon creation,y
// the user gets the index into this array of the window he created.  We will
// use this array for any function which does not deal with the window
// directly, but relies on the current window.
// MGM: hInstance is no longer a handle to the DLL, but instead it is
// a "self" handle, returned by GetCurrentThread( ).
std::vector<HWND> BGI__WindowTable;
int BGI__WindowCount = 0;                    // Number of windows currently in use
int BGI__CurrentWindow = NO_CURRENT_WINDOW;  // Index to current window
HINSTANCE BGI__hInstance;   // Handle to the instance of the DLL (creating the window class)

#include <iostream>
using namespace std;
// ID numbers for new options that are added to the system menu:
#define BGI_PRINT_SMALL 1
#define BGI_PRINT_MEDIUM 2
#define BGI_PRINT_LARGE 3
#define BGI_SAVE_AS 4
// This is the window creation and message processing function.  Each new
// window is created in its own thread and handles all its own messages.
// It creates the window, sets the current window of the application to this
// window, and signals the main thread that the window has been created.
//
DWORD WINAPI BGI__ThreadInitWindow( LPVOID pThreadData )
{
    HWND hWindow;                       // A handle to the window
    MSG Message;                        // A windows event message
    HDC hDC;                            // The device context of the window
    HBITMAP hBitmap;                    // A compatible bitmap of the DC for the Memory DC
    HMENU hMenu;                        // Handle to the system menu
    int CaptionHeight, xBorder, yBorder;
    
    WindowData *pWndData = (WindowData*)pThreadData; // Thread creation data

    if (pWndData->title.size( ))
    {
	CaptionHeight = GetSystemMetrics( SM_CYCAPTION );   // Height of caption
    }
    else
    {
	CaptionHeight = 0;                                 // Height of caption
    }
    xBorder = GetSystemMetrics( SM_CXFIXEDFRAME );      // Width of border
    yBorder = GetSystemMetrics( SM_CYFIXEDFRAME );      // Height of border
    
    int height = pWndData->height + CaptionHeight + 2*yBorder; // Calculate total height
    int width = pWndData->width + 2*xBorder;                   // Calculate total width

    int top = pWndData->inittop;                                 // MGM: Initial top
    int left = pWndData->initleft;                               // MGM: Initial left

    hWindow = CreateWindowEx( 0,                    // Extended window styles
                              _T( "BGILibrary" ),   // What kind of window
                              pWndData->title.c_str( ),  // Title at top of the window
			      pWndData->title.size( )
			      ? (WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_DLGFRAME)
			      : (WS_POPUP|WS_DLGFRAME),
                              left, top, width, height,  // left top width height
                              NULL,                 // HANDLE to the parent window
                              NULL,                 // HANDLE to the menu
                              BGI__hInstance,       // HANDLE to this program
                              NULL );               // Address of window-creation data
    // Check if the window was created
    if ( hWindow == 0 )
    {
        showerrorbox( );
        return 0;
    }

    // Add the print options to the system menu, as shown in Chapter 10 (p 460) of Petzold
    hMenu = GetSystemMenu( hWindow, FALSE );
    AppendMenu( hMenu, MF_SEPARATOR, 0, NULL );
    AppendMenu( hMenu, MF_STRING, BGI_SAVE_AS, TEXT("Save image as...") );
    AppendMenu( hMenu, MF_STRING, BGI_PRINT_SMALL, TEXT("Print 2\" wide...") );
    AppendMenu( hMenu, MF_STRING, BGI_PRINT_MEDIUM, TEXT("Print 4.5\" wide...") );
    AppendMenu( hMenu, MF_STRING, BGI_PRINT_LARGE, TEXT("Print 7\" wide...") );
    AppendMenu( hMenu, MF_SEPARATOR, 0, NULL );

    // Store the HANDLE in the structure
    pWndData->hWnd = hWindow;

    // Store the address of the WindowData structure in the window's user data
    SetWindowLongPtr( hWindow, GWLP_USERDATA, (LONG_PTR)pWndData );

    // Set the default active and visual page.  These must be set here in
    // addition to setting all the defaults in initwindow because the paint
    // method depends on using the correct page.
    pWndData->ActivePage = 0;
    pWndData->VisualPage = 0;

    // Clear the mouse handler array and turn off queuing
    memset( pWndData->mouse_handlers, 0, (WM_MOUSELAST-WM_MOUSEFIRST+2) * sizeof(Handler) );
    memset( pWndData->mouse_queuing, 0, (WM_MOUSELAST-WM_MOUSEFIRST+1) * sizeof(bool) );
    pWndData->mousedbl=0;
    pWndData->nmouse_key=0;
    pWndData->mouseb=0;
    // Create a memory Device Context used for drawing.  The image is copied from here
    // to the screen in the paint method.  The DC and bitmaps are deleted
    // in cls_OnDestroy()
    hDC = GetDC( hWindow );
    pWndData->hDCMutex = CreateMutex(NULL, FALSE,	NULL);
    WaitForSingleObject(pWndData->hDCMutex, 5000);
    for ( int i = 0; i < MAX_PAGES; i++ )
    {
        pWndData->hDC[i] = CreateCompatibleDC( hDC );
        // Create a bitmap for the memory DC.  This is where the drawn image is stored.
        hBitmap = CreateCompatibleBitmap( hDC, pWndData->width, pWndData->height );
        pWndData->hOldBitmap[i] = (HBITMAP)SelectObject( pWndData->hDC[i], hBitmap );
    }
    ReleaseMutex(pWndData->hDCMutex);    
    // Release the original DC and set up the mutex for the hDC array
    ReleaseDC( hWindow, hDC );
    
    // Make the window visible
    ShowWindow( hWindow, SW_SHOWNORMAL );           // Make the window visible
    UpdateWindow( hWindow );                        // Flush output buffer
    
    // Tell the user thread that the window was created successfully
    SetEvent( pWndData->WindowCreated );

    // The message loop, which stops when a WM_QUIT message arrives
    while( GetMessage( &Message, NULL, 0, 0 ) )
    {
        TranslateMessage( &Message );
        DispatchMessage( &Message );
    }

    // Free memory used by thread structure
    delete pWndData;
    return (DWORD)Message.wParam;
}


// This function handles the WM_CHAR message.  This message is sent whenever
// the user presses a key in the window (after a WM_KEYDOWN and WM_KEYUP
// message, a WM_CHAR message is added).  It adds the key pressed to the
// keyboard queue for the window.
//
void cls_OnChar( HWND hWnd, TCHAR ch, int repeat )
{
    // This gets the address of the WindowData structure associated with the window
    WindowData *pWndData = BGI__GetWindowDataPtr( hWnd );

    pWndData->kbd_queue.push( ch&0xFF );// Add the key to the queue
    SetEvent( pWndData->key_waiting );    // Notify the waiting thread, if any
    FORWARD_WM_CHAR( hWnd, ch, repeat, DefWindowProc );
}

static void cls_OnClose( HWND hWnd )
{
    // This gets the address of the WindowData structure associated with the window
    WindowData *pWndData = BGI__GetWindowDataPtr( hWnd );

    exit(0);
}

// This function handles the destroy message.  It will cause the application
// to send WM_QUIT, which will then terminate the message pump thread.
//
static void cls_OnDestroy( HWND hWnd )
{
    // This gets the address of the WindowData structure associated with the window
    WindowData *pWndData = BGI__GetWindowDataPtr( hWnd );

    WaitForSingleObject(pWndData->hDCMutex, 5000);
    for ( int i = 0; i < MAX_PAGES; i++ )
    {
        // Delete the pen in the DC's
        DeletePen( SelectPen( pWndData->hDC[i], GetStockPen( WHITE_PEN ) ) );

        // Delete the brush in the DC's
        DeleteBrush( SelectBrush( pWndData->hDC[i], GetStockBrush( WHITE_BRUSH ) ) );

        // Here we clean up the memory device contexts used by the program.
        // This selects the original bitmap back into the memory DC.  The SelectObject
        // function returns the current bitmap which we then delete.
        DeleteObject( SelectObject( pWndData->hDC[i], pWndData->hOldBitmap[i] ) );
        // Finally, we delete the MemoryDC
        DeleteObject( pWndData->hDC[i] );
    }
    ReleaseMutex(pWndData->hDCMutex);
    // Clean up the bitmap memory
    DeleteBitmap( pWndData->hbitmap );

    // Delete the two events created
    CloseHandle( pWndData->key_waiting );
    CloseHandle( pWndData->WindowCreated );

    PostQuitMessage( 0 );
}


// This function handles the KEYDOWN message and will translate the virtual
// keys to the keys expected by the user
//
static char key_state[256];
int keystate(int key)
{ if(key&KEY_SPECIAL)
    key=(key&0x7F)+128;
  else {
    key&=0x7F; 
    if(key>='a' && key<='z') key=key-'a'+'A';
  }
  int r=key_state[key];
  if(r==2) key_state[key]=0;
  return r;
}

#define VK_ALT 18
static void push_key(WindowData *pWndData, int key)
{
  pWndData->kbd_queue.push( 0 );
  pWndData->kbd_queue.push( key&0xFF );
  key_state[128+(key&0x7F)]=1;
  SetEvent( pWndData->key_waiting );
}
static void push_key2(WindowData *pWndData, int key)
{
  pWndData->kbd_queue.push( key );
  if(key!=WINDOW_CLOSE) pWndData->nmouse_key++;
  SetEvent( pWndData->key_waiting );
}
static int vk2keyalpha(UINT vk)
{ if(vk>=0x41 && vk<=0x5A) return vk-0x41+'A';
  if(vk==VK_SPACE) return ' ';
  if(vk>=0x30 && vk<=0x39) return vk-0x30+'0';
  if(vk>=0x60 && vk<=0x69) return vk-0x60+'0';
  if(vk==VK_ESCAPE) return KEY_ESC;
  if(vk==VK_BACK) return KEY_BACKSPACE;
  if(vk==VK_RETURN) return KEY_ENTER;
  if(vk==VK_TAB) return KEY_TAB;
  if(vk==VK_MULTIPLY) return '*';
  if(vk==VK_ADD) return '+';
  if(vk==VK_SUBTRACT) return '-';
  if(vk==VK_DECIMAL) return ',';
  if(vk==VK_DIVIDE) return '/';
  if(vk==VK_OEM_1) return ';';
  if(vk==VK_OEM_PLUS) return '+';
  if(vk==VK_OEM_COMMA) return ',';
  if(vk==VK_OEM_MINUS) return '-';
  if(vk==VK_OEM_PERIOD) return '.';
  if(vk==VK_OEM_2) return '/';
  if(vk==VK_OEM_3) return '~';
  if(vk==VK_OEM_4) return '[';
  if(vk==VK_OEM_5) return '\\';
  if(vk==VK_OEM_6) return ']';
  if(vk==VK_OEM_7) return '\'';
  if(vk==VK_OEM_8) return '?';
  return 0;
}
static int vk2keycode(UINT vk)
{
  switch(vk)
  {
    case VK_CLEAR: return KEY_CENTER;
    case VK_PRIOR: return KEY_PGUP;
    case VK_NEXT: return KEY_PGDN;
    case VK_END: return KEY_END;
    case VK_HOME: return KEY_HOME;
    case VK_LEFT: return KEY_LEFT;
    case VK_UP: return KEY_UP;
    case VK_RIGHT: return KEY_RIGHT;
    case VK_DOWN: return KEY_DOWN;
    case VK_INSERT: return KEY_INSERT;
    case VK_DELETE: return KEY_DELETE;
    case VK_F1: return KEY_F1;
    case VK_F2: return KEY_F2;
    case VK_F3: return KEY_F3;
    case VK_F4: return KEY_F4;
    case VK_F5: return KEY_F5;
    case VK_F6: return KEY_F6;
    case VK_F7: return KEY_F7;
    case VK_F8: return KEY_F8;
    case VK_F9: return KEY_F9;
    case VK_F10: return KEY_F10;
    case VK_F11: return KEY_F11;
    case VK_F12: return KEY_F12;
    case VK_NUMLOCK: return KEY_NUM_LOCK;
    case VK_SCROLL: return KEY_SCROLL_LOCK;
    case VK_SHIFT: return KEY_SHIFT;
    case VK_CONTROL: return KEY_CTRL;
    case VK_ALT: return KEY_ALT;
    default: return 0;
  }
}
static void cls_OnKey( HWND hWnd, UINT vk, BOOL down, int repeat, UINT flags )
{
    // This gets the address of the WindowData structure associated with the window
    // TODO: Set event for each key
    WindowData *pWndData = BGI__GetWindowDataPtr( hWnd );
    int key=vk2keycode(vk);
    if(key)
    {
      switch ( vk )
      {
        case VK_SHIFT: 
          if(MapVirtualKey(flags&0xFF, MAPVK_VSC_TO_VK_EX)==VK_LSHIFT)
  	    pWndData->flags|=STATE_LSHIFT;
          else
  	    pWndData->flags|=STATE_RSHIFT;
          push_key(pWndData,key);
          break;
        case VK_CONTROL: 
  	  if(flags&0x100) pWndData->flags|=STATE_RCTRL; else pWndData->flags|=STATE_LCTRL;
          push_key(pWndData,key);
          break;
        case VK_ALT: 
    	  if(flags&0x100) pWndData->flags|=STATE_RALT; else pWndData->flags|=STATE_LALT;
          push_key(pWndData,key);
          return;  
        default: 
          push_key(pWndData,key);
      }
    }
    else
    { key=vk2keyalpha(vk);
      if(key) key_state[key]=1;
    }
    FORWARD_WM_KEYDOWN( hWnd, vk, repeat, flags, DefWindowProc );
}

#include <iostream>
static void cls_OnPaint( HWND hWnd )
{
    PAINTSTRUCT ps;             // BeginPaint puts info about the paint request here
    HDC hSrcDC;                 // The device context to copy from
    WindowData *pWndData = BGI__GetWindowDataPtr( hWnd );
    int width, height;          // Area that needs to be redrawn
    POINT srcCorner;            // Logical coordinates of the source image upper left point
    BOOL success;               // Is the BitBlt successful?
    int i;                      // Count for how many bitblts have been tried.

    WaitForSingleObject(pWndData->hDCMutex, INFINITE);
    BeginPaint( hWnd, &ps );
    hSrcDC = pWndData->hDC[pWndData->VisualPage];   // The source (memory) DC

    // Get the dimensions of the area that needs to be redrawn.
    width = ps.rcPaint.right - ps.rcPaint.left;
    height = ps.rcPaint.bottom - ps.rcPaint.top;
    

    // The region that needs to be updated is specified in device units (pixels) for the actual DC.
    // However, if a viewport is specified, the source image is referenced in logical
    // units.  Perform the conversion.
    // MGM TODO: When is the DPtoLP call needed?
    srcCorner.x = ps.rcPaint.left;
    srcCorner.y = ps.rcPaint.top;
    DPtoLP( hSrcDC, &srcCorner, 1 );

    // MGM: Screen BitBlts are not always successful, although I don't know why.
    success = BitBlt( ps.hdc, ps.rcPaint.left, ps.rcPaint.top, width, height,
			 hSrcDC, srcCorner.x, srcCorner.y, SRCCOPY );
    EndPaint( hWnd, &ps );  // Validates the rectangle
    ReleaseMutex(pWndData->hDCMutex);
    
    if ( !success )
    {   // I would like to invalidate the rectangle again
	// since BitBlt wasn't successful, but the recursion seems
	// to hang some machines.
	// delay(100);
	// InvalidateRect( hWnd, &(ps.rcPaint), FALSE );
	// std::cout << "Failure in cls_OnPaint" << std:: endl;
    }
}

// The message-handler function for the window
//
LRESULT CALLBACK WndProc
( HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam )
{
    const std::queue<POINTS> EMPTY;
    POINTS where;
    
    WindowData *pWndData = BGI__GetWindowDataPtr( hWnd );
    int type;           // Type of mouse message
    Handler handler;    // Registered mouse handler
    UINT uHitTest;
    
    // If this is a mouse message, set our internal state
    if ( pWndData && ( uiMessage >= WM_MOUSEFIRST ) && ( uiMessage <= WM_MOUSELAST ) )
    {
	type = uiMessage - WM_MOUSEFIRST;
	if (!(pWndData->mouse_queuing[type]) && pWndData->clicks[type].size( ) )
	{
	    pWndData->clicks[type] = EMPTY;
	}
        pWndData->clicks[type].push(where = MAKEPOINTS( lParam ));  // Set the current position for the event type
	if(uiMessage!=WM_MOUSEWHEEL) pWndData->mouse = where; // Set the current mouse position
	switch(uiMessage)
        { case WM_LBUTTONDOWN: pWndData->mouseb|=1; 
              if((pWndData->mousedbl&1)==0)
                push_key2(pWndData, MOUSE_LCLICK);
              pWndData->mousedbl&=~1;
              break;
	  case WM_LBUTTONUP: pWndData->mouseb&=~1; 
              pWndData->mousedbl&=~1;
              break;
          case WM_RBUTTONDOWN: pWndData->mouseb|=2; 
              if((pWndData->mousedbl&2)==0)
                push_key2(pWndData, MOUSE_RCLICK);
              pWndData->mousedbl&=~2;
              break;
	  case WM_RBUTTONUP: pWndData->mouseb&=~2; 
              pWndData->mousedbl&=~2;
              break;
          case WM_MBUTTONDOWN: pWndData->mouseb|=4; break;
	  case WM_MBUTTONUP: pWndData->mouseb&=~4; break;
          case WM_MOUSEWHEEL: 
            where.x=GET_WHEEL_DELTA_WPARAM(wParam); where.y=0; 
            push_key2(pWndData, where.x>0?MOUSE_WHEELUP:MOUSE_WHEELDOWN);
            break;
        }
        // If the user has registered a mouse handler, call it now
        handler = pWndData->mouse_handlers[type];
        if ( handler != NULL )
	    handler( where.x, where.y );
    }
    if( pWndData && (uiMessage==WM_SYSKEYDOWN || uiMessage==WM_SYSKEYUP 
            || uiMessage==WM_KEYDOWN || uiMessage==WM_KEYUP || uiMessage==WM_CHAR))
    {
       handler = pWndData->mouse_handlers[WM_MOUSELAST-WM_MOUSEFIRST+1];
       if ( handler != NULL )
         handler(wParam, uiMessage==WM_CHAR?0:lParam);
    }
    switch ( uiMessage )
    {
    HANDLE_MSG( hWnd, WM_CHAR, cls_OnChar );
    HANDLE_MSG( hWnd, WM_DESTROY, cls_OnDestroy );
    HANDLE_MSG( hWnd, WM_KEYDOWN, cls_OnKey );
    HANDLE_MSG( hWnd, WM_PAINT, cls_OnPaint );
    case WM_SYSKEYDOWN:
	if(wParam==VK_F10)
	{   HANDLE_WM_KEYDOWN(hWnd, wParam, lParam, cls_OnKey);
	    return 0;
	}
	else if(wParam==VK_ALT)
	{   HANDLE_WM_KEYDOWN(hWnd, VK_ALT, lParam, cls_OnKey);
	    return 0;
	}
	break;
    case WM_KEYUP:
	if(wParam==VK_CONTROL)
	{
	    if(lParam&0x1000000) 
              pWndData->flags&=~STATE_RCTRL; 
            else 
              pWndData->flags&=~STATE_LCTRL;
            key_state[KEY_CTRL+128]=2;
	}
	else if(wParam==VK_SHIFT)
	{
           if(MapVirtualKey((lParam&0xFF0000)>>16, MAPVK_VSC_TO_VK_EX)==VK_LSHIFT)
       	     pWndData->flags&=~STATE_LSHIFT;
           else
     	     pWndData->flags&=~STATE_RSHIFT;
          key_state[KEY_SHIFT+128]=2;
	}
        else {
          int key=vk2keycode(wParam);
          if(key) key_state[128+(key&0x7f)]=2;
          else { key=vk2keyalpha(wParam);
                 if(key) key_state[key]=2;
          }
        }
	break;
    case WM_SYSKEYUP:
	if(wParam==VK_F10)
        { key_state[KEY_F10+128]=2;
	  return 0;
        }
	else if(wParam==VK_ALT)
	{
          if(lParam&0x1000000) pWndData->flags&=~STATE_RALT; else pWndData->flags&=~STATE_LALT;
          key_state[KEY_ALT+128]=2;
          return 0;
	}
	break;
    case WM_LBUTTONDBLCLK:
        pWndData->mousedbl|=1;
        push_key2(pWndData, MOUSE_LDBLCLICK);
	return TRUE;
    case WM_RBUTTONDBLCLK:
        pWndData->mousedbl|=2;
        push_key2(pWndData, MOUSE_RDBLCLICK);
	return TRUE;
    case WM_NCHITTEST:
	uHitTest = DefWindowProc(hWnd, WM_NCHITTEST, wParam, lParam);
	if(uHitTest != HTCLIENT && pWndData && pWndData->title.size( ) == 0)
	    return HTCAPTION;
	else
	    return uHitTest;
    case WM_CLOSE:
	if ( pWndData->CloseBehavior )
	{
	    HANDLE_WM_CLOSE( hWnd, wParam, lParam, cls_OnClose );
	}
        else
        {
            push_key2(pWndData, WINDOW_CLOSE);
        }
	return TRUE;
    case WM_SYSCOMMAND:
	switch ( LOWORD(wParam) )
	{
	case BGI_SAVE_AS: writeimagefile(NULL, 0, 0, INT_MAX, INT_MAX, false, hWnd); return 0;
	case BGI_PRINT_SMALL: printimage(NULL, 2.0, 0.75, 0.75, 0, 0, INT_MAX, INT_MAX, false, hWnd); return 0;
	case BGI_PRINT_MEDIUM: printimage(NULL, 4.5, 0.75, 0.75, 0, 0, INT_MAX, INT_MAX, false, hWnd); return 0;
	case BGI_PRINT_LARGE: printimage(NULL, 7.0, 0.75, 0.75, 0, 0, INT_MAX, INT_MAX, false, hWnd); return 0;
	}
	break;
    }
    return DefWindowProc( hWnd, uiMessage, wParam, lParam );
}

