#include "hextile.h"

RectUpdate::RectUpdate(unsigned char *bbuf, int llen, int xx, int yy, int ww, int hh) :
    buf(NULL), x(xx), y(yy), w(ww), h(hh)
{
    buf = new unsigned char [len];
    memcpy(buf, bbuf, llen);
}

RectUpdate::~RectUpdate()
{
    delete [] buf;
}

void
RectUpdate::decode(unsigned char *screen, int screen_width, int screen_height)
{
    int start = 3*(y*screen_width + x);
    for (int i = 0; i < h; i++) {
        unsigned char *screenp = screen + start + i*screen_width*3;
        for (int j = 0; j < w; j++) {
            *screenp++ = *buf++;
            *screenp++ = *buf++;
            *screenp++ = *buf++;
        }
    }
}

ColorRect::ColorRect(unsigned int ccolor, int xx, int yy, int ww, int hh)
    : color(color), x(xx), y(yy), w(ww), h(hh) {}

void
ColorRect::decode(unsigned char *screen, int screen_width, int screen_height)
{
    int start = 3*(y*screen_width + x);
    int r = (color >> 16) & 0xff;
    int g = (color >> 8) & 0xff;
    int b = (color) & 0xff;
    for (int i = 0; i < h; i++) {
        unsigned char *screenp = screen + start + i*screen_width*3;
        for (int j = 0; j < w; j++) {
            *screenp++ = r;
            *screenp++ = g;
            *screenp++ = b;
        }
    }
}

FgBgRect::FgBgRect(unsigned int fg_color, unsigned int bg_color, int xx, int yy, int ww, int hh)
{

}

void
FgBgRect::decode(unsigned char *screen, int screen_width, int screen_height)
{

}

HextileDecoder::HextileDecoder(int wwidth, int hheight) :
    width(wwidth), height(hheight), screen(NULL), bg_color(0), fg_color(0) {}

HextileDecoder::~HextileDecoder()
{
    for (VectorUpdates::iterator i = updates.begin(); i != updates.end(); ++i)
    {
        delete *i;
    }
    delete [] screen;
}

void
HextileDecoder::rect(unsigned char *bbuf, int xx, int yy, int ww, int hh)
{
    Update *update = new RectUpdate(bbuf, xx, yy, ww, hh);
    updates.push(update);
}

void
HextileDecoder::color_rect(unsigned int ccolor, int xx, int yy, int ww, int hh)
{
    Update *update = new ColorRect(ccolor, xx, yy, ww, hh);
    updates.push(update);
}

void
HextileDecoder::fgbg_rect(int xx, int yy, int ww, int hh)
{
    Update *update = new FgBgRect(fg_color, bg_color, int xx, int yy, int ww, int hh);
    updates.push(update);
}

void
HextileDecoder::set_background(unsigned int ccolor)
{
    bg_color = ccolor;
}

void
HextileDecoder::set_foreground(unisnged int ccolor)
{
    fg_color = color;
}

void
HextileDecoder::decode()
{
    if (!screen)
        screen = new unsigned char [width*height*3];

    for (VectorUpdates::iterator i = updates.begin(); i != updates.end(); ++i)
    {
        i->decode(screen);
    }
}

const unsigned char *
HextileDecoder::get() const
{
    return screen;
}

int
HextileDecoder::get_len() const
{
    return width*height*3;
}

