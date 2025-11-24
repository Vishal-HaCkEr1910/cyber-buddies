# 🚀 Complete Integration Guide - StegoProto

## 📋 Project Overview

**StegoProto** is a full-stack steganography application that allows users to hide files within images through a web interface.

### Architecture:

- **Frontend**: HTML/CSS/JavaScript (Modern UI)
- **Backend**: Node.js + Express (REST API)
- **Core Engine**: C++ (Steganography algorithms)

---

## ✅ Issues Fixed

### 1. Bug in `decode.js` - **FIXED** ✓

- **Issue**: Error messages were not displaying
- **Fix**: Changed `classList.add('hidden')` to `classList.remove('hidden')` in `showError()` function

### 2. C++ Interface Mismatch - **FIXED** ✓

- **Issue**: Original `stego.cpp` has interactive menu, but server needs CLI
- **Fix**: Created new `stego_cli.cpp` with command-line interface

---

## 🛠️ Installation & Setup

### Step 1: Install Node.js Dependencies

```powershell
cd c:\Users\HP\OneDrive\Desktop\Project_OOPs\cyber-buddies
npm install
```

This installs:

- `express` - Web server
- `multer` - File upload handling
- `nodemon` (dev dependency) - Auto-restart during development

### Step 2: Compile C++ Program

You have **TWO options**:

#### Option A: Use Command-Line Version (Recommended for Web Server)

```powershell
# Compile the CLI version
g++ -o stego.exe stego_cli.cpp -std=c++11
```

#### Option B: Use Original Interactive Version (For Testing)

```powershell
# Compile the original version with menu
g++ -o stego_interactive.exe stego.cpp -std=c++11
```

**Note**: The web server requires the CLI version (`stego.exe`).

### Step 3: Test C++ Program

Test the compiled program:

```powershell
# Test encode
.\stego.exe encode test_image.png secret.txt output_stego.png

# Test decode
.\stego.exe decode output_stego.png extracted_secret.txt
```

### Step 4: Start the Server

```powershell
# Production mode
npm start

# OR Development mode (auto-restart on changes)
npm run dev
```

The server starts on `http://localhost:3000`

### Step 5: Open in Browser

Navigate to: `http://localhost:3000`

---

## 📁 Complete Project Structure

```
cyber-buddies/
├── 📄 index.html              # Home page with project info
├── 📄 encode.html             # Encode/Hide file interface
├── 📄 decode.html             # Decode/Extract file interface
├── 📄 style.css               # Modern UI styling
├── 📄 encode.js               # Frontend logic for encoding
├── 📄 decode.js               # Frontend logic for decoding (FIXED)
├── 📄 server.js               # Backend Express server
├── 📄 package.json            # Node.js dependencies
├── 📄 stego.cpp               # Original C++ with interactive menu
├── 📄 stego_cli.cpp           # NEW: CLI version for web server
├── 🔧 stego.exe               # Compiled C++ executable
├── 📖 README.md               # Project documentation
├── 📖 INTEGRATION_GUIDE.md    # This file
├── 📁 uploads/                # Temporary uploaded files (auto-created)
└── 📁 output/                 # Generated stego files (auto-created)
```

---

## 🔄 How It Works

### Encoding Process:

1. **User uploads** cover image + secret file via `encode.html`
2. **Frontend** (`encode.js`) sends files to `/api/encode` endpoint
3. **Backend** (`server.js`) receives files, saves to `uploads/`
4. **Server executes**: `./stego.exe encode <cover> <secret> <output>`
5. **C++ program** embeds file and creates stego image in `output/`
6. **Server returns** download link to frontend
7. **User downloads** the stego image

### Decoding Process:

1. **User uploads** stego image via `decode.html`
2. **Frontend** (`decode.js`) sends file to `/api/decode` endpoint
3. **Backend** receives file, saves to `uploads/`
4. **Server executes**: `./stego.exe decode <stego> <output>`
5. **C++ program** extracts hidden file to `output/`
6. **Server returns** download link to frontend
7. **User downloads** the extracted file

---

## 🎯 API Endpoints

### POST `/api/encode`

- **Input**: `multipart/form-data`
  - `coverImage`: Image file
  - `secretFile`: File to hide
- **Output**: JSON
  ```json
  {
    "success": true,
    "message": "File encoded successfully",
    "outputFile": "/output/stego-1234567890.png",
    "filename": "stego-1234567890.png"
  }
  ```

### POST `/api/decode`

- **Input**: `multipart/form-data`
  - `stegoImage`: Stego image
  - `outputName`: (optional) Output filename
- **Output**: JSON
  ```json
  {
    "success": true,
    "message": "File decoded successfully",
    "outputFile": "/output/extracted-1234567890.bin",
    "filename": "extracted-1234567890.bin"
  }
  ```

### GET `/api/download/:filename`

- **Input**: filename in URL
- **Output**: File download

---

## 🔧 C++ Program Interface

### Command-Line Arguments (stego_cli.cpp):

```bash
# Encode
stego.exe encode <cover_image> <secret_file> <output_image>

# Decode
stego.exe decode <stego_image> <output_file>
```

### Features:

- ✅ Universal file type support
- ✅ Automatic size validation
- ✅ Original filename preservation
- ✅ Data integrity checking (checksum)
- ✅ Magic signature verification
- ✅ Robust error handling

---

## 🧪 Testing

### Test Files Needed:

1. **Cover Image**: Any image (PNG, JPG, BMP) - minimum 10 KB
2. **Secret File**: Any file type - max 85% of cover image size

### Test Procedure:

```powershell
# 1. Test C++ directly
.\stego.exe encode cover.png secret.txt stego_output.png
.\stego.exe decode stego_output.png extracted.txt

# 2. Start server
npm start

# 3. Test via browser
# - Open http://localhost:3000
# - Click "Encode File"
# - Upload cover image and secret file
# - Download stego image
# - Click "Decode File"
# - Upload stego image
# - Download extracted file
```

---

## 🐛 Troubleshooting

### Issue: "stego.exe not found"

**Solution**: Make sure you compiled the C++ program and it's in the project root

```powershell
g++ -o stego.exe stego_cli.cpp -std=c++11
```

### Issue: "File too large to hide"

**Solution**: The hidden file can only be up to 85% of the cover image size. Use a larger cover image.

### Issue: "No hidden data found in file"

**Solution**: The image was not created with this program. Make sure to use a properly encoded stego image.

### Issue: Server won't start - "Port already in use"

**Solution**: Another process is using port 3000. Either kill that process or change the port in `server.js`:

```javascript
const PORT = 3001; // Change to different port
```

### Issue: "Module not found" error

**Solution**: Install dependencies:

```powershell
npm install
```

---

## 🔒 Security Considerations

⚠️ **Important Notes**:

1. This is a **prototype/educational project**
2. Steganography is NOT encryption - files are hidden but not encrypted
3. For production use, add:
   - File type validation
   - Size limits
   - Rate limiting
   - Authentication
   - HTTPS
   - File encryption before hiding

---

## 🚀 Future Enhancements

- [ ] Add password protection for hidden files
- [ ] Support for multiple file hiding
- [ ] Image quality preservation options
- [ ] Preview before download
- [ ] History of encoded/decoded files
- [ ] Drag & drop file upload
- [ ] Progress bars for large files
- [ ] Support for video steganography

---

## 👥 Team

- **Vishal Yadav** - vishal@gmail.com
- **Aryan Sheoran** - aryan@gmail.com
- **Keshav Sethi** - keshav@gmail.com

**Institution**: IIITU

---

## 📝 License

MIT License - Educational Project

---

## 🎓 Learning Outcomes

This project demonstrates:

- Full-stack web development
- C++ and JavaScript integration
- File handling and binary operations
- RESTful API design
- Modern UI/UX design
- Error handling and validation
- Steganography concepts
- Process execution from Node.js

---

## ✨ Quick Start (TL;DR)

```powershell
# 1. Install dependencies
npm install

# 2. Compile C++ program
g++ -o stego.exe stego_cli.cpp -std=c++11

# 3. Start server
npm start

# 4. Open browser
# Navigate to http://localhost:3000

# 5. Test encode/decode features
```

---

**Need Help?** Check the README.md or contact the team!
