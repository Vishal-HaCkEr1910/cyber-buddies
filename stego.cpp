#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <stdexcept>
#include <bitset>
#include <algorithm>

using namespace std;

// ============================================================================
// Base Exception Class for Steganography Errors
// ============================================================================
class SteganographyException : public runtime_error {
public:
    explicit SteganographyException(const string& msg) : runtime_error(msg) {}
};

// ============================================================================
// Utility Class for File Operations
// ============================================================================
class FileUtils {
public:
    /**
     * Reads entire file into a byte vector
     * @param filename Path to the file
     * @return Vector containing file bytes
     */
    static vector<unsigned char> readFile(const string& filename) {
        ifstream file(filename, ios::binary);
        if (!file.is_open()) {
            throw SteganographyException("Cannot open file: " + filename);
        }
        
        file.seekg(0, ios::end);
        size_t fileSize = file.tellg();
        file.seekg(0, ios::beg);
        
        vector<unsigned char> buffer(fileSize);
        file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
        file.close();
        
        return buffer;
    }
    
    /**
     * Writes byte vector to file
     * @param filename Output file path
     * @param data Byte vector to write
     */
    static void writeFile(const string& filename, const vector<unsigned char>& data) {
        ofstream file(filename, ios::binary);
        if (!file.is_open()) {
            throw SteganographyException("Cannot create file: " + filename);
        }
        
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        file.close();
    }
    
    /**
     * Extracts file extension
     * @param filename File path
     * @return Extension in lowercase
     */
    static string getExtension(const string& filename) {
        size_t pos = filename.find_last_of('.');
        if (pos == string::npos) return "";
        
        string ext = filename.substr(pos + 1);
        transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        return ext;
    }
};

// ============================================================================
// Base Steganography Interface
// ============================================================================
class ISteganography {
public:
    virtual ~ISteganography() = default;
    virtual void hideData(const vector<unsigned char>& data, 
                         const string& containerFile, 
                         const string& outputFile) = 0;
    virtual vector<unsigned char> revealData(const string& containerFile) = 0;
};

// ============================================================================
// Image Steganography (LSB Method for BMP images)
// ============================================================================
class ImageSteganography : public ISteganography {
private:
    static const uint32_t SIGNATURE = 0x53544547; // "STEG" in hex
    
    /**
     * Validates BMP file format
     * @param imageData BMP file data
     * @return true if valid BMP
     */
    bool isValidBMP(const vector<unsigned char>& imageData) {
        if (imageData.size() < 54) return false;
        return imageData[0] == 'B' && imageData[1] == 'M';
    }
    
    /**
     * Gets pixel data offset in BMP file
     * @param imageData BMP file data
     * @return Offset to pixel data
     */
    size_t getPixelDataOffset(const vector<unsigned char>& imageData) {
        return imageData[10] | (imageData[11] << 8) | 
               (imageData[12] << 16) | (imageData[13] << 24);
    }
    
    /**
     * Embeds one bit into a byte using LSB
     * @param carrier Byte to modify
     * @param bit Bit to embed (0 or 1)
     * @return Modified byte
     */
    unsigned char embedBit(unsigned char carrier, int bit) {
        return (carrier & 0xFE) | bit;
    }
    
    /**
     * Extracts LSB from a byte
     * @param carrier Byte to extract from
     * @return Extracted bit (0 or 1)
     */
    int extractBit(unsigned char carrier) {
        return carrier & 0x01;
    }

public:
    /**
     * Hides data in BMP image using LSB steganography
     * @param data Data to hide
     * @param containerFile Path to BMP image
     * @param outputFile Output file path
     */
    void hideData(const vector<unsigned char>& data, 
                  const string& containerFile, 
                  const string& outputFile) override {
        vector<unsigned char> image = FileUtils::readFile(containerFile);
        
        if (!isValidBMP(image)) {
            throw SteganographyException("Container must be a valid BMP image");
        }
        
        size_t pixelOffset = getPixelDataOffset(image);
        size_t availableBytes = (image.size() - pixelOffset) / 8;
        size_t requiredBytes = data.size() + sizeof(uint32_t) + sizeof(uint32_t);
        
        if (requiredBytes > availableBytes) {
            throw SteganographyException("Image too small to hide data. Need " + 
                                        to_string(requiredBytes) + " bytes, have " + 
                                        to_string(availableBytes));
        }
        
        size_t bitIndex = 0;
        
        // Embed signature
        for (int i = 31; i >= 0; i--) {
            int bit = (SIGNATURE >> i) & 1;
            image[pixelOffset + bitIndex] = embedBit(image[pixelOffset + bitIndex], bit);
            bitIndex++;
        }
        
        // Embed data size
        uint32_t dataSize = data.size();
        for (int i = 31; i >= 0; i--) {
            int bit = (dataSize >> i) & 1;
            image[pixelOffset + bitIndex] = embedBit(image[pixelOffset + bitIndex], bit);
            bitIndex++;
        }
        
        // Embed actual data
        for (unsigned char byte : data) {
            for (int i = 7; i >= 0; i--) {
                int bit = (byte >> i) & 1;
                image[pixelOffset + bitIndex] = embedBit(image[pixelOffset + bitIndex], bit);
                bitIndex++;
            }
        }
        
        FileUtils::writeFile(outputFile, image);
    }
    
    /**
     * Reveals hidden data from BMP image
     * @param containerFile Path to BMP image with hidden data
     * @return Extracted data
     */
    vector<unsigned char> revealData(const string& containerFile) override {
        vector<unsigned char> image = FileUtils::readFile(containerFile);
        
        if (!isValidBMP(image)) {
            throw SteganographyException("File is not a valid BMP image");
        }
        
        size_t pixelOffset = getPixelDataOffset(image);
        size_t bitIndex = 0;
        
        // Extract and verify signature
        uint32_t signature = 0;
        for (int i = 0; i < 32; i++) {
            signature = (signature << 1) | extractBit(image[pixelOffset + bitIndex]);
            bitIndex++;
        }
        
        if (signature != SIGNATURE) {
            throw SteganographyException("No hidden data found in image");
        }
        
        // Extract data size
        uint32_t dataSize = 0;
        for (int i = 0; i < 32; i++) {
            dataSize = (dataSize << 1) | extractBit(image[pixelOffset + bitIndex]);
            bitIndex++;
        }
        
        if (dataSize == 0 || dataSize > image.size()) {
            throw SteganographyException("Invalid data size detected");
        }
        
        // Extract actual data
        vector<unsigned char> result(dataSize);
        for (size_t i = 0; i < dataSize; i++) {
            unsigned char byte = 0;
            for (int j = 0; j < 8; j++) {
                byte = (byte << 1) | extractBit(image[pixelOffset + bitIndex]);
                bitIndex++;
            }
            result[i] = byte;
        }
        
        return result;
    }
};

// ============================================================================
// Document Steganography (for PDF and DOCX using EOF appending)
// ============================================================================
class DocumentSteganography : public ISteganography {
private:
    static const uint32_t SIGNATURE = 0x44535447; // "DSTG" in hex
    
public:
    /**
     * Hides data in document by appending after EOF marker
     * @param data Data to hide
     * @param containerFile Path to PDF or DOCX file
     * @param outputFile Output file path
     */
    void hideData(const vector<unsigned char>& data, 
                  const string& containerFile, 
                  const string& outputFile) override {
        vector<unsigned char> document = FileUtils::readFile(containerFile);
        
        // Reserve space for signature and size
        vector<unsigned char> output;
        output.reserve(document.size() + sizeof(uint32_t) * 2 + data.size());
        
        // Copy original document
        output.insert(output.end(), document.begin(), document.end());
        
        // Append signature
        for (int i = 3; i >= 0; i--) {
            output.push_back((SIGNATURE >> (i * 8)) & 0xFF);
        }
        
        // Append data size
        uint32_t dataSize = data.size();
        for (int i = 3; i >= 0; i--) {
            output.push_back((dataSize >> (i * 8)) & 0xFF);
        }
        
        // Append actual data
        output.insert(output.end(), data.begin(), data.end());
        
        FileUtils::writeFile(outputFile, output);
    }
    
    /**
     * Reveals hidden data from document
     * @param containerFile Path to document with hidden data
     * @return Extracted data
     */
    vector<unsigned char> revealData(const string& containerFile) override {
        vector<unsigned char> document = FileUtils::readFile(containerFile);
        
        if (document.size() < sizeof(uint32_t) * 2) {
            throw SteganographyException("File too small to contain hidden data");
        }
        
        // Search for signature from end
        size_t sigPos = document.size() - sizeof(uint32_t) * 2;
        bool found = false;
        
        // Search backwards for signature
        for (size_t i = document.size() - sizeof(uint32_t) * 2; i > 0; i--) {
            uint32_t sig = 0;
            for (int j = 0; j < 4; j++) {
                sig = (sig << 8) | document[i + j];
            }
            
            if (sig == SIGNATURE) {
                sigPos = i;
                found = true;
                break;
            }
        }
        
        if (!found) {
            throw SteganographyException("No hidden data found in document");
        }
        
        // Extract data size
        uint32_t dataSize = 0;
        for (int i = 0; i < 4; i++) {
            dataSize = (dataSize << 8) | document[sigPos + 4 + i];
        }
        
        if (dataSize == 0 || sigPos + 8 + dataSize > document.size()) {
            throw SteganographyException("Invalid data size detected");
        }
        
        // Extract data
        vector<unsigned char> result(dataSize);
        for (size_t i = 0; i < dataSize; i++) {
            result[i] = document[sigPos + 8 + i];
        }
        
        return result;
    }
};

// ============================================================================
// Main Steganography Manager Class
// ============================================================================
class Steganography {
private:
    ISteganography* engine;
    
    /**
     * Selects appropriate steganography engine based on file type
     * @param containerFile Path to container file
     */
    void selectEngine(const string& containerFile) {
        string ext = FileUtils::getExtension(containerFile);
        
        if (ext == "bmp") {
            engine = new ImageSteganography();
        } else if (ext == "pdf" || ext == "docx") {
            engine = new DocumentSteganography();
        } else {
            throw SteganographyException("Unsupported file format: " + ext + 
                                        ". Supported: BMP, PDF, DOCX");
        }
    }

public:
    Steganography() : engine(nullptr) {}
    
    ~Steganography() {
        delete engine;
    }
    
    /**
     * Hides text in container file
     * @param text Text to hide
     * @param containerFile Path to container file
     * @param outputFile Output file path
     */
    void hideText(const string& text, const string& containerFile, 
                  const string& outputFile) {
        selectEngine(containerFile);
        
        vector<unsigned char> data(text.begin(), text.end());
        engine->hideData(data, containerFile, outputFile);
        
        cout << "✓ Successfully hidden " << text.length() 
             << " characters in " << outputFile << endl;
    }
    
    /**
     * Hides image file in container file
     * @param imageFile Path to image to hide
     * @param containerFile Path to container file
     * @param outputFile Output file path
     */
    void hideImage(const string& imageFile, const string& containerFile, 
                   const string& outputFile) {
        selectEngine(containerFile);
        
        vector<unsigned char> data = FileUtils::readFile(imageFile);
        engine->hideData(data, containerFile, outputFile);
        
        cout << "✓ Successfully hidden image (" << data.size() 
             << " bytes) in " << outputFile << endl;
    }
    
    /**
     * Reveals hidden text from container file
     * @param containerFile Path to container file
     * @return Extracted text
     */
    string revealText(const string& containerFile) {
        selectEngine(containerFile);
        
        vector<unsigned char> data = engine->revealData(containerFile);
        string text(data.begin(), data.end());
        
        cout << "✓ Successfully extracted " << text.length() 
             << " characters" << endl;
        return text;
    }
    
    /**
     * Reveals hidden data and saves to file
     * @param containerFile Path to container file
     * @param outputFile Output file path
     */
    void revealToFile(const string& containerFile, const string& outputFile) {
        selectEngine(containerFile);
        
        vector<unsigned char> data = engine->revealData(containerFile);
        FileUtils::writeFile(outputFile, data);
        
        cout << "✓ Successfully extracted " << data.size() 
             << " bytes to " << outputFile << endl;
    }
};

// ============================================================================
// Main Function - Interactive Menu
// ============================================================================
int main() {
    Steganography stego;
    
    cout << "============================================" << endl;
    cout << "   C++ Steganography Program" << endl;
    cout << "============================================" << endl;
    cout << "Supported formats: BMP, PDF, DOCX" << endl;
    cout << endl;
    
    while (true) {
        cout << "\nMenu:" << endl;
        cout << "1. Hide text in file" << endl;
        cout << "2. Hide image in file" << endl;
        cout << "3. Reveal text from file" << endl;
        cout << "4. Reveal data to file" << endl;
        cout << "5. Exit" << endl;
        cout << "\nChoice: ";
        
        int choice;
        cin >> choice;
        cin.ignore(); // Clear newline
        
        try {
            if (choice == 1) {
                string text, container, output;
                
                cout << "\nEnter text to hide: ";
                getline(cin, text);
                
                cout << "Container file (BMP/PDF/DOCX): ";
                getline(cin, container);
                
                cout << "Output file: ";
                getline(cin, output);
                
                stego.hideText(text, container, output);
                
            } else if (choice == 2) {
                string imageFile, container, output;
                
                cout << "\nImage file to hide: ";
                getline(cin, imageFile);
                
                cout << "Container file (BMP/PDF/DOCX): ";
                getline(cin, container);
                
                cout << "Output file: ";
                getline(cin, output);
                
                stego.hideImage(imageFile, container, output);
                
            } else if (choice == 3) {
                string container;
                
                cout << "\nContainer file: ";
                getline(cin, container);
                
                string text = stego.revealText(container);
                cout << "\nExtracted text:\n" << text << endl;
                
            } else if (choice == 4) {
                string container, output;
                
                cout << "\nContainer file: ";
                getline(cin, container);
                
                cout << "Output file: ";
                getline(cin, output);
                
                stego.revealToFile(container, output);
                
            } else if (choice == 5) {
                cout << "\nGoodbye!" << endl;
                break;
            } else {
                cout << "Invalid choice!" << endl;
            }
            
        } catch (const SteganographyException& e) {
            cout << "✗ Error: " << e.what() << endl;
        } catch (const exception& e) {
            cout << "✗ Unexpected error: " << e.what() << endl;
        }
    }
    
    return 0;
}