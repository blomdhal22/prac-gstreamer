#!/bin/sh

##
# Simple gstreamer pipeline to record a video captured from a webcam (through v4l) to OGG file while
# showing the video also on the screen.
#
# Sample usage:
#
# Record to webcam.ogg:
#
#   ./camrec.sh
#
# Record to output.ogg:
#
#   ./camrec.sh output.ogg
#
# To adjust capture width/hight or set the V4L device see variables below.
#
# Pipeline:
#
# Capture video from V4L source, convert to ffmpegcolorspace, split result (with tee and two queues)
# to two separate path. One shows currently captured video on screen (xvimagesink), the other one 
# encodes the captured video to OGG-container with theoraenc and writes the result to a file (filesink). 
#
#                                                                      +-------------+
#                                                                   +--| xvimagesink |
#                                                                 [Q]  +-------------+
# +---------+  +-----------------+  +------------------+  +-----+ /
# | v4l2src |--| video/x-raw-yuv |--| ffmpegcolorspace |--| tee |
# +---------+  +-----------------+  +------------------+  +-----+ \
#                                                                 [Q]  +-----------+  +--------+  +----------+
#                                                                   +--| theoraenc |--| oggmux |--| filesink |
#                                                                      +-----------+  +--------+  +----------+
# 2012-11-19, sw@kaltpost.de, http://gpio.kaltpost.de
##

##
# --- Settings
##

# Output file 
OUTPUT=$1

# capture device to use
V4LDEV="/dev/video0"

# capture width
WIDTH=1280

# capture height
HEIGHT=720

# bitrate for theoraenc
BITRATE=256

##
# --- END of settings
##


if [ "X$1" = "X" ]
then
   OUTPUT="webcam.mp4"
fi

echo "**"
echo "* Starting recording from webcam@${V4LDEV} to: ${OUTPUT}@${WIDTH}x${HEIGHT}"
echo "* Using OGG-container and THEORA-encoding with bitrate: ${BITRATE}" 
echo "* Press CTRL+C to end recording"
echo "**"

#
# -e switch makes gst to close the OGG container properly when exited with CTRL+C
#
# gst-launch-1.0 -e \
#    v4l2src device=${V4LDEV} ! video/x-raw-yuv,format=\(fourcc\)YUY2,width=${WIDTH},height=${HEIGHT} ! \
#    ffmpegcolorspace ! \
#    tee name="splitter" ! queue ! xvimagesink sync=false splitter. ! \
#    queue ! videorate ! video/x-raw-yuv ! \
#    theoraenc bitrate=${BITRATE} ! oggmux ! filesink location=${OUTPUT}

gst-launch-1.0 -e \
   v4l2src device=${V4LDEV} ! video/x-raw,framerate=30/1,width=${WIDTH},height=${HEIGHT} ! \
   tee name="splitter" ! queue ! xvimagesink sync=false splitter. ! \
   queue ! videorate ! video/x-raw , framerate=30/1 ! \
   theoraenc bitrate=${BITRATE} ! mp4mux ! filesink location=${OUTPUT}
