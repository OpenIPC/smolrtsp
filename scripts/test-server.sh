#!/bin/bash

mkdir examples/build -p
cd examples/build
cmake ..
cmake --build .
./server &

# To avoid missing some RTP packets.
sleep 1s

ffmpeg -y -i rtsp://localhost -vcodec copy test.h264 -acodec copy -f mulaw test.g711a
cmp test.h264 ../media/video.h264
cmp test.g711a ../media/audio.g711a

echo "The server is ok."
