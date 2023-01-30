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
        case CV_16UC1:
        {
            static QVector<QRgb>  sColorTable;
            // only create our color table once
            if (sColorTable.isEmpty())
            for (int i = 0; i < 256; ++i)
                sColorTable.push_back(qRgb(i, i, i));

            cv::Mat convertImage;
            image.convertTo(convertImage, CV_8UC1, 0.5);

            QImage img((uchar*)convertImage.data, int(convertImage.cols), int(convertImage.rows), int(convertImage.step), QImage::Format_Indexed8);
            img.setColorTable(sColorTable);
            return img.copy();
        }
        break;
        default:
           log_warn_m << "cv::Mat image type not handled; type: " << image.type();
    }
    return QImage();
}

QImage cvMatToQImage(const video::Frame::Ptr& frame)
{
    return cvMatToQImage(frame->data);
}

} // namespace qutils
