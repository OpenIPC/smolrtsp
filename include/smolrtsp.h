/**
 * @file
 * @brief Re-exports all the functionality.
 */

/**
 * @mainpage
 *
 * A small, portable, extensible RTSP 1.0 implementation in C99.<br>
 *
 * See the <a href="files.html">file list</a> for the available abstractions.
 * See <a href="https://github.com/OpenIPC/smolrtsp">our official GitHub
 * repository</a> for a high-level overview.
 */

#pragma once

#include <smolrtsp/types/error.h>
#include <smolrtsp/types/header.h>
#include <smolrtsp/types/header_map.h>
#include <smolrtsp/types/message_body.h>
#include <smolrtsp/types/method.h>
#include <smolrtsp/types/reason_phrase.h>
#include <smolrtsp/types/request.h>
#include <smolrtsp/types/request_line.h>
#include <smolrtsp/types/request_uri.h>
#include <smolrtsp/types/response.h>
#include <smolrtsp/types/response_line.h>
#include <smolrtsp/types/rtp.h>
#include <smolrtsp/types/rtsp_version.h>
#include <smolrtsp/types/sdp.h>
#include <smolrtsp/types/status_code.h>

#include <smolrtsp/context.h>
#include <smolrtsp/controller.h>
#include <smolrtsp/droppable.h>
#include <smolrtsp/io_vec.h>
#include <smolrtsp/nal.h>
#include <smolrtsp/nal_transport.h>
#include <smolrtsp/option.h>
#include <smolrtsp/rtp_transport.h>
#include <smolrtsp/transport.h>
#include <smolrtsp/util.h>
#include <smolrtsp/writer.h>
