#include "img.h"
#include "vk.h"

img::img(int sx, int sy)
{
    this->im = gdImageCreateTrueColor(sx, sy);
}

img::img(gdImagePtr New)
{
    this->im = New;
}

#include "str.h"
img::img(Doc doc, Net& net)
{
    this->im = NULL;
    std::string buff = net.send(doc.url);
    if (str::low(doc.ext) == "jpg" || str::low(doc.ext) == "jpeg")
        this->im = gdImageCreateFromJpegPtr(buff.size(), (void*)buff.c_str());
    else if (str::low(doc.ext) == "png")
        this->im = gdImageCreateFromPngPtr(buff.size(), (void*)buff.c_str());
    else if (str::low(doc.ext) == "bmp")
        this->im = gdImageCreateFromBmpPtr(buff.size(), (void*)buff.c_str());
}

std::string img::getPng()
{
    int s;
    //void* png = gdImagePngPtrEx(this->im, &s, 0);
    void* png = gdImagePngPtr(this->im, &s);
    std::string buff((const char*)png, s);
    if (png)
        gdFree(png);
    return buff;
}

Doc img::getDoc(uint32_t peer_id, Net& net, Vk& vk)
{
    if (this->im == NULL)
        return Doc();
    std::string dat = this->getPng();
    Doc doc;
    if (doc.uploadDoc("img.png", dat, net, vk, peer_id))
        return doc;
    return doc;
}

Doc img::getPhoto(uint32_t peer_id, Net& net, Vk& vk)
{
    if (this->im == NULL)
        return Doc();
    std::string dat = this->getPng();
    Doc doc;
    if (doc.uploadPhoto("img.png", dat, net, vk, peer_id))
        return doc;
    return doc;
}

img::~img()
{
    if (this->im)
        gdImageDestroy(this->im);
}

img img::copy()
{
    return img(gdImageClone(this->im));
}

Doc img::CVtoPhoto(cv::Mat matIm, uint32_t peer_id, Net& net, Vk& vk)
{
    std::vector<uint8_t> buff;
    cv::imencode(".png", matIm, buff, {cv::IMWRITE_JPEG_PROGRESSIVE, 1});
    std::string imBuff(buff.begin(), buff.end());
    Doc doc;
    if (doc.uploadPhoto("img.png", imBuff, net, vk, peer_id))
        return doc;
    return doc;
}

Doc img::CVtoDoc(cv::Mat matIm, uint32_t peer_id, Net& net, Vk& vk)
{
    std::vector<uint8_t> buff;
    cv::imencode(".png", matIm, buff, {cv::IMWRITE_JPEG_PROGRESSIVE, 1});
    std::string imBuff(buff.begin(), buff.end());
    Doc doc;
    if (doc.uploadDoc("img.png", imBuff, net, vk, peer_id))
        return doc;
    return doc;
}

cv::Mat img::getCVim()
{
    std::string data = this->getPng();
    std::vector<uint8_t> vectordata(data.begin(),data.end());
    cv::Mat data_mat(vectordata,true);
    return cv::imdecode(data_mat,1);
}
