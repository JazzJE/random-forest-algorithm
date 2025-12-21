import { useState } from "react";

function FileUploader() {
  const [selectedFile, setSelectedFile] = useState<File | null>(null);

  const onFileChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    setSelectedFile(e.target.files?.[0] ?? null);
  };

  const onFileUpload = async () => {
    if (!selectedFile) return;

    const formData = new FormData();
    formData.append("file", selectedFile); // MUST match server key

    try {
      const res = await fetch("http://localhost:8080/upload_dataset", {
        method: "POST",
        body: formData,
        // DO NOT set Content-Type manually
      });

      if (!res.ok) {
        throw new Error(`Upload failed: ${res.status}`);
      }

      const text = await res.text();
      console.log(text);
    } catch (err) {
      console.error("Upload failed:", err);
    }
  };

  return (
    <div>
      <input type="file" onChange={onFileChange} />
      <button onClick={onFileUpload}>Upload</button>

      {selectedFile && (
        <div>
          <p>Name: {selectedFile.name}</p>
          <p>Type: {selectedFile.type}</p>
          <p>Size: {selectedFile.size} bytes</p>
        </div>
      )}
    </div>
  );
}

export default FileUploader;
