#!/bin/bash

# Path to the video location
VIDEO=$1

# Make sure there is no pipeline with this name already
gstd-client pipeline_delete p

# gstd-client pipeline_create p videotestsrc is-live=true pattern=ball ! x264enc ! mp4mux ! filesink location=$VIDEO
# gstd-client pipeline_create p v4l2src device=/dev/video0 ! video/x-raw, framerate=30/1 ! videoconvert ! queue ! x264enc ! mp4mux ! filesink location=/tmp/v.mp4
gstd-client pipeline_create p v4l2src device=/dev/video0 ! video/x-raw, framerate=30/1 ! videoconvert ! queue ! x264enc ! mp4mux ! filesink location=$VIDEO
gstd-client pipeline_play p

# Wait for user interruption
echo "Press enter to finish file"
read

# Send the EOS event to avoid corruption
gstd-client event_eos p
gstd-client bus_filter p eos 
gstd-client bus_read p
gstd-client pipeline_delete p

echo "Video recorded to \"${VIDEO}\""