# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## Unreleased

### Added

 - Per-codec compile-time gates: `SMOLRTSP_WITH_H264`, `SMOLRTSP_WITH_H265`, `SMOLRTSP_WITH_H266`, `SMOLRTSP_WITH_AV1`, `SMOLRTSP_WITH_JPEGXS`. Every option defaults to `ON` (no behaviour change for existing consumers). Switching one off removes the matching `.c` source from the build, the matching header from the umbrella `<smolrtsp.h>`, and -- for the H.26x family -- the matching `SmolRTSP_NalHeader` variant plus the per-codec arms in `SmolRTSP_NalTransportConfig`, `SmolRTSP_NalHeader_*`, and `SmolRTSP_NalTransport_send_packet`. At least one of `SMOLRTSP_WITH_H264 / _H265 / _H266` must remain `ON` (enforced by `FATAL_ERROR` at configure time). Each enabled option becomes a `PUBLIC` compile definition so callers see the same gates as the library. Lets downstream firmwares strip codecs no shipped sensor can produce -- OpenIPC's `hi3516ev200` lite build drops 4 KB of stripped binary with H.266 + AV1 + JPEG XS disabled.

## 0.2.0 - 2026-05-24

### Added

 - H.266 / VVC RTP payload format per [RFC 9328](https://datatracker.ietf.org/doc/html/rfc9328). New header `smolrtsp/nal/h266.h` (2-byte NAL header struct, FU header, all VVC NAL unit type constants from H.266 §7.4.2.2) and a third variant in `SmolRTSP_NalHeader` -- VVC NALs flow through the existing `SmolRTSP_NalTransport` like H.264 / H.265, sharing the FU fragmentation logic. `SmolRTSP_NalTransportConfig` gains a `max_h266_nalu_size` field (defaults to 1200). The example server gains an opt-in `/vvc` stream behind the `ENABLE_VVC` CMake option.
 - AV1 RTP payload format per the [AOMedia AV1 RTP Specification](https://aomediacodec.github.io/av1-rtp-spec/). New headers `smolrtsp/av1.h` (1-byte §4.4 aggregation header, LEB128 helpers, OBU constants) and `smolrtsp/av1_transport.h` (`SmolRTSP_Av1Transport`, a thin packetizer over `SmolRTSP_RtpTransport`). `send_temporal_unit` parses a raw OBU stream, drops Temporal Delimiter and Tile List OBUs, clears `obu_has_size_field`, and emits one OBU per RTP packet (W=1) with Z/Y continuation flags on cross-packet fragments. The example server gains an opt-in `/av1` stream behind the `ENABLE_AV1` CMake option.
 - JPEG XS RTP payload format per [RFC 9134](https://datatracker.ietf.org/doc/html/rfc9134). New headers `smolrtsp/jpegxs.h` (4-byte RFC 9134 §4.3 payload header struct + serializer) and `smolrtsp/jpegxs_transport.h` (`SmolRTSP_JpegXsTransport`, a thin packetizer over `SmolRTSP_RtpTransport`). Both codestream (`K=0`) and slice (`K=1`) packetization modes are supported, with progressive and interlaced framing, and a configurable MTU. The example server gains an opt-in `/jpegxs` stream behind the `ENABLE_JPEGXS` CMake option.
 - RTCP Sender Report (SR) serialization per [RFC 3550 §6.4.1](https://datatracker.ietf.org/doc/html/rfc3550#section-6.4.1). New header `smolrtsp/types/rtcp.h` exposing `SmolRTSP_RtcpSr`, `SmolRTSP_RtcpSr_size`, and `SmolRTSP_RtcpSr_serialize`. Currently emits the fixed 28-byte SR header (RC = 0).
 - RTCP Receiver Report (RR), SDES with single CNAME item, and BYE serialization. New types `SmolRTSP_RtcpRr`, `SmolRTSP_RtcpSdesCname`, `SmolRTSP_RtcpBye` with matching `_size` / `_serialize` functions in `smolrtsp/types/rtcp.h`. SDES and BYE handle the variable-length item-list padding to 32-bit boundaries automatically.
 - `SmolRTSP_RtpTransport_ssrc`, `SmolRTSP_RtpTransport_pkt_count`, and `SmolRTSP_RtpTransport_octet_count` accessors so callers can populate the corresponding fields of an RTCP Sender Report. `pkt_count` and `octet_count` are now tracked inside `SmolRTSP_RtpTransport` and advance with each successful `_send_packet` call.
 - `SmolRTSP_RtpTransport_new_with_ssrc` — a constructor that accepts an explicit SSRC instead of generating one via `rand()`. The plain `_new` is now a thin wrapper that supplies `rand()` as the SSRC.

### Fixed

 - Update the minimum required CMake version to 3.10.0 due to deprecation (see [metalang99/issues/33](https://github.com/hirrolot/metalang99/issues/33)).
 - Fix the `SmolRTSP_NalTransportConfig_default` value for H.265 ([PR #17](https://github.com/OpenIPC/smolrtsp/pull/17)).

## 0.1.3 - 2023-03-12

### Fixed

 - Fix the `DOWNLOAD_EXTRACT_TIMESTAMP` CMake warning (see [datatype99/issues/15](https://github.com/hirrolot/datatype99/issues/15)).

## 0.1.2 - 2022-07-27

### Fixed

 - Suppress a compilation warning for an unused variable in `smolrtsp_vheader`.
 - Overflow while computing an RTP timestamp.

## 0.1.1 - 2022-03-31

### Fixed

 - Mark the following functions with `__attribute__((warn_unused_result))` (when available):
   - `SmolRTSP_ParseError_print`.
   - `SmolRTSP_MessageBody_empty`.
   - `SmolRTSP_Request_uninit`.
   - `SmolRTSP_Response_uninit`.
   - `SmolRTSP_NalTransportConfig_default`.
   - `smolrtsp_determine_start_code`.
   - `smolrtsp_dgram_socket`.

## 0.1.0 - 2022-03-30

### Added

 - This awesome library.
