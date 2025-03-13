
# Steganography Suite v1.0 (CLI) (C++ Version)

A command-line tool for encrypting and embedding secret messages in images using steganography.

## Features
- **Encryption**: Uses AES-256 for secure message encryption.
- **Steganography**: Embeds encrypted messages into images using LSB (Least Significant Bit) steganography.
- **Decryption**: Extracts and decrypts hidden messages from images.

## How to Use
1. Clone the repository:
   ```bash
   git clone https://github.com/quadvecta/Steganography-Suite-C-Plus-Plus-Version
   
   cd Steganography-Suite-C-Plus-Plus-Version
   ```
2. Compile the program:

```bash
g++ -o stego_cli main.cpp -lopencv_core -lopencv_imgcodecs -lcryptopp
```

3. Run the program:

```bash
./stego_cli
```

4. Follow the on-screen instructions to encrypt/decrypt messages.

# Requirements
- OpenCV (for image processing).

- Crypto++ (for encryption).

# License
This project is licensed under the MIT License. See the LICENSE file for details.

---
