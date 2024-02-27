#pragma once

#ifdef VIDEO_CAPTURE
#include "video/video_frame.h"
#endif

#include <opencv2/opencv.hpp>
#include <QtCore>
#include <QImage>

namespace qutils {

QImage cvMatToQImage(const cv::Mat&);

#ifdef VIDEO_CAPTURE
QImage cvMatToQImage(const video::Frame::Ptr& frame);
#endif

} // namespace qutils
