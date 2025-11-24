document.getElementById('encodeForm').addEventListener('submit', async (e) => {
  e.preventDefault();

  const formData = new FormData();
  const coverImage = document.getElementById('coverImage').files[0];
  const secretFile = document.getElementById('secretFile').files[0];

  if (!coverImage || !secretFile) {
    showError('Please select both files');
    return;
  }

  formData.append('coverImage', coverImage);
  formData.append('secretFile', secretFile);

  // Show loading, hide results
  showLoading();
  hideResult();
  hideError();

  try {
    const response = await fetch('/api/encode', {
      method: 'POST',
      body: formData
    });

    const data = await response.json();

    hideLoading();

    if (data.success) {
      showResult(data.message, data.outputFile, data.filename);
    } else {
      showError(data.error || 'Encoding failed');
    }
  } catch (error) {
    hideLoading();
    showError('Network error: ' + error.message);
  }
});

function showLoading() {
  document.getElementById('loading').classList.remove('hidden');
}

function hideLoading() {
  document.getElementById('loading').classList.add('hidden');
}

function showResult(message, filePath, filename) {
  document.getElementById('resultMessage').textContent = message;
  document.getElementById('result').classList.remove('hidden');
  
  const downloadBtn = document.getElementById('downloadBtn');
  downloadBtn.onclick = () => {
    window.location.href = `/api/download/${filename}`;
  };
}

function hideResult() {
  document.getElementById('result').classList.add('hidden');
}

function showError(message) {
  document.getElementById('errorMessage').textContent = message;
  document.getElementById('error').classList.remove('hidden');
}

function hideError() {
  document.getElementById('error').classList.add('hidden');
}
