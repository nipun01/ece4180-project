# Python 3 server example
from http.server import BaseHTTPRequestHandler, HTTPServer
import time
from PIL import Image

#hostName = "localhost"
hostName = "192.168.137.1"
serverPort = 8080
left = True
first = True
arr_left = None
arr_right = None

class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()
        self.wfile.write(bytes("<html><head><title>https://pythonbasics.org</title></head>", "utf-8"))
        self.wfile.write(bytes("<p>Request: %s</p>" % self.path, "utf-8"))
        self.wfile.write(bytes("<body>", "utf-8"))
        self.wfile.write(bytes("<p>This is an example web server.</p>", "utf-8"))
        self.wfile.write(bytes("</body></html>", "utf-8"))

    def do_POST(self):
        global left
        global first
        global arr_left
        global arr_right
        content_len = int(self.headers["content-length"])
        post_body = self.rfile.read(content_len)
        split_data = str(post_body)[16:-5].split(", ")
        split_data = split_data[:-1]
        for x in range(len(split_data)):
            split_data[x] = int(split_data[x].strip().strip('"'))

        print(len(split_data))

        if left:
            if first:
                arr_left = split_data
                first = False
            else:
                for val in split_data:
                    arr_left.append(val)
                first = True
                left = False
            
        else:
            if first:
                arr_right = split_data
                first = False
            else:
                for val in split_data:
                    arr_right.append(val)
                first = True
                left = True
                img = Image.new("1", (240, 120), "black")
                pixels = img.load()

                for y in range(120):
                    for x in range(120):
                        pixels[x, y] = arr_left[min(x + y * 120, len(arr_left) - 1)]
                        pixels[x + 120, y] = arr_right[min(x + y * 120, len(arr_right) - 1)]
                
                img.save("map.bmp")
                img.show()    
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()
        self.wfile.write(bytes("<html><head><title>https://pythonbasics.org</title></head>", "utf-8"))
        self.wfile.write(bytes("<p>Request: %s</p>" % self.path, "utf-8"))
        self.wfile.write(bytes("<body>", "utf-8"))
        self.wfile.write(bytes("<p>This is an example web server.</p>", "utf-8"))
        self.wfile.write(bytes("</body></html>", "utf-8"))
        

if __name__ == "__main__":        
    webServer = HTTPServer((hostName, serverPort), MyServer)
    print("Server started http://%s:%s" % (hostName, serverPort))

    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")