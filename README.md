# MarketSharkHeadless

MarketSharkHeadless is a headless C++ Minecraft client designed to connect to Hypixel Skyblock's Auction House and automatically buy items specified by the CoflNet websocket. The client is built to connect to a backend server, enabling reporting and control via a Discord bot. It includes a CLI for configuration and a Linux service to maintain continuous operation.

## Credits

MarketSharkHeadless was built using [mclib](https://github.com/plushmonkey/mclib), a C++ library by plushmonkey for connecting to Minecraft servers. 

Original `mclib` features:
- Supports Minecraft protocol for versions 1.10 through 1.12.2
- Premium and offline modes
- Yggdrasil API support
- Authentication with passwords or tokens
- Encryption and compression
- Connection to Forge servers
- World and entity data handling
- Basic inventory and NBT data operations
- Block entity and collision data
- Basic player control

We extend our thanks to plushmonkey for the foundation provided by `mclib`.

## Features

- 1.8 Protocol Support
- Automated item purchasing via CoflNet websocket on Hypixel Skyblock
- Integration with a backend server for reporting and control
- Discord bot integration for remote monitoring and commands
- CLI tool for easy configuration
- Linux service for sustained operation

## Configuring MarketSharkHeadless

1. **Configure Env File**
    - Copy the example.env file using `cp example.env .env`
    - Set your DOMAIN (or IP Address) for your [MarketSharkServer](https://github.com/JackTYM/MarketSharkServer) host   

2. **Switching to Unsecure (Not Reccomended)**
    - Only required if using an IP or Domain that has no SSL Certifciates Served
    - Replace all "https" strings related to DOMAIN with "http"
    - Replace all "wss" strings related to DOMAIN with "ws"

## Building MarketSharkHeadless

### Installing Required Dependencies

```bash
sudo apt update
sudo apt install cmake build-essential libcurl4-openssl-dev libssl-dev zlib1g-dev libncurses5-dev libncursesw5-dev uuid-dev git
```

### Building IXWebSocket

```bash
git clone https://github.com/machinezone/IXWebSocket
cd IXWebSocket/ && mkdir build && cd build
cmake .. && make
make install
```

### Building MarketSharkHeadless

```bash
git clone https://github.com/JackTYM/MarketSharkHeadless
cd MarketSharkHeadless/ && mkdir build && cd build
cmake .. && cmake --build .
```

### Built Files

- `cli` - Program to launch and configure MarketSharkHeadless
- `service` - Linux service for handling CLI queries and maintaining communication with the server
- `client` - The headless client for Hypixel

## Missing Features

- AutoList functionality
- Remote Start/Stop control

## Known Vulnerabilities

- None reported

## Known Bugs

- Occasional crashes under certain conditions

## Contributing

Contributions are welcome! To contribute:

1. **Fork** the repository.
2. **Clone** your fork: `git clone https://github.com/JackTYM/MarketSharkHeadless.git`
3. **Create a new branch** for your feature or bug fix: `git checkout -b feature-name`
4. **Commit** your changes: `git commit -m "Add new feature"`
5. **Push** to your fork: `git push origin feature-name`
6. **Submit a pull request** with a description of your changes.

Pull requests will be reviewed and accepted if applicable to the project goals.

---

Thank you for your interest in MarketSharkHeadless!
