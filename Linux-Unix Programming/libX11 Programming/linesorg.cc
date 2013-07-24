#include <X11/Xlib.h>
#include <unistd.h>
#include <stdlib.h>

void custom_XDrawRectangle(Display* display, Drawable d, GC gc, int x1, int y1, unsigned int x2, unsigned int y2){
    int temp;
    if(x2<x1){ temp=x1; x1=x2; x2=temp; }
    if(y2<y1){ temp=y1; y1=y2; y2=temp; }
    XDrawRectangle(display, d, gc, x1, y1, x2-x1, y2-y1);
}



class Line
{
public:
   int x1; int y1; int x2; int y2;
   Line * next;
   Line()
   {
      x1 = y1 = x2 = y2 = 0;
      next = NULL;
   }
   void draw( Display * display, Window window, GC gc )
   {
      XDrawLine( display, window, gc, x1, y1, x2, y2 );
   }
};

class Rectangle: public Line{
public:
    Rectangle* next;
    Rectangle(){
	x1 = y1 = x2 = y2 = 0;
	next=NULL;
    }
    
    void draw( Display* display, Window window, GC gc ){
	custom_XDrawRectangle(display, window, gc, x1, y1, x2, y2);

    }
};



main()
{
   Line * lines = NULL;
   Rectangle* rectangles = NULL;

   int firstLineNotDrawn=1;
   Line* firstLine;

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
	    x = xTmp = e.xbutton.x; y = yTmp = e.xbutton.y; drawing = 1;

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
	       if ( firstLineNotDrawn ){
		  firstLine=l;
		  firstLineNotDrawn=0;
	       }
	       else{
		    firstLine->next=l;
	       }
               l->x1 = x; l->y1 = y;
               l->x2 = e.xbutton.x; l->y2 = e.xbutton.y;
               l->next = lines; lines = l;
               l->draw( dpy, w, gc );
               XFlush( dpy );

            }//block
            break;
	case Button2 :
            if( drawing ){
               drawing = 0;
               custom_XDrawRectangle( dpy, w, gcXor, x, y, xTmp, yTmp );
               //XDrawLine( dpy, w, gc, x, y, e.xbutton.x, e.xbutton.y );
               //XFlush( dpy );
               Rectangle * r = new Rectangle ;
               r->x1 = x; r->y1 = y;
               r->x2 = e.xbutton.x; r->y2 = e.xbutton.y;
               r->next = rectangles; rectangles = r;
               r->draw( dpy, w, gc );
               XFlush( dpy );
	    }
     }//switch
     break;
case MotionNotify :
   if( drawing && (e.xmotion.state & Button1Mask) )
   {
      XDrawLine( dpy, w, gcXor, x, y, xTmp, yTmp );
      xTmp = e.xmotion.x;
      yTmp = e.xmotion.y;
      XDrawLine( dpy, w, gcXor, x, y, xTmp, yTmp );
   }//if
   else if( drawing && (e.xmotion.state & Button2Mask) )
   {
      custom_XDrawRectangle( dpy, w, gcXor, x, y, xTmp, yTmp );
      xTmp = e.xmotion.x;
      yTmp = e.xmotion.y;
      custom_XDrawRectangle( dpy, w, gcXor, x, y, xTmp, yTmp );
   }
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

     Rectangle * r = rectangles;
     while( r!=NULL ){
	r->draw( dpy, w, gc );
	r = r->next;
     }     

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
