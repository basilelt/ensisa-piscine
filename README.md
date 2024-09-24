# 🎮 Immersion Project "PISCINE" 

**IR 1 - BUT RT 3**

![Language](https://img.shields.io/badge/language-C-blue.svg)
![GTK3](https://img.shields.io/badge/GTK3-3.0-green.svg)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20MacOS-lightgrey.svg)
![GCC](https://img.shields.io/badge/GCC-9.3.0-brightgreen.svg)
![LLDB](https://img.shields.io/badge/LLDB-10.0.0-orange.svg)

<p align="center">
  <img src="docs/img/banner.png" alt="Game banner" style="width: 100%; max-width: 100%; height: auto;" />
</p>

## 📖 Table of Contents

- [🎮 Immersion Project "PISCINE"]
  - [📖 Table of Contents]
  - [🕹️ The Game]
  - [📋 Specifications]
  - [📦 Installation]
  - [🛠️ Compilation]
  - [📦 Documentation]
  - [🚀 Run]
    - [🏠 Run Locally]
    - [🌐 Run with Network]
      - [📥 Client]
      - [🗄️ Server]
  - [👥 Authors]


## 🕹️ The Game 

**Square Game (Chomp Game)**

- 🟦 **Rectangular board of dimensions 7x9**
- 🤝 **Two opposing players**
- 🔄 **Turn-based gameplay**
  - On their turn, a player chooses a square on the board:
    - 🧱 The chosen square is destroyed,
    - 💥 All squares below and to the right of the chosen square are also destroyed.
  - 🔢 A player can destroy a maximum of **5 squares per turn**.
- 🎯 **Goal**: The player who destroys the **last square loses**.

<p align="center">
  <img src="docs/img/gui1.png" alt="Game screen 1" width="45%" />
  <img src="docs/img/gui2.png" alt="Game screen 2" width="45%" />
</p>

## 📋 Specifications 

- 💻 Developed in **C language** (compilable with gcc)
- 🖥️ Executable on **Linux** or **MacOS** machines
- 🛠️ Option to install a **virtual machine** if needed
- ⚙️ Code runs on a **standard architecture** (e.g., 8 cores, 2.5GHz, 16GB RAM)

## 📦 Installation 

To install the game, use the **** script.

```bash
./install.sh
```

The script will install all necessary dependencies.

## 🛠️ Compilation

To compile the game, use the **** script.

```bash
./compile.sh
```

This will compile the game and create the executable file named **game**.
It will also make the documentation available in the **docs** folder.

## 📦 Documentation 

- The documentation is generated using **Doxygen** with `make docs`, it is already done in the **compile.sh** script.

- To access the documentation, open the **docs** folder and open the **index.html** file in your browser.
  You can also use the **Five server** extension in your IDE or nginx/apache. 
```bash
cd project/docs/html
open index.html
```

## 🚀 Run 

You have multiple options when it comes to running the game:

### 🏠 Run Locally 

- To run the game locally, use the **`-l`** argument.
- Additional options:
  - 🖥️  **CLI Mode**: Add the **`-t`** argument.
  - 🤖 **AI Mode**: Add the **`-ia`** argument.
  - 🖥️ 🤖 **CLI + AI Mode**: Add both **`-t`** and **`-ia`** arguments.

```bash
# Run locally
./game -l

# Run locally with GUI
./game -l -g

# Run locally with AI in terminal mode
./game -l -t -ia

# Run locally with GUI and AI
./game -l -ia
```

### 🌐 Run with Network 

#### 📥 Client 

- To run the client, use the **`-c`** argument along with the server's IP address and port.
- Additional options:
  - 🖥️  **CLI Mode**: Add the **`-t`** argument.
  - 🤖 **AI Mode**: Add the **`-ia`** argument.
  - 🖥️ 🤖 **CLI + AI Mode**: Add both **`-t`** and **`-ia`** arguments.

```bash
# Run client in the terminal
./game -c <server_ip>:<port> -t

# Run client with GUI
./game -c <server_ip>:<port> 

# Run client with AI in the terminal
./game -c <server_ip>:<port> -t -ia

# Run client with GUI and AI
./game -c <server_ip>:<port> -ia
```

#### 🗄️ Server 

- To run the server, use the **`-s`** argument with the desired port.
  - *Uses the IP address of the host machine.*
- Additional options:
  - 🖥️  **CLI Mode**: Add the **`-t`** argument.
  - 🤖 **AI Mode**: Add the **`-ia`** argument.
  - 🖥️ 🤖 **CLI + AI Mode**: Add both **`-t`** and **`-ia`** arguments.

```bash
# Run server in the terminal
./game -s <port> -t

# Run server with GUI
./game -s <port> 

# Run server with AI in the terminal
./game -s <port> -ia -t

# Run server with GUI and AI
./game -s <port> -ia
```

> **Note:** Combine multiple arguments to tailor your game experience.

## 👥 Authors 
This project is created and maintained by:

<table>
  <tr>
    <td><a href="mailto:samuel.rolli@uha.fr"><img src="docs/img/user3.png" alt="Samuel ROLLI" width="100" /></a></td>
    <td><a href="mailto:elise.beauvy@uha.fr"><img src="docs/img/user7.png" alt="Elise BEAUVY" width="100" /></a></td>
    <td><a href="mailto:martin.baratte@uha.fr"><img src="docs/img/user5.png" alt="Martin BARATTE" width="100" /></a></td>
    <td><a href="mailto:basile.le-thiec@uha.fr"><img src="docs/img/user.png" alt="Basile LE THIEC" width="100" /></a></td>
    <td><a href="mailto:louay.ben-ltoufa@uha.fr"><img src="docs/img/user4.png" alt="Louay BEN LTOUFA" width="100" /></a></td>
    <td><a href="mailto:sakithyan.ragavan@uha.fr"><img src="docs/img/user2.png" alt="Sakithyan RAGAVAN" width="100" /></a></td>
    <td><a href="mailto:mehmet.bozkurt@uha.fr"><img src="docs/img/user6.png" alt="Mehmet BOZKURT" width="100" /></a></td>
  </tr>
  <tr>
    <td><a href="mailto:samuel.rolli@uha.fr">Samuel ROLLI</a></td>
    <td><a href="mailto:elise.beauvy@uha.fr">Elise BEAUVY</a></td>
    <td><a href="mailto:martin.baratte@uha.fr">Martin BARATTE</a></td>
    <td><a href="mailto:basile.le-thiec@uha.fr">Basile LE THIEC</a></td>
    <td><a href="mailto:louay.ben-ltoufa@uha.fr">Louay BEN LTOUFA</a></td>
    <td><a href="mailto:sakithyan.ragavan@uha.fr">Sakithyan RAGAVAN</a></td>
    <td><a href="mailto:mehmet.bozkurt@uha.fr">Mehmet BOZKURT</a></td>
  </tr>
</table>

Feel free to reach out to any of us for questions!
