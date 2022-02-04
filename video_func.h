#pragma once

#include "video/video_frame.h"

#include <QtCore>
#include <QImage>

namespace qutils {

QImage cvMatToQImage(const video::Frame::Ptr& frame);

} // namespace qutils
