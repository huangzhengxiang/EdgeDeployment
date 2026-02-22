## frp tunnel

How to map a webpage service on localhost of a LAN machine to a service on a website address on WAN?

Topology
```
WAN server:       (http port:5003) ←→  frps  ←→ (frp port:7000)
                                                       ↑
LAN localhost:                                        frpc ←→ (http port:5003) ←→ 你的localhost网页
```
You shall make sure that the WAN server have its website http port (e.g., 5003) and frp port (e.g., 7000)  available (firewall and port enabled).

### frp server end
1. Installation:
[https://github.com/fatedier/frp/releases](https://github.com/fatedier/frp/releases)

2. unzip and modify frps.toml
```bash
tar -zxvf frp_0.56.0_linux_amd64.tar.gz
```
frps.toml set the frp port.
```toml
bindPort = 7000
```

3. set up the frps service
```bash
nohup ./frps -c frps.toml
```
After setup, the server is not subject to code change of the website implementations.

### frp client end
1. Intall frp too.

2. unzip and modify frpc.toml
Server IP address and frp port shall be configured. Besides, `localIP = "0.0.0.0"` shows the local IP.
`localPort` and `remotePort` are the local and remote website service ports. (They are not necessarily to be the same.)

```toml
serverAddr = "47.100.35.64"
serverPort = 7000

[[proxies]]
name = "guosen"
type = "tcp"
localIP = "0.0.0.0"
localPort = 5003
remotePort = 5003
```

**Important**: use "0.0.0.0" rather than "127.0.0.1" is more robust as it listen to both IPv4 and v6 callbacks.
```python
import socketserver

class ThreadingTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    allow_reuse_address = True

PORT = 5003

with ThreadingTCPServer(("0.0.0.0", PORT), CustomHTTPRequestHandler) as httpd:
    print(f"Server running at http://localhost:{PORT}/")
    httpd.serve_forever()
```
For example, use `ThreadingTCPServer` and "0.0.0.0" to support multi-threading and IPV4/v6 visiting.

3. set up the frpc service
```ps1
frpc.exe -c frpc.toml
```
