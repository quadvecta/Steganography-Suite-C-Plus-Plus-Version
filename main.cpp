#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>
#include <cryptopp/osrng.h>

// Function to encrypt a message using AES
std::string encryptMessage(const std::string &message, std::string &key) {
    CryptoPP::AutoSeededRandomPool prng;
    CryptoPP::SecByteBlock keyBytes(32); // 256-bit key
    prng.GenerateBlock(keyBytes, keyBytes.size());

    key = std::string(keyBytes.begin(), keyBytes.end());

    CryptoPP::AES::Encryption aesEncryption(keyBytes, keyBytes.size());
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, keyBytes);

    std::string encrypted;
    CryptoPP::StringSource(message, true,
        new CryptoPP::StreamTransformationFilter(cbcEncryption,
            new CryptoPP::StringSink(encrypted)));

    return encrypted;
}

// Function to decrypt a message using AES
std::string decryptMessage(const std::string &encrypted, const std::string &key) {
    CryptoPP::SecByteBlock keyBytes(reinterpret_cast<const unsigned char*>(key.data()), key.size());

    CryptoPP::AES::Decryption aesDecryption(keyBytes, keyBytes.size());
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, keyBytes);

    std::string decrypted;
    CryptoPP::StringSource(encrypted, true,
        new CryptoPP::StreamTransformationFilter(cbcDecryption,
            new CryptoPP::StringSink(decrypted)));

    return decrypted;
}

// Function to embed a message into an image
void embedMessage(cv::Mat &image, const std::string &message) {
    int n = 0, m = 0, z = 0;
    for (char c : message) {
        image.at<cv::Vec3b>(n, m)[z] = c;
        m++;
        if (m >= image.cols) {
            m = 0;
            n++;
        }
        z = (z + 1) % 3;
    }
}

// Function to extract a message from an image
std::string extractMessage(const cv::Mat &image) {
    std::string message;
    int n = 0, m = 0, z = 0;
    while (true) {
        char c = image.at<cv::Vec3b>(n, m)[z];
        if (c == 0) break;  // End of message
        message += c;
        m++;
        if (m >= image.cols) {
            m = 0;
            n++;
        }
        z = (z + 1) % 3;
    }
    return message;
}

int main() {
    std::cout << "Steganography Suite v1.0 (CLI)" << std::endl;
    std::cout << "-----------------------------" << std::endl;

    int choice;
    std::cout << "1. Encrypt and Embed Message\n2. Decrypt and Extract Message\nEnter your choice: ";
    std::cin >> choice;
    std::cin.ignore();  

    if (choice == 1) {
        // Encrypt and Embed
        std::string imagePath, message;
        std::cout << "Enter the path to the input image: ";
        std::getline(std::cin, imagePath);
        std::cout << "Enter the secret message: ";
        std::getline(std::cin, message);

        // Load image
        cv::Mat image = cv::imread(imagePath);
        if (image.empty()) {
            std::cerr << "Error: Could not load image." << std::endl;
            return 1;
        }

        // Encrypt message
        std::string key;
        std::string encrypted = encryptMessage(message, key);
        std::cout << "Encryption Key: " << key << std::endl;

        // Embed message into image
        embedMessage(image, encrypted);
        cv::imwrite("output_image.png", image);
        std::cout << "Message embedded and saved as output_image.png" << std::endl;

    } else if (choice == 2) {
        // Decrypt and Extract
        std::string imagePath, key;
        std::cout << "Enter the path to the encrypted image: ";
        std::getline(std::cin, imagePath);
        std::cout << "Enter the decryption key: ";
        std::getline(std::cin, key);

        // Load image
        cv::Mat loadedImage = cv::imread(imagePath);
        if (loadedImage.empty()) {
            std::cerr << "Error: Could not load image." << std::endl;
            return 1;
        }

        // Extract message from image
        std::string extracted = extractMessage(loadedImage);

        // Decrypt message
        std::string decrypted = decryptMessage(extracted, key);
        std::cout << "Decrypted Message: " << decrypted << std::endl;

    } else {
        std::cerr << "Invalid choice. Exiting..." << std::endl;
        return 1;
    }

    return 0;
}
