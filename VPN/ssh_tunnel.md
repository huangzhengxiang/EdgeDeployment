# SSH Tunneling
This technique allows Device A to SSH into Device B, even though neither has a public WAN IP address, with the help of a VPS server with WAN IP.

```
topology:
A -> VPS -> B
```

## VPS Public Server End
### 1. Configure Firewall of the Service Provider (if you own the physical machine of your server, skip this)
For example, add new rules to allow a non-designated port number `<VPS-proxy-port>` in "安全组" if you are using Aliyun.

### 2. Set up Firewall on Server
Check if ufw is on，
```bash
sudo ufw status
```

Allow to use a new 
```bash
sudo ufw allow <VPS-proxy-port>/tcp
sudo ufw reload
```

Make Sure the Port is on
```bash
ss -tlnp | grep <VPS-proxy-port>
```

If you see `127.0.0.1:<VPS-proxy-port>` → refuse connection.

If `0.0.0.0:<VPS-proxy-port>` → accept

### 3. Allow SSH Forwarding on Server
First, ensure the following options are set in /etc/ssh/sshd_config:
```
GatewayPorts yes
AllowTcpForwarding yes
```

Restart SSH to apply changes:
```bash
sudo systemctl restart ssh
```

Now ssh forwading is enabled, which means reverse tunnel is available on open port now.

## Device End

### 1. Set up Reverse Tunnel on Device B
Run the reverse tunnel from Device B:
```bash
ssh -N -R 0.0.0.0:<VPS-proxy-port>:localhost:22 <VPS-username>@<VPS-IP>
```

### 2. Connect and Login from Device A
```bash
ssh -p <VPS-proxy-port> <B-username>@<VPS-IP>
```