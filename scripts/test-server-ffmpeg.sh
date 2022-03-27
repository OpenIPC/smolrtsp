#!/bin/bash

set -e

ffmpeg -y -rtsp_transport $1 -i rtsp://localhost -vcodec copy test_$1.h264 -acodec copy -f mulaw test_$1.g711a
cmp test_$1.h264 examples/media/video.h264
cmp test_$1.g711a examples/media/audio.g711a

echo "Server '$1' is ok."
