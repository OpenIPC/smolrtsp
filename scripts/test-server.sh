#!/bin/bash

set -e

mkdir examples/build -p
cd examples/build
cmake ..
cmake --build .
./server &
server_pid=$!

sleep 2s

cd ../..

bash scripts/test-server-ffmpeg.sh tcp &
tcp_pid=$!
bash scripts/test-server-ffmpeg.sh udp &
udp_pid=$!

wait $tcp_pid
wait $udp_pid
kill $server_pid
