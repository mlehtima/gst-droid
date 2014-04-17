/*
 * gst-droid
 *
 * Copyright (C) 2014 Mohammed Sameer <msameer@foolab.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __GST_DROID_CAM_SRC_H__
#define __GST_DROID_CAM_SRC_H__

#include <gst/gst.h>
#include "gstdroidcamsrcdev.h"
#include "gstdroidcamsrcenums.h"
#include <hardware/camera.h>
#include <gst/meta/nemometa.h>
#ifndef GST_USE_UNSTABLE_API
#define GST_USE_UNSTABLE_API
#endif /* GST_USE_UNSTABLE_API */
#include <gst/basecamerabinsrc/gstcamerabin-enum.h>
#include <gst/basecamerabinsrc/gstbasecamerasrc.h>
#include <gst/interfaces/photography.h>

G_BEGIN_DECLS

#define GST_TYPE_DROIDCAMSRC \
  (gst_droidcamsrc_get_type())
#define GST_DROIDCAMSRC(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), GST_TYPE_DROIDCAMSRC, GstDroidCamSrc))
#define GST_DROIDCAMSRC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_DROIDCAMSRC, GstDroidCamSrcClass))
#define GST_IS_DROIDCAMSRC(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_DROIDCAMSRC))
#define GST_IS_DROIDCAMSRC_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_DROIDCAMSRC))

#define MAX_CAMERAS 2
#define GST_DROIDCAMSRC_CAPTURE_START "photo-capture-start"
#define GST_DROIDCAMSRC_CAPTURE_END "photo-capture-end"
#define GST_DROIDCAMSRC_PREVIEW_IMAGE "photo-capture-preview"

typedef struct _GstDroidCamSrc GstDroidCamSrc;
typedef struct _GstDroidCamSrcClass GstDroidCamSrcClass;
typedef struct _GstDroidCamSrcCamInfo GstDroidCamSrcCamInfo;
typedef struct _GstDroidCamSrcPad GstDroidCamSrcPad;

typedef gboolean (* GstDroidCamSrcNegotiateCallback)(GstDroidCamSrcPad * pad);

struct _GstDroidCamSrcCamInfo
{
  int num;
  NemoGstDeviceDirection direction;
  NemoGstBufferOrientation orientation;
};

struct _GstDroidCamSrcPad
{
  GstPad *pad;
  GQueue *queue;
  GCond cond;
  GMutex lock;
  gboolean running;
  gboolean open_stream;
  gboolean open_segment;
  gboolean adjust_segment;
  gboolean send_flush_stop;
  gboolean capture_pad;
  unsigned pushed_buffers;
  GstSegment segment;
  GstDroidCamSrcNegotiateCallback negotiate;
};

struct _GstDroidCamSrc
{
  GstElement parent;

  camera_module_t *hw;
  GstDroidCamSrcDev *dev;
  GstDroidCamSrcCamInfo info[MAX_CAMERAS];

  GstDroidCamSrcPad *vfsrc;
  GstDroidCamSrcPad *imgsrc;
  GstDroidCamSrcPad *vidsrc;

  GstDroidCamSrcCameraDevice camera_device;
  GstDroidCamSrcCameraDevice user_camera_device;
  GstCameraBinMode mode;

  int captures;
  GMutex capture_lock;

  GstPhotographySettings photo;
};

struct _GstDroidCamSrcClass
{
  GstElementClass parent_class;
};

GType gst_droidcamsrc_get_type (void);
void gst_droidcamsrc_post_message (GstDroidCamSrc * src, GstStructure * s);
void gst_droidcamsrc_timestamp (GstDroidCamSrc * src, GstBuffer * buffer);

G_END_DECLS

#endif /* __GST_DROID_CAM_SRC_H__ */
