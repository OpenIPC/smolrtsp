# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## unreleased

### Fixed

 - Update the minimum required CMake version to 3.10.0 due to deprecation (see [metalang99/issues/33](https://github.com/hirrolot/metalang99/issues/33)).

### Fixed

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
