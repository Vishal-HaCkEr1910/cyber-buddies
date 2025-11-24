# 🔐 StegoProto - Steganography Web Interface

A full-stack web-based steganography application that hides secret files inside images using C++ backend processing.

## ✨ Features

- 🎯 **Universal File Support** - Hide ANY file type in ANY image
- � **Secure Encoding** - Advanced steganography with data integrity checking
- 🔓 **Easy Extraction** - Simple decode process to retrieve hidden files
- 🌐 **Modern Web Interface** - Clean, intuitive UI built with HTML/CSS/JavaScript
- ⚡ **Fast Processing** - Efficient C++ backend for quick operations
- 📊 **File Validation** - Automatic size checking and capacity management

## �🚀 Quick Setup (Automated)

### For Windows Users:

1. **Run the setup script:**

   ```powershell
   .\setup.bat
   ```

2. **Start the server:**

   ```powershell
   npm start
   ```

3. **Open your browser:**
   ```
   http://localhost:3000
   ```

That's it! 🎉

---

## 📋 Manual Setup Instructions

### Prerequisites

- **Node.js** (v14 or higher) - [Download](https://nodejs.org/)
- **npm** (comes with Node.js)
- **C++ compiler** (g++ or clang++) - [Download MinGW](https://winlibs.com/)

### Step 1: Install Node.js Dependencies

Open PowerShell in the project directory and run:

```powershell
npm install
```

This will install:

- `express` - Web server framework
- `multer` - File upload handling

### Step 2: Compile C++ Program

Compile the command-line version of the steganography program:

```powershell
g++ -o stego.exe stego_cli.cpp -std=c++11
```

**Note:** Use `stego_cli.cpp` (command-line interface) for the web server, not `stego.cpp` (interactive menu).

### Step 3: Test the C++ Program (Optional)

Run the test script:

```powershell
.\test.bat
```

Or test manually:

```powershell
# Encode
.\stego.exe encode cover.png secret.txt output_stego.png

# Decode
.\stego.exe decode output_stego.png extracted_secret.txt
```

### Step 4: Start the Web Server

```powershell
# Production mode
npm start

# OR Development mode (auto-restart on changes)
npm run dev
```

The server will start on `http://localhost:3000`

### Step 5: Open in Browser

Navigate to:

```
http://localhost:3000
```

## 📁 Complete Project Structure

```
cyber-buddies/
├── 📄 index.html              # Home page with project info
├── 📄 encode.html             # Encode/Hide file interface
├── 📄 decode.html             # Decode/Extract file interface
├── 📄 style.css               # Modern UI styling
├── 📄 encode.js               # Frontend logic for encoding
├── 📄 decode.js               # Frontend logic for decoding
├── 📄 server.js               # Backend Express server
├── 📄 package.json            # Node.js dependencies
├── 📄 stego.cpp               # Original C++ with interactive menu
├── 📄 stego_cli.cpp           # CLI version for web server ⭐
├── 🔧 stego.exe               # Compiled C++ executable
├── 📖 README.md               # This file
├── 📖 INTEGRATION_GUIDE.md    # Detailed integration guide
├── 🔧 setup.bat               # Automated setup script
├── 🔧 test.bat                # Testing script
├── 📁 uploads/                # Temporary uploaded files (auto-created)
└── 📁 output/                 # Generated stego files (auto-created)
```

## 🎯 How to Use

### Encoding (Hiding a File):

1. Navigate to **Home** → Click "**Encode File**"
2. **Select Cover Image**: Choose an image file (PNG, JPG, BMP)
3. **Select Secret File**: Choose any file you want to hide
4. Click "**🔒 Embed Secret File**"
5. **Download** the generated stego image

### Decoding (Extracting Hidden File):

1. Navigate to **Home** → Click "**Decode File**"
2. **Select Stego Image**: Upload the image containing hidden data
3. (Optional) Specify output filename
4. Click "**🔓 Extract & Download**"
5. **Download** the extracted secret file

## 🔧 Troubleshooting

### ❌ Error: "Cannot find module 'express'"

**Solution:**

```powershell
npm install
```

### ❌ Error: "stego.exe not found" or "Encoding/Decoding failed"

**Solutions:**

- Check if `stego.exe` is in the project root directory
- Recompile the C++ program:
  ```powershell
  g++ -o stego.exe stego_cli.cpp -std=c++11
  ```
- Test the executable manually:
  ```powershell
  .\stego.exe encode test.png secret.txt output.png
  ```

### ❌ Error: "File too large to hide"

**Solution:** The hidden file can only be up to **85%** of the cover image size. Use a larger cover image or smaller secret file.

### ❌ Error: "No hidden data found in file"

**Solution:** The uploaded image was not created with this program. Make sure to use a properly encoded stego image.

### ❌ Port 3000 already in use

**Solution:** Change the PORT in `server.js`:

```javascript
const PORT = 3001; // Change to any available port
```

### ❌ Error: "Permission denied" (Linux/Mac)

**Solution:** Make the executable runnable:

```bash
chmod +x stego
```

## 🧪 Testing

### Automated Testing:

```powershell
.\test.bat
```

### Manual Testing:

```powershell
# 1. Test C++ program
.\stego.exe encode cover.png secret.txt stego_output.png
.\stego.exe decode stego_output.png extracted.txt

# 2. Start server
npm start

# 3. Test in browser
# Open http://localhost:3000 and test encode/decode
```

## 📝 Technical Details

### C++ Program Features:

- ✅ **Universal file support** - Works with any file type
- ✅ **Magic signature** - 0x5354454E for file identification
- ✅ **Data integrity** - Checksum validation
- ✅ **Filename preservation** - Original filename stored in header
- ✅ **Size validation** - Automatic capacity checking
- ✅ **Error handling** - Comprehensive exception handling

### API Endpoints:

**POST `/api/encode`**

- Input: `coverImage`, `secretFile` (multipart/form-data)
- Output: JSON with download link

**POST `/api/decode`**

- Input: `stegoImage`, `outputName` (optional)
- Output: JSON with download link

**GET `/api/download/:filename`**

- Input: filename in URL
- Output: File download

### Size Limits:

- **Minimum cover image**: 10 KB
- **Maximum hidden file**: 85% of cover image size
- **Upload limit**: 50 MB per file

## 🔒 Security Notes

⚠️ **Important**: This is a **prototype/educational project**

- Steganography ≠ Encryption (files are hidden, not encrypted)
- For production use, add:
  - Authentication & authorization
  - File encryption before hiding
  - HTTPS/TLS
  - Rate limiting
  - Enhanced validation

## 📚 Additional Resources

- **INTEGRATION_GUIDE.md** - Detailed integration instructions
- **stego.cpp** - Original C++ with interactive menu
- **stego_cli.cpp** - Command-line version for web server

## 🚀 Future Enhancements

- [ ] Password protection for hidden files
- [ ] Multiple file hiding
- [ ] Progress bars for large files
- [ ] Drag & drop file upload
- [ ] File preview before download
- [ ] Video steganography support

## 👥 Team

- **Vishal Yadav** - vishal@gmail.com
- **Aryan Sheoran** - aryan@gmail.com
- **Keshav Sethi** - keshav@gmail.com

**Institution**: IIITU

## 📄 License

MIT License - Educational Project

---

## 💡 Need Help?

1. Check **INTEGRATION_GUIDE.md** for detailed instructions
2. Run `.\test.bat` to verify your setup
3. Check server console for error messages
4. Ensure all prerequisites are installed

**Happy Steganography! 🎉**

---

**Note**: This is an academic project for demonstration purposes.
