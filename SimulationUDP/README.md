# Routing Simulation 

**Network** 149.54.0.0/16


- **lan 1** 1000IP
  - Hosts available: from 149.54.0.1/22 to 149.54.3.255/22
  - file `nano /etc/network/interfaces`
  ```md
  # The primary network
    allow-hotplug enp0s3
    iface enp0s3 inet static
    address 149.54.0.2/22
    gateway 149.54.0.1
  ```
- **lan 2** 1000IP
  - Hosts available: from 149.54.4.1/22 to 149.54.7.255/22
  - file `nano /etc/network/interfaces`
  ```md
  # The primary network
    allow-hotplug enp0s3
    iface enp0s3 inet static
    address 149.54.4.2/22
    gateway 149.54.4.1
  ```
- **lan 3** 512IP
  - Hosts available:
    - from 149.54.8.1/22 to 149.54.10.255/22 
    - or from 149.54.8.1/23 to 149.54.9.255/23 it does not matter
  - file: `nano /etc/network/interfaces`
  ```md
  # The primary network
    allow-hotplug enp0s3
    iface enp0s3 inet static
    address 149.54.8.2/23
    gateway 149.54.8.1
  ``` 

- **Router**
    - lan 01: 149.54.0.1/22
    - lan 02: 149.54.4.1/22
    - lan 03: 149.54.8.1/23
    - file `nano /etc/sysctl.conf`
      ```md
      # Uncommet the next line to enable placket forwarding for IPV4
      net.ipv4.ip_forward=1
      ```
    - file `nano /etc/network/intefaces`
      ```md
      # The primary network interfaces
      # LAN 01
      allow-hotplug enp0s3 
      iface enp0s3 inet static
      address 149.54.0.1/22
      
      # LAN 02
      allow-hotplug enp0s8
      iface enp0s8 inet static
      address 149.54.4.1/22
      
      # LAN 03
      allow-hotplug enp0s9
      iface enp0s9 inet static
      address 149.54.8.1/23  

      ```
## Commands `scp` to transfer files to the VMs
```md
    scp "path\client.c" user@192.168.56.106:/home/user/client.c
    
    scp "path\server.c" user@192.168.56.108:/home/user/server.c
    
```

[Alessandro Ferrante](https://alessandroferrante.net)  