#!/usr/bin/env python3
"""Drives the example server through out-of-order RTSP sessions.

Usage: test-server-sessions.py <path to the server binary>

Every case gets a fresh server, which must answer with the expected status
codes, close the sockets it opened, still serve a new client, and exit cleanly
on SIGINT (the example is built with AddressSanitizer, so a memory error or a
leak makes its exit code non-zero).
"""

import os
import re
import signal
import socket
import subprocess
import sys
import time

PORT = 554
TCP = "Transport: RTP/AVP/TCP;unicast;interleaved=0-1\r\n"
UDP = "Transport: RTP/AVP;unicast;client_port=40000-40001\r\n"


def setup(path="/audio", session=12345, transport=TCP):
    return ("SETUP", path, session, transport)


def play(session=12345):
    return ("PLAY", "/", session, "")


def teardown(session=12345):
    return ("TEARDOWN", "/", session, "")


# (name, requests, expected status codes)
CASES = [
    ("regular session", [setup(), setup("/video"), play(), teardown()], [200] * 4),
    ("disconnect while playing", [setup(), play()], [200] * 2),
    ("TEARDOWN before PLAY", [setup(), teardown()], [200] * 2),
    ("TEARDOWN twice", [setup(), play(), teardown(), teardown()], [200] * 3 + [454]),
    ("TEARDOWN of session 0 without SETUP", [teardown(0)], [454]),
    ("PLAY of session 0 without SETUP", [play(0)], [454]),
    ("PLAY of session 0 next to a SETUP stream", [setup(), play(0)], [200, 454]),
    ("PLAY twice", [setup(), play(), play()], [200] * 3),
    ("PLAY after TEARDOWN", [setup(), play(), teardown(), play()], [200] * 3 + [454]),
    (
        "session restarted after TEARDOWN",
        [setup(), play(), teardown(), setup(), play()],
        [200] * 5,
    ),
    ("SETUP and PLAY twice", [setup(), play(), setup(), play()], [200] * 4),
    ("SETUP while playing", [setup(), play(), setup()], [200] * 3),
    ("SETUP repeated over UDP", [setup(transport=UDP)] * 20, [200] * 20),
    (
        "SETUP over UDP with a malformed session",
        [setup(session="x", transport=UDP)] * 20,
        [400] * 20,
    ),
]


def connect():
    conn = socket.create_connection(("127.0.0.1", PORT), timeout=3)
    return conn


def request(conn, cseq, method, path, session, transport):
    conn.sendall(
        (
            f"{method} rtsp://127.0.0.1{path} RTSP/1.0\r\n"
            f"CSeq: {cseq}\r\n{transport}Session: {session}\r\n\r\n"
        ).encode()
    )
    # The response may be surrounded by interleaved RTP.
    pattern = re.compile(
        rb"RTSP/1\.0 (\d+) [^\r\n]*\r\n(?:[^\r\n]+\r\n)*?CSeq: %d\r\n" % cseq
    )
    data = b""
    deadline = time.monotonic() + 3
    while time.monotonic() < deadline:
        chunk = conn.recv(65536)
        if not chunk:
            return None
        data += chunk
        found = pattern.search(data)
        if found:
            return int(found[1])
    return None


def converse(requests):
    codes = []
    conn = connect()
    try:
        for cseq, req in enumerate(requests, 1):
            codes.append(request(conn, cseq, *req))
            if codes[-1] is None:
                break
        time.sleep(0.2)  # Let the media timers fire.
    except OSError as e:
        codes.append(repr(e))
    finally:
        conn.close()
    time.sleep(0.3)  # Let them fire after the disconnect, too.
    return codes


def run_case(server, requests, expected):
    proc = subprocess.Popen(
        [server], stdout=subprocess.DEVNULL, stderr=subprocess.PIPE, text=True
    )
    problems = []
    try:
        for attempt in range(50):
            try:
                connect().close()
                break
            except OSError:
                if proc.poll() is not None or attempt == 49:
                    raise
                time.sleep(0.1)
        time.sleep(0.2)

        def count_fds():
            try:
                return len(os.listdir(f"/proc/{proc.pid}/fd"))
            except FileNotFoundError:
                return 0  # No procfs; skip the check.

        fds = count_fds()
        codes = converse(requests)
        if codes != expected:
            problems.append(f"expected {expected}, got {codes}")
        if proc.poll() is None:
            leaked = count_fds() - fds
            if leaked > 0:
                problems.append(f"{leaked} file descriptors leaked")
            if converse([setup(), play(), teardown()]) != [200] * 3:
                problems.append("the next client was not served")
    except OSError as e:
        problems.append(repr(e))
    finally:
        if proc.poll() is None:
            proc.send_signal(signal.SIGINT)
        try:
            _, err = proc.communicate(timeout=20)
        except subprocess.TimeoutExpired:
            proc.kill()
            _, err = proc.communicate()
            problems.append("the server did not exit on SIGINT")
    if proc.returncode != 0:
        problems.append(f"the server exited with {proc.returncode}\n{err}")
    return problems


def main():
    failed = 0
    for name, requests, expected in CASES:
        problems = run_case(sys.argv[1], requests, expected)
        print(f"{'FAIL' if problems else 'ok'}: {name}", flush=True)
        for problem in problems:
            print(f"    {problem}", flush=True)
        failed += bool(problems)
    print(f"{len(CASES) - failed}/{len(CASES)} session cases passed.")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
