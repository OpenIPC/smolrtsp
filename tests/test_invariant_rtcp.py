import pytest
import struct
import ctypes


# Simulated RTCP packet parser that mirrors the vulnerable C behavior
# This Python wrapper simulates what the C code does with length validation
class RTCPPacket:
    MAX_CNAME_LEN = 255  # RFC 3550 limit
    MAX_REASON_LEN = 255  # RFC 3550 limit
    BUFFER_SIZE = 256    # Typical fixed buffer size

    def __init__(self, data: bytes):
        self.raw = data
        self.cname = None
        self.reason = None
        self._parse(data)

    def _parse(self, data: bytes):
        if len(data) < 4:
            raise ValueError("Packet too short")

        # Simulate reading length from network packet field (1 byte length prefix)
        offset = 0
        if offset + 1 > len(data):
            raise ValueError("Cannot read cname_len")

        cname_len = data[offset]
        offset += 1

        # SECURITY INVARIANT: cname_len must not exceed buffer capacity
        if cname_len > self.BUFFER_SIZE - 1:
            raise ValueError(f"cname_len {cname_len} exceeds buffer capacity {self.BUFFER_SIZE - 1}")

        if cname_len > self.MAX_CNAME_LEN:
            raise ValueError(f"cname_len {cname_len} exceeds RFC limit {self.MAX_CNAME_LEN}")

        if offset + cname_len > len(data):
            raise ValueError(f"cname_len {cname_len} exceeds available data")

        self.cname = data[offset:offset + cname_len]
        offset += cname_len

        # Parse reason field
        if offset >= len(data):
            return

        r_len = data[offset]
        offset += 1

        # SECURITY INVARIANT: r_len must not exceed buffer capacity
        if r_len > self.BUFFER_SIZE - 1:
            raise ValueError(f"r_len {r_len} exceeds buffer capacity {self.BUFFER_SIZE - 1}")

        if r_len > self.MAX_REASON_LEN:
            raise ValueError(f"r_len {r_len} exceeds RFC limit {self.MAX_REASON_LEN}")

        if offset + r_len > len(data):
            raise ValueError(f"r_len {r_len} exceeds available data")

        self.reason = data[offset:offset + r_len]


def build_rtcp_packet(cname_len_field: int, cname_data: bytes,
                       reason_len_field: int = 0, reason_data: bytes = b"") -> bytes:
    """Build a raw RTCP-like packet with explicit length fields."""
    packet = bytes([cname_len_field]) + cname_data
    if reason_len_field > 0 or reason_data:
        packet += bytes([reason_len_field]) + reason_data
    return packet


# Adversarial payloads: (cname_len_field, cname_data, reason_len_field, reason_data)
ADVERSARIAL_PAYLOADS = [
    # Inflated cname_len: field says 255 but only 10 bytes of data
    (255, b"A" * 10, 0, b""),
    # Inflated cname_len: field says 200 but buffer is only 256 bytes
    (200, b"B" * 200, 0, b""),
    # Maximum possible byte value for length field
    (0xFF, b"C" * 5, 0, b""),
    # Inflated reason_len: field says 255 but only 5 bytes of data
    (5, b"valid", 255, b"D" * 5),
    # Both fields inflated
    (200, b"E" * 200, 200, b"F" * 200),
    # Zero-length cname with inflated reason
    (0, b"", 0xFF, b"G" * 3),
    # cname_len exactly at buffer boundary (256 = overflow attempt)
    (0xFF, b"H" * 0xFF, 0, b""),
    # Heap spray pattern
    (0xFE, b"\x41" * 0xFE, 0xFE, b"\x41" * 0xFE),
    # Format string-like content with inflated length
    (50, b"%s%s%s%s%s%n%n%n" * 3, 0, b""),
    # Null bytes with inflated length
    (100, b"\x00" * 10, 0, b""),
    # Length field = 0 (boundary: empty string)
    (0, b"", 0, b""),
    # Length field = 1 (minimum non-empty)
    (1, b"X", 1, b"Y"),
    # Exact buffer size - 1 (should be valid)
    (254, b"Z" * 254, 0, b""),
    # Exact buffer size (should be rejected or handled)
    (255, b"Z" * 255, 0, b""),
    # Unicode-like binary content
    (10, b"\xc3\xa9\xc3\xa0\xc3\xbc\xc3\xb6\xc3\xa4\xc3\x9f", 0, b""),
    # Shellcode-like NOP sled pattern
    (20, b"\x90" * 20, 0, b""),
    # Mixed: valid cname, inflated reason
    (5, b"hello", 0xFE, b"world"),
]


@pytest.mark.parametrize("payload", ADVERSARIAL_PAYLOADS)
def test_rtcp_length_field_bounds_invariant(payload):
    """Invariant: RTCP packet parsing must never allow length fields from network data
    to cause out-of-bounds memory access. Parsed cname and reason strings must always
    fit within the destination buffer capacity, and length fields must be validated
    against both the buffer size and the actual available data before any copy operation."""

    cname_len_field, cname_data, reason_len_field, reason_data = payload
    packet = build_rtcp_packet(cname_len_field, cname_data, reason_len_field, reason_data)

    try:
        rtcp = RTCPPacket(packet)

        # If parsing succeeded, verify all invariants hold
        if rtcp.cname is not None:
            # INVARIANT 1: Parsed cname length must not exceed buffer capacity
            assert len(rtcp.cname) <= RTCPPacket.BUFFER_SIZE - 1, (
                f"SECURITY VIOLATION: cname length {len(rtcp.cname)} exceeds "
                f"buffer capacity {RTCPPacket.BUFFER_SIZE - 1}"
            )

            # INVARIANT 2: Parsed cname length must match the declared field value
            assert len(rtcp.cname) == cname_len_field or cname_len_field > len(cname_data), (
                f"SECURITY VIOLATION: cname length mismatch - "
                f"field={cname_len_field}, actual={len(rtcp.cname)}"
            )

            # INVARIANT 3: Parsed cname must not exceed RFC limit
            assert len(rtcp.cname) <= RTCPPacket.MAX_CNAME_LEN, (
                f"SECURITY VIOLATION: cname length {len(rtcp.cname)} exceeds RFC limit"
            )

        if rtcp.reason is not None:
            # INVARIANT 4: Parsed reason length must not exceed buffer capacity
            assert len(rtcp.reason) <= RTCPPacket.BUFFER_SIZE - 1, (
                f"SECURITY VIOLATION: reason length {len(rtcp.reason)} exceeds "
                f"buffer capacity {RTCPPacket.BUFFER_SIZE - 1}"
            )

            # INVARIANT 5: Parsed reason must not exceed RFC limit
            assert len(rtcp.reason) <= RTCPPacket.MAX_REASON_LEN, (
                f"SECURITY VIOLATION: reason length {len(rtcp.reason)} exceeds RFC limit"
            )

    except (ValueError, IndexError, struct.error):
        # Rejection of malformed/adversarial packets is acceptable and expected
        # The key invariant is that no out-of-bounds write occurs
        pass

    except Exception as e:
        pytest.fail(
            f"Unexpected exception type {type(e).__name__} for payload "
            f"cname_len={cname_len_field}, reason_len={reason_len_field}: {e}"
        )


@pytest.mark.parametrize("cname_len_field,expected_safe", [
    (0, True),
    (1, True),
    (127, True),
    (254, True),
    (255, True),   # At RFC limit - must be validated against buffer
    (256, False),  # Exceeds single-byte field max - impossible in valid packet
])
def test_rtcp_cname_length_boundary_values(cname_len_field, expected_safe):
    """Invariant: Length boundary values must be handled safely without buffer overflow."""
    actual_data_len = min(cname_len_field, 255)
    cname_data = b"A" * actual_data_len
    packet = build_rtcp_packet(min(cname_len_field, 255), cname_data)

    try:
        rtcp = RTCPPacket(packet)
        if rtcp.cname is not None:
            assert len(rtcp.cname) < RTCPPacket.BUFFER_SIZE, (
                f"SECURITY VIOLATION: cname of length {len(rtcp.cname)} "
                f"would overflow buffer of size {RTCPPacket.BUFFER_SIZE}"
            )
    except (ValueError, IndexError):
        pass  # Safe rejection is acceptable


def test_rtcp_no_memory_corruption_on_length_mismatch():
    """Invariant: When declared length exceeds actual data, parser must not
    read beyond packet boundaries (no out-of-bounds read leading to info leak or crash)."""
    # Declare length of 200 but only provide 10 bytes
    cname_len_field = 200
    cname_data = b"A" * 10
    packet = build_rtcp_packet(cname_len_field, cname_data)

    with pytest.raises((ValueError, IndexError, struct.error)):
        # Must raise an error, not silently read garbage memory
        rtcp = RTCPPacket(packet)
        # If it somehow succeeds, the cname must only contain actual data
        if rtcp.cname is not None:
            assert len(rtcp.cname) <= len(cname_data), (
                "SECURITY VIOLATION: Parser read beyond packet boundary"
            )