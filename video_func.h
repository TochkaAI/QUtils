#pragma once

#include "video/video_frame.h"

#include <opencv2/opencv.hpp>
#include <QtCore>
#include <QImage>

namespace qutils {

QImage cvMatToQImage(const cv::Mat&);
QImage cvMatToQImage(const video::Frame::Ptr& frame);

} // namespace qutils
