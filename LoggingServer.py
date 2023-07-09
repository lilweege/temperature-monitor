#!/usr/bin/env python

from http.server import BaseHTTPRequestHandler, HTTPServer
from datetime import datetime
from zoneinfo import ZoneInfo
from base64 import b64encode
import argparse
import os


class Handler(BaseHTTPRequestHandler):
    def do_HEAD(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/plain')
        self.end_headers()

    def do_AUTHHEAD(self):
        self.send_response(401)
        self.send_header('WWW-Authenticate', 'Basic realm="Private Server"')
        self.send_header('Content-type', 'text/plain')
        self.end_headers()

    def do_POST(self):
        global auth_user, auth_pass
        expected_authorization = f"Basic {b64encode(f'{auth_user}:{auth_pass}'.encode()).decode()}"
        if self.headers['Authorization'] != expected_authorization:
            self.do_AUTHHEAD()
            self.wfile.write(b'Bad Auth!\n')
            return

        file_length = self.headers["Content-Length"]
        if not file_length:
            self.send_response(400)
            self.end_headers()
            return

        try:
            content = self.rfile.read(int(file_length)).decode()
            temperature = float(content)
            now = datetime.now(tz=ZoneInfo("America/Toronto"))
            curr_date = now.strftime("%Y-%m-%d")
            curr_time = now.strftime("%H:%M:%S")
            log_fn = f"{curr_date}"
            with open(os.path.join(log_dir, log_fn), "a") as log_file:
                log_file.write(f"{curr_time} {temperature:.2f}\n")
        except:
            self.send_response(500)
            self.end_headers()
        else:
            self.send_response(200)
            self.end_headers()


def main():
    global auth_user, auth_pass, log_dir
    parser = argparse.ArgumentParser()
    parser.add_argument("auth_user", help="Username for POST authentication")
    parser.add_argument("auth_pass", help="Password for POST authentication")
    parser.add_argument("--log_dir", help="Directory to save log files to", default=".")
    args = parser.parse_args()
    auth_user = args.auth_user
    auth_pass = args.auth_pass
    log_dir = args.log_dir

    with HTTPServer(('', 8000), Handler) as server:
        server.serve_forever()

    return 0


if __name__ == '__main__':
    exit(main())

