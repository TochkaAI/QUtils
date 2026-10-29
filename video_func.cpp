/* clang-format off */
/*****************************************************************************
  The MIT License

  Copyright © 2026 TochkaAI

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*****************************************************************************/

#include "qutils/video_func.h"

#include "shared/logger/logger.h"
#include "shared/logger/format.h"
#include "shared/qt/logger_operators.h"

#define log_error_m   alog::logger().error   (alog_line_location, "VideoFunc")
#define log_warn_m    alog::logger().warn    (alog_line_location, "VideoFunc")
#define log_info_m    alog::logger().info    (alog_line_location, "VideoFunc")
#define log_verbose_m alog::logger().verbose (alog_line_location, "VideoFunc")
#define log_debug_m   alog::logger().debug   (alog_line_location, "VideoFunc")
#define log_debug2_m  alog::logger().debug2  (alog_line_location, "VideoFunc")

namespace qutils {

/**
  Source: https://asmaloney.com/2013/11/code/converting-between-cvmat-and-qimage-or-qpixmap/
*/
QImage cvMatToQImage(const cv::Mat& image)
{
    switch (image.type())
    {
        case CV_8UC4: // 8-bit, 4 channel
        {
            return QImage((uchar*)image.data,
                          int(image.cols), int(image.rows), int(image.step),
                          QImage::Format_RGB32
                         ).copy(); // Обязательно делать копию QImage
        }
        case CV_8UC3: // 8-bit, 3 channel
        {
            return QImage((uchar*)image.data,
                          int(image.cols), int(image.rows), int(image.step),
                          QImage::Format_RGB888
                         ).rgbSwapped(); // Здесь уже возвращается копия QImage
        }
        case CV_8UC2: // YUYV
        {
            cv::Mat frm;
            cvtColor(image, frm, cv::COLOR_YUV2BGR_UYVY);
            return QImage((uchar*)frm.data,
                          int(frm.cols), int(frm.rows), int(frm.step),
                          QImage::Format_RGB888
                         ).rgbSwapped(); // Здесь уже возвращается копия QImage
        }
//        case CV_16UC2: // YUYV
//        {
//            int t = frame->data.type();

//            cv::Mat frm {frame->data.rows, frame->data.cols, CV_16UC3};
//            try
//            {
//                cvtColor(frame->data, frm, cv::COLOR_YUV2RGB_UYVY);
//            }
//            catch (const std::exception& e)
//            {
//                log_error << "cvtColor: " << e.what();
//                alog::logger().flush();
//            }

//            int tp = frm.type();
//            int t1 = CV_8UC3;
//            int t2 = CV_16UC3;

//            return QImage((uchar*)frm.data,
//                          int(frm.cols), int(frm.rows), int(frm.step),
//                          QImage::Format_RGB888
//                          ).rgbSwapped(); // Здесь уже возвращается копия QImage
//        }
        case CV_8UC1: // 8-bit, 1 channel
        {
           static QVector<QRgb>  sColorTable;
           // only create our color table once
           if (sColorTable.isEmpty())
              for (int i = 0; i < 256; ++i)
                 sColorTable.push_back(qRgb(i, i, i));

            QImage img((uchar*)image.data,
                       int(image.cols), int(image.rows), int(image.step),
                       QImage::Format_Indexed8);
           img.setColorTable(sColorTable);
           return img.copy(); // Обязательно делать копию QImage
        }
        default:
           log_warn_m << "cv::Mat image type not handled; type: " << image.type();
    }
    return QImage();
}

#ifdef VIDEO_CAPTURE
QImage cvMatToQImage(const video::Frame::Ptr& frame)
{
    return cvMatToQImage(frame->data);
}
#endif

} // namespace qutils
