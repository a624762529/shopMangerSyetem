#ifndef STYLESHEETCOM_H
#define STYLESHEETCOM_H
#include"head.h"
#include<QString>
#include<QRgb>
/*
    1.设置字体颜色
    2.设置button边框
    3.设置渐变
    4.设置渐变结束
*/
class Rgb
{
public:
    Rgb();
    Rgb(int r,int g,int b);
public:
    int m_r;
    int m_g;
    int m_b;
};
class StyleSheetCom
{
public:
    StyleSheetCom();


public:
    void setBorderColor(Rgb);
    void setBorderWidth(int width);
    void setFontColor(Rgb);
    void setRedius(int radius);
    QString getQss();
private:
    QString getBorderQss();
    QString getFontQss();
    QString getButtonRadius();
private:
    QString m_qss_border;          //设置边框
    Rgb     m_border_color;        //边框具体的颜色
    int     m_border_width;        //设置边框宽度

    QString m_qss_front;           //设置字体
    Rgb     m_font_color;          //字体的颜色

    QString m_redius;
    int     m_redius_value;
};

#endif // STYLESHEETCOM_H
