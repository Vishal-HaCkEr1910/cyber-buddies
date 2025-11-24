# StegoProto - Steganography Web Interface

A web-based steganography application that hides secret files inside images using C++ backend processing.

## 🚀 Setup Instructions

### Prerequisites

- Node.js (v14 or higher)
- npm (comes with Node.js)
- C++ compiler (g++ or clang++)
- Your C++ steganography code compiled as an executable

### Step 1: Install Dependencies

Open terminal in the project directory and run:

```bash
npm install
```

This will install:

- `express` - Web server framework
- `multer` - File upload handling

### Step 2: Compile Your C++ Code

You need to compile your `project_new.cpp` (or your C++ steganography code) into an executable named `stego`.

#### On Linux/Mac:

```bash
g++ -o stego project_new.cpp -std=c++11
```

#### On Windows:

```bash
g++ -o stego.exe project_new.cpp -std=c++11
```

**Important:** Your C++ program should accept command-line arguments in this format:

**For Encoding:**

```bash
./stego encode <cover_image_path> <secret_file_path> <output_image_path>
```

**For Decoding:**

```bash
./stego decode <stego_image_path> <output_file_path>
```

### Step 3: Adjust Server Configuration (if needed)

If your C++ executable has a different command format, edit `server.js`:

**Line 60-61** (Encode command):

```javascript
const command = `./stego encode "${coverImage}" "${secretFile}" "${outputImage}"`;
```

**Line 116** (Decode command):

```javascript
const command = `./stego decode "${stegoImage}" "${outputFile}"`;
```

### Step 4: Run the Server

```bash
npm start
```

Or for development with auto-restart:

```bash
npm run dev
```

The server will start on `http://localhost:3000`

### Step 5: Open in Browser

Navigate to:

```
http://localhost:3000
```

## 📁 Project Structure

```
Steganography/
├── server.js           # Backend Node.js server
├── package.json        # Node.js dependencies
├── index.html          # Home page
├── encode.html         # Encode page
├── decode.html         # Decode page
├── encode.js           # Encode page logic
├── decode.js           # Decode page logic
├── style.css           # Styling
├── stego               # Your compiled C++ executable
├── project_new.cpp     # Your C++ source code
├── uploads/            # Temporary file uploads (auto-created)
└── output/             # Processed files (auto-created)
```

## 🔧 Troubleshooting

### Error: "Cannot find module 'express'"

Run: `npm install`

### Error: "Encoding/Decoding failed"

- Check if your C++ executable is in the project root directory
- Verify the executable name matches what's in `server.js`
- Test your C++ program manually with command-line arguments
- Check the terminal for detailed error messages

### Error: "EACCES: permission denied"

On Linux/Mac, make your executable runnable:

```bash
chmod +x stego
```

### Port 3000 already in use

Change the PORT in `server.js` (line 8):

```javascript
const PORT = 8080; // or any other available port
```

## 🧪 Testing Your C++ Executable

Before running the server, test your C++ program manually:

```bash
# Test encoding
./stego encode test_image.png secret.txt output.png

# Test decoding
./stego decode output.png extracted.txt
```

## 📝 C++ Program Requirements

Your C++ program should:

1. Accept command-line arguments for file paths
2. Return exit code 0 on success
3. Print error messages to stderr on failure
4. Support common image formats (PNG, BMP, JPG)

## 🎯 Usage

1. **Home Page**: Overview and team information
2. **Encode Page**: Upload a cover image and secret file
3. **Decode Page**: Upload a stego image to extract hidden data
4. Click "Download" to get your processed files

## 👥 Team

- Vishal Yadav
- Aryan Sheoran
- Keshav Sethi

## 🏫 Institution

IIITU

---

**Note**: This is an academic project for demonstration purposes.
