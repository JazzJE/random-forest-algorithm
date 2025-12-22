import { useState, useRef } from "react";

function FileUploader() {
  const [selectedFile, setSelectedFile] = useState<File | null>(null);
  const [headers, setHeaders] = useState<string[]>([]);
  const [continuousFeatures, setContinuousFeatures] = useState<Set<string>>(new Set());
  const [targetLabel, setTargetLabel] = useState<string | null>(null);


  const fileInputRef = useRef<HTMLInputElement>(null);

  const isLikelyCSV = (file: File) => {
    const validMimeTypes = [
      "text/csv",
      "application/vnd.ms-excel" // common on Windows
    ];

    const hasCsvExtension = file.name.toLowerCase().endsWith(".csv");

    return validMimeTypes.includes(file.type) || hasCsvExtension;
  };

  const parseHeaders = async (file: File) => {
    const text = await file.text();          // Read entire file as string
    const firstLine = text.split(/\r?\n/)[0]; // Get the first line
    const headers = firstLine.split(",");

    return headers;
  };

  const onFileChange = async (e: React.ChangeEvent<HTMLInputElement>) => {
    // Verify the file is a CSV
    const chosenFile = e.target.files?.[0] ?? null;
    if (!chosenFile || !isLikelyCSV(chosenFile)) {
      if (fileInputRef.current) {
        fileInputRef.current.value = "";
      }
      alert("[ERROR] Please upload a CSV file");
      return;
    }

    // Update the states for each
    const newHeaders = await parseHeaders(chosenFile);
    setHeaders(newHeaders);
    setContinuousFeatures(new Set());
    setTargetLabel(null);

    setSelectedFile(chosenFile);
  };

  const onFileUpload = async () => {
    // If there is no file uploaded, then return and alert the user
    if (!selectedFile) {
      alert("[ERROR] No file selected!");
      return;
    }

    // If no target label was selected, then return and alert the user
    if (!targetLabel) {
      alert("[ERROR] No target label was selected!");
      return;
    }

    // Append form data
    const formData = new FormData();
    formData.append("file", selectedFile); // MUST match server key
    formData.append("continuous_features", JSON.stringify([...continuousFeatures]));
    formData.append("target_label", targetLabel);
    formData.append("headers", JSON.stringify(headers));

    console.log(continuousFeatures);
    console.log(targetLabel);
    console.log(headers);

    console.log(formData);
    let res;

    try {
      res = await fetch("http://localhost:8080/upload_dataset", {
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
      console.error(res);
    }
  };

  return (
    <div>
      <input type="file" ref={fileInputRef} accept=".csv,text/csv" onChange={onFileChange} />
      <button onClick={onFileUpload}>Upload</button>

      {selectedFile && (
        <div>

          {/* Metadata */}
          <p>Name: {selectedFile.name}</p>
          <p>Type: {selectedFile.type}</p>
          <p>Size: {selectedFile.size} bytes</p>

          {/* Continuous feature elements chooser */}
          <p>Select the feature columns that should be treated as <strong><em>continuous</em></strong>:</p>
          <div id="continuous-feature-checklist">
          {headers.map((header) => (
            <label key={header} style={{ display: "block" }}>
              <input
                type="checkbox"
                disabled={targetLabel === header}
                checked={continuousFeatures.has(header)}
                onChange={(e) => {
                  setContinuousFeatures(prev => {
                    const next = new Set(prev);
                    if (e.target.checked) {
                      next.add(header);
                    } else {
                      next.delete(header);
                    }
                    return next;
                  });
                }}
              />
              {header}
            </label>
          ))}
          </div>

          {/* Target label chooser */}
          <p>Select <strong><em>one</em></strong> of your feature columns to be your <strong><em>target labels</em></strong>:</p>
          <div>
            {headers.map((header) => (
              <label key={header} style={{ display: "block" }}>
                <input
                  type="radio"
                  disabled={continuousFeatures.has(header)}
                  name="targetLabel"
                  value={header}
                  checked={targetLabel === header}
                  onChange={() => setTargetLabel(header)}
                />
                {header}
              </label>
            ))}
          </div>

        </div>
      )}
    </div>
  );
}

export default FileUploader;
