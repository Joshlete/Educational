#include <X11/Xlib.h>
#include <unistd.h>
#include <stdlib.h>

class Line{
public:
   int x1; int y1; int x2; int y2;
   Line * next;
   Line(){
      x1 = y1 = x2 = y2 = 0;
      next = NULL;
   }
   void draw( Display * display, Window window, GC gc ){
      XDrawLine( display, window, gc, x1, y1, x2, y2 );
   }
};

    

main()
{
   Line * lines = NULL;
   // Open the display
   Display *dpy = XOpenDisplay( NULL );
   // Get some colors
  int blackColor = BlackPixel(dpy, DefaultScreen(dpy));
  int whiteColor = WhitePixel(dpy, DefaultScreen(dpy));
  // Create the window
  Window w = XCreateSimpleWindow( dpy, DefaultRootWindow(dpy), 0, 0,
				         200, 100, 0, blackColor, blackColor );
  // "Map" the window (that is, make it appear on the screen)
  XMapWindow(dpy, w);
  // Create a "Graphics Context"
  GC gc = XCreateGC(dpy, w, 0, NULL );
  GC gcXor = XCreateGC(dpy, w, 0, NULL );
  XSetFunction( dpy, gcXor, GXxor );
  // Tell the GC we draw using the white color
  XSetForeground(dpy, gc, whiteColor);
  XSetForeground(dpy, gcXor, whiteColor);

// We want to get Expose & ButtonPress & ButtonRelease & ButtonMotion events
XSelectInput(dpy,w,ExposureMask|PointerMotionMask | ButtonPressMask |ButtonReleaseMask);
short done = 0; short drawing = 0;
int x = 0; int y = 0; int xTmp = 0; int yTmp = 0;	
while( ! done ){
   XEvent e;
   XNextEvent( dpy, & e ); // Get event
   switch( e.type ){
     case ButtonPress :
     switch( e.xbutton.button ){
        case Button1 :
            x = xTmp = e.xbutton.x; y = yTmp = e.xbutton.y; drawing = 1;
            XDrawLine( dpy, w, gcXor, x, y, xTmp, yTmp );
            break;
        case Button2 :
        case Button3 :
           done = 1;
           break;
     }//switch
     break;
     case ButtonRelease :
     switch( e.xbutton.button ){
         case Button1 :
            if( drawing ){
               drawing = 0;
               XDrawLine( dpy, w, gcXor, x, y, xTmp, yTmp );
               //XDrawLine( dpy, w, gc, x, y, e.xbutton.x, e.xbutton.y );
               //XFlush( dpy );
               Line * l = new Line;
               l->x1 = x; l->y1 = y;
               l->x2 = e.xbutton.x; l->y2 = e.xbutton.y;
               l->next = lines; lines = l;
               l->draw( dpy, w, gc );
               XFlush( dpy );
            }//block
            break;
     }//switch
     break;
case MotionNotify :
   if( drawing && (e.xmotion.state & Button1Mask) )
   {
      XDrawLine( dpy, w, gcXor, x, y, xTmp, yTmp );
      //xTmp = e.xmotion.x;
      //yTmp = e.xmotion.y;
      XDrawLine( dpy, w, gcXor, x, y, xTmp, yTmp );
   }//if
   break;
case Expose :
   if( e.xexpose.count == 0 )
   {
      // Draw lines
     Line * l = lines;
     while( l != NULL )
     {
        l->draw( dpy, w, gc );
        l = l->next;
     }//while
     XDrawArc(dpy, w, gc, 100, 100, 50, 50, 0 , 64*359);
     
     // Send requests to the server
     XFlush( dpy );
   }//if
   break;
 }//switch
}//while

XFreeGC( dpy, gc ); // free GC
XFreeGC( dpy, gcXor );
XDestroyWindow( dpy, w ); // destroy window
XCloseDisplay( dpy ); // Close connection to the display
}
