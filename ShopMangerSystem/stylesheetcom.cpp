#include "stylesheetcom.h"

Rgb::Rgb()
{
    m_r=0;m_g=0;m_b=0;
}

Rgb::Rgb(int r,int g,int b)
{
    m_r=r;
    m_g=g;
    m_b=b;
}

StyleSheetCom::StyleSheetCom()
{
    m_qss_border="border:%1px outset rgb(%2,%3,%4);";
    m_qss_front= "color:rgb(%1,%2,%3);";
    m_redius=    "border-radius: %1px;";

    m_border_width=3;
    m_redius_value=2;
}

void StyleSheetCom::setBorderColor(Rgb rgb)
{
    m_border_color.m_b=rgb.m_b;
    m_border_color.m_g=rgb.m_g;
    m_border_color.m_r=rgb.m_r;
}

void StyleSheetCom::setBorderWidth(int width)
{
    m_border_width=width;
}

void StyleSheetCom::setFontColor(Rgb rgb)
{
    m_font_color.m_b=rgb.m_b;
    m_font_color.m_g=rgb.m_g;
    m_font_color.m_r=rgb.m_r;
}

void StyleSheetCom::setRedius(int radius)
{
    m_redius_value=radius;
}

QString StyleSheetCom::getBorderQss()
{
    return m_qss_border.arg(m_border_width).arg(m_border_color.m_r)
                                           .arg(m_border_color.m_g)
                                           .arg(m_border_color.m_b);
}

QString StyleSheetCom::getFontQss()
{
    return m_qss_front.arg(m_border_width).arg(m_font_color.m_r)
                                           .arg(m_font_color.m_g)
                                           .arg(m_font_color.m_b);
}

QString StyleSheetCom::getButtonRadius()
{
    return m_redius.arg(m_redius_value);
}

QString StyleSheetCom::getQss()
{
    return getBorderQss()+getFontQss()+getButtonRadius();
}
