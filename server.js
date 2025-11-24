const express = require('express');
const multer = require('multer');
const path = require('path');
const { exec } = require('child_process');
const fs = require('fs');

const app = express();
const PORT = 3000;

// Configure multer for file uploads
const storage = multer.diskStorage({
  destination: (req, file, cb) => {
    const uploadDir = './uploads';
    if (!fs.existsSync(uploadDir)) {
      fs.mkdirSync(uploadDir, { recursive: true });
    }
    cb(null, uploadDir);
  },
  filename: (req, file, cb) => {
    cb(null, Date.now() + '-' + file.originalname);
  }
});

const upload = multer({ 
  storage: storage,
  limits: { fileSize: 50 * 1024 * 1024 } // 50MB limit
});

// Serve static files (HTML, CSS, JS)
app.use(express.static(__dirname));
app.use('/uploads', express.static('uploads'));
app.use('/output', express.static('output'));

// Create necessary directories
['uploads', 'output'].forEach(dir => {
  if (!fs.existsSync(dir)) {
    fs.mkdirSync(dir, { recursive: true });
  }
});

// Encode endpoint
app.post('/api/encode', upload.fields([
  { name: 'coverImage', maxCount: 1 },
  { name: 'secretFile', maxCount: 1 }
]), (req, res) => {
  try {
    if (!req.files['coverImage'] || !req.files['secretFile']) {
      return res.status(400).json({ 
        success: false, 
        error: 'Both cover image and secret file are required' 
      });
    }

    const coverImage = req.files['coverImage'][0].path;
    const secretFile = req.files['secretFile'][0].path;
    const outputImage = `./output/stego-${Date.now()}.png`;

    console.log('Encoding files:', { coverImage, secretFile, outputImage });

    // Call your C++ executable
    // Adjust the command based on your C++ program's interface
    // Format: stego.exe encode <cover_image> <secret_file> <output_image>
    const command = `stego.exe encode "${coverImage}" "${secretFile}" "${outputImage}"`;

    exec(command, (error, stdout, stderr) => {
      // Clean up uploaded files
      try {
        fs.unlinkSync(coverImage);
        fs.unlinkSync(secretFile);
      } catch (cleanupError) {
        console.error('Cleanup error:', cleanupError);
      }

      if (error) {
        console.error(`Encoding error: ${error.message}`);
        console.error(`stderr: ${stderr}`);
        return res.status(500).json({ 
          success: false, 
          error: 'Encoding failed: ' + (stderr || error.message)
        });
      }

      console.log('Encoding successful:', stdout);

      res.json({
        success: true,
        message: 'File encoded successfully',
        outputFile: outputImage.replace('./', '/'),
        filename: path.basename(outputImage)
      });
    });
  } catch (error) {
    console.error('Server error:', error);
    res.status(500).json({ 
      success: false, 
      error: 'Server error: ' + error.message 
    });
  }
});

// Decode endpoint
app.post('/api/decode', upload.single('stegoImage'), (req, res) => {
  try {
    if (!req.file) {
      return res.status(400).json({ 
        success: false, 
        error: 'Stego image is required' 
      });
    }

    const stegoImage = req.file.path;
    const outputFile = `./output/extracted-${Date.now()}.bin`;

    console.log('Decoding file:', { stegoImage, outputFile });

    // Call your C++ executable
    // Format: stego.exe decode <stego_image> <output_file>
    const command = `stego.exe decode "${stegoImage}" "${outputFile}"`;

    exec(command, (error, stdout, stderr) => {
      // Clean up uploaded file
      try {
        fs.unlinkSync(stegoImage);
      } catch (cleanupError) {
        console.error('Cleanup error:', cleanupError);
      }

      if (error) {
        console.error(`Decoding error: ${error.message}`);
        console.error(`stderr: ${stderr}`);
        return res.status(500).json({ 
          success: false, 
          error: 'Decoding failed: ' + (stderr || error.message)
        });
      }

      console.log('Decoding successful:', stdout);

      res.json({
        success: true,
        message: 'File decoded successfully',
        outputFile: outputFile.replace('./', '/'),
        filename: path.basename(outputFile)
      });
    });
  } catch (error) {
    console.error('Server error:', error);
    res.status(500).json({ 
      success: false, 
      error: 'Server error: ' + error.message 
    });
  }
});

// Download endpoint
app.get('/api/download/:filename', (req, res) => {
  const filename = req.params.filename;
  const filepath = path.join(__dirname, 'output', filename);
  
  if (fs.existsSync(filepath)) {
    res.download(filepath);
  } else {
    res.status(404).json({ error: 'File not found' });
  }
});

app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});
