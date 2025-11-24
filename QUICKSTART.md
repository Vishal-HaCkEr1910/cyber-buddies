# 🚀 QUICK START - StegoProto

## ⚡ Get Running in 3 Steps!

### Step 1: Setup (One-time)

```powershell
.\setup.bat
```

This will:

- ✅ Check Node.js installation
- ✅ Check C++ compiler
- ✅ Install npm dependencies
- ✅ Compile C++ program

### Step 2: Start Server

```powershell
npm start
```

### Step 3: Open Browser

```
http://localhost:3000
```

---

## 🎯 Using the Application

### To Hide a File:

1. Click **"Encode File"**
2. Upload **cover image** (PNG/JPG)
3. Upload **secret file** (any type)
4. Click **"🔒 Embed Secret File"**
5. **Download** the stego image

### To Extract Hidden File:

1. Click **"Decode File"**
2. Upload the **stego image**
3. Click **"🔓 Extract & Download"**
4. **Download** the extracted file

---

## 📝 Important Notes

✅ **Cover image must be at least 10 KB**
✅ **Hidden file must be ≤ 85% of cover image size**
✅ **Supports any file type**
✅ **Original filename is preserved**

---

## 🐛 Quick Fixes

### Problem: Setup failed?

```powershell
# Install Node.js from: https://nodejs.org/
# Install MinGW from: https://winlibs.com/
# Then run setup.bat again
```

### Problem: Encoding/Decoding fails?

```powershell
# Recompile the C++ program:
g++ -o stego.exe stego_cli.cpp -std=c++11
```

### Problem: File too large?

- Use a **bigger cover image**
- OR use a **smaller secret file**
- Remember: secret file ≤ 85% of cover image

---

## 🧪 Test Your Setup

```powershell
# Run automated tests:
.\test.bat
```

---

## 📚 More Information

- **README.md** - Full documentation
- **INTEGRATION_GUIDE.md** - Detailed integration guide
- **Server logs** - Check console for errors

---

## 👥 Team

- Vishal Yadav - vishal@gmail.com
- Aryan Sheoran - aryan@gmail.com
- Keshav Sethi - keshav@gmail.com

**IIITU**

---

**That's it! Enjoy using StegoProto! 🎉**
