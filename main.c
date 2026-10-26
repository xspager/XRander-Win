#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>

int main(int argc, char *argv[])
{
    Display *dp = XOpenDisplay(NULL);
    if (dp == NULL) {
        fprintf(stderr, "Can't open display\n");
        return EXIT_FAILURE;
    }
    Window win = XCreateSimpleWindow(dp, XDefaultRootWindow(dp),
        0, 0,
        800, 600,
        0, 0, 0);
    XWindowAttributes wa = {0};
    XGetWindowAttributes(dp, win, &wa);

    GC gc = XCreateGC(dp, win, 0, NULL);
    Atom wm_delete_window = XInternAtom(dp, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(dp, win, &wm_delete_window, 1);
    XStoreName(dp, win, "Hello");
    XMapWindow(dp, win);

    int event_base, error_base;
    if (!XRenderQueryExtension(dp, &event_base, &error_base)){
        fprintf(stderr, "xrender not present\n");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "xrender event_base = %d error_base = %d\n", event_base, error_base);

    int major_version, minor_version;
    if (!XRenderQueryVersion(dp, &major_version, &minor_version)){
        fprintf(stderr, "xrender not present\n");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "xrender %d.%d\n", major_version, minor_version);
    Picture pic;

    XRenderPictFormat *pictf = XRenderFindStandardFormat(dp, PictStandardRGB24);
    //XRenderPictFormat *pictf = XRenderFindVisualFormat(dp, wa.visual);
    pic = XRenderCreatePicture(dp, win, pictf, 0, NULL);

    XRenderColor pink;
    if (!XRenderParseColor(dp, "pink", &pink)){
        fprintf(stderr, "Fail to get pink color xD\n");
        return EXIT_FAILURE;
    }

    XFilters *filters = XRenderQueryFilters(dp, win);
    XTransform transform = {{
        {XDoubleToFixed(1), XDoubleToFixed(0), XDoubleToFixed(0)},
        {XDoubleToFixed(0), XDoubleToFixed(-1), XDoubleToFixed(0)},
        {XDoubleToFixed(0), XDoubleToFixed(0), XDoubleToFixed(1)},
    }};
    XRenderSetPictureTransform(dp, pic, &transform);
    char pixelmap[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    //Pixmap img = XCreatePixmapFromBitmapData(dp, win, pixelmap,
    //    1, 2,
    //    0, 0,
    //    24
    //);
    //XPutImage(dp, win, gc, img, 0,0,0,0, 1,2);
    XColor color;
    //XParseColor(dp, win->colormap, "blue", &color);
    //XSetWindowBackground(dp, win, color->pixel);
    
    bool quit = False;

    while (!quit) {
        while (XPending(dp) > 0){
            XEvent event = {0};
            XNextEvent(dp, &event);
            switch(event.type) {
                case ClientMessage:
                    if ((Atom) event.xclient.data.l[0] == wm_delete_window){
                        quit = true;
                        break;
                    }
                default:
                    printf("Event %i\n", event.type);
            }
        }
        //XRenderChangePicture(
        XRenderFillRectangle(dp, PictOpSrc, pic, &pink, 0,0,800,600);
        //XRenderCreateSolidFill(dp, &pink);
        //XRenderComposite(dp, PictOpSrc, pic, 0, NULL
    }
    return EXIT_SUCCESS;
}
